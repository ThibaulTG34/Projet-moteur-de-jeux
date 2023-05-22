// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

#include <common/Entity.h>
#include <common/EntityRoot.h>
#include <common/Sphere.h>
#include <common/Terrain.h>
#include <common/objloader.hpp>
#include <common/shader.hpp>
#include <common/skybox.h>
#include <common/vboindexer.hpp>
#include <common/cube.h>
#include <common/BBOX.h>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPosNormal = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraPosLibre = glm::vec3(2.f, 3.f, 7.f);
glm::vec3 cameraPosOrbit = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraOrbitTarget = glm::vec3(0., 0., 0.);

float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
float rotation_speed = 0.01f;

//Mecanique
int poids_character = 2;
float v0_angle = 45.0f;
vec3 speedVector = vec3(10 * cos(v0_angle), 10 * sin(v0_angle), 0.);
vec3 gravite = vec3(0., -poids_character * 9.81, 0.);
vec3 acceleration = vec3(gravite.x / poids_character, gravite.y / poids_character, gravite.z / poids_character);
vec3 normale_au_terrain;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// rotation
float angle = 0.;
float zoom = 1.;

//Declaration des booleens
bool collision = false;
bool firstMouse = true;
bool isCrounched = false;
bool isJumping = false;
bool starting = false;
bool horsZone = false;
bool isColliding = false;
bool movePlan;

//Autres variables utiles
float ismoving = 0.05;
int cpt = 0;
int wait = 0;

GLuint programScene;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

//Declaration des objets de la scene
EntityRoot Racine;
skybox Skybox;
Entity head, right_leg, left_leg, right_arm, left_arm, bust, infinite_plane;
Terrain terrain1, terrain2, terrain3, terrain4, terrain5, terrain6;
Entity left_leg_transform, right_leg_transform;
cube Obstacle;

//Declaration des textures
string textureTerrain("mur.jpg");
string heightmap("Heightmap_Rocky.png");

//Positions initiales
vec3 position_character = vec3(10, 0, 0);
vec3 position_during_jump;

// Variables pour les angles de rotation des jambes
float leftLeg_angle = 0.0f;
float rightLeg_angle = 0.0f;

// Variables pour le sens de rotation des jambes
int leftLeg_direction = 1;
int rightLeg_direction = -1;

//Parametres de construction de la skybox
std::vector<std::string> faces = {"rainbow/rainbow_lf.png", "rainbow/rainbow_rt.png",
                                   "rainbow/rainbow_up.png", "rainbow/rainbow_dn.png",
                                   "rainbow/rainbow_ft.png", "rainbow/rainbow_bk.png"};

float skybox_size = 30.f;
int skybox_mode = 2;


int main(void)
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                 GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(2048, 1500, "TP1 - GLFW", NULL, NULL);
  if (window == NULL)
  {
    fprintf(stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are "
            "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return -1;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  // Hide the mouse and enable unlimited mouvement
  //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Set the mouse at the center of the screen
  glfwPollEvents();
  glfwSetCursorPos(window, 2048 / 2, 1500 / 2);

  // Dark blue background
  glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  // Cull triangles which normal is not towards the camera

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  programScene = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");

  Racine = EntityRoot();
  Skybox = skybox(faces, skybox_size, skybox_mode);
  head = Entity("head.obj", "rock.png", 2, 1);
  bust = Entity("bust.obj", "rock.png", 2, 1);
  bust.transform.updateScaling(vec3(0.1, 0.1, 0.1));
  bust.transform.updateTranslate(position_character);

  right_leg = Entity("right_leg.obj", "rock.png", 2, 1);
  right_leg_transform = Entity();
  right_leg_transform.addChildren(right_leg);

  left_leg = Entity("left_leg.obj", "rock.png", 2, 1);
  left_leg_transform = Entity();
  left_leg_transform.addChildren(left_leg);

  right_arm = Entity("right_arm.obj", "rock.png", 2, 1);
  left_arm = Entity("left_arm.obj", "rock.png", 2, 1);

  infinite_plane = Entity();
  terrain1 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programScene);

  vec3 v1 = terrain1.sommets[terrain1.indices[0]] - terrain1.sommets[terrain1.indices[1]];
  vec3 v2 = terrain1.sommets[terrain1.indices[0]] - terrain1.sommets[terrain1.indices[2]];
  normale_au_terrain = cross(v1, v2);
  normale_au_terrain = normalize(normale_au_terrain);

  terrain2 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programScene);
  terrain3 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programScene);
  terrain4 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programScene);
  terrain5 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programScene);
  terrain6 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programScene);

  infinite_plane.addChildren(terrain1);
  infinite_plane.addChildren(terrain2);
  infinite_plane.addChildren(terrain3);
  infinite_plane.addChildren(terrain4);
  infinite_plane.addChildren(terrain5);
  infinite_plane.addChildren(terrain6);

  Obstacle = cube(1, 0.4, 0.4, 0.4, 4);
  terrain2.addChildren(Obstacle);
  Obstacle.transform.updateTranslate(vec3(-1, 0., 0.));

  float offset = terrain1.FindMaxZ();

  vec3 pos1 = vec3(2., 0., 0.);
  vec3 pos2 = vec3(2, 0, -offset - 2);
  vec3 pos3 = vec3(2, 0, -2 * offset - 4);
  vec3 pos4 = vec3(2, 0, -3 * offset - 6);
  vec3 pos5 = vec3(2, 0, -4 * offset - 8);
  vec3 pos6 = vec3(2, 0, -5 * offset - 10);

  terrain1.transform.updateTranslate(pos1);
  terrain2.transform.updateTranslate(pos2);
  terrain3.transform.updateTranslate(pos3);
  terrain4.transform.updateTranslate(pos4);
  terrain5.transform.updateTranslate(pos5);
  terrain6.transform.updateTranslate(pos6);

  Racine.addChildren(Skybox);
  Skybox.addChildren(bust);
  Skybox.addChildren(infinite_plane);

  bust.addChildren(head);
  bust.addChildren(left_arm);
  bust.addChildren(right_arm);
  bust.addChildren(left_leg_transform);
  bust.addChildren(right_leg_transform);

  bust.transform.updateRotationY(195);

  bust.transform.updateTranslate(vec3(0.4, 0.2, 4.));

  left_leg_transform.transform.updateTranslate(vec3(-0.05, -0.2, 0.));
  right_leg_transform.transform.updateTranslate(vec3(0.1, -0.2, 0.));

  glUseProgram(programScene);
  GLuint LightID = glGetUniformLocation(programScene, "LightPosition_worldspace");

  double lastTime = glfwGetTime();
  int nbFrames = 0;

  ViewMatrix = glm::mat4(1.f);
  ProjectionMatrix = glm::mat4(1.f);

  do
  {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programScene);

    ProjectionMatrix = glm::perspective(glm::radians(fov), (float)4 / (float)3, 0.1f, 100.f);
    ViewMatrix = glm::lookAt(cameraPosLibre, cameraPosLibre + cameraFront, cameraUp);

    Racine.updateSelfAndChild();
    Racine.drawEntity(programScene);

    if (movePlan){
      terrain1.transform.updateTranslate(vec3(0, 0, ismoving));
      if (cpt == 0){
        terrain2.transform.updateTranslate(vec3(0, 0, ismoving));
        terrain3.transform.updateTranslate(vec3(0, 0, ismoving));
        terrain4.transform.updateTranslate(vec3(0, 0, ismoving));
        terrain5.transform.updateTranslate(vec3(0, 0, ismoving));
        terrain6.transform.updateTranslate(vec3(0, 0, ismoving));
      }
    }


    if (terrain1.transform.position.z - 2.5 > bust.transform.position.z)
    {
      terrain1.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain2.transform.position.z - 2.5 > bust.transform.position.z)
    {
      terrain2.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain3.transform.position.z - 2.5 > bust.transform.position.z)
    {
      terrain3.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain4.transform.position.z - 2.5 > bust.transform.position.z)
    {
      terrain4.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain5.transform.position.z - 2.5 > bust.transform.position.z)
    {
      terrain5.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain6.transform.position.z - 2.5 > bust.transform.position.z)
    {
      terrain6.transform.updateTranslate(vec3(0, 0, pos6.z));
    }

    if (terrain2.transform.position.z > right_leg.FindMinPoint().z + bust.transform.position.z - 0.2 && !isColliding){
      isColliding = true;
      movePlan = false;
      bust.transform.updateTranslate(vec3(0, 0, 1));
      movePlan = true;
    }

    if (collision)
    {
      vec3 velocity = speedVector + acceleration * deltaTime;
      position_during_jump = position_during_jump + speedVector * deltaTime + acceleration * deltaTime * deltaTime;
      speedVector = velocity;
      if (position_during_jump.y - 0.2 < terrain1.sommets[0][1] || position_during_jump.y < terrain2.sommets[0][1] || position_during_jump.y < terrain3.sommets[0][1])
      {
        isJumping = false;
        speedVector = vec3(0., 0., 0.);
        // speedVector = speedVector - 2 * dot(normale_au_terrain, speedVector) * normale_au_terrain;
        position_during_jump = vec3(position_during_jump.x, 0., position_during_jump.z);
      }
      bust.transform.updateTranslate(vec3(0, speedVector.y * deltaTime, 0));
    }

    if (wait % 10 == 0 && !isJumping && starting){

      right_leg_transform.transform.identity();
      right_leg_transform.transform.updateRotationX(rightLeg_angle);
      right_leg_transform.transform.updateTranslate(vec3(0.1, -0.2f, 0.f));

      left_leg_transform.transform.identity();
      left_leg_transform.transform.updateRotationX(leftLeg_angle);
      left_leg_transform.transform.updateTranslate(vec3(-0.05, -0.2, 0.));

      leftLeg_angle += 5.0f * leftLeg_direction;
      rightLeg_angle += 5.0f * rightLeg_direction;

      if (leftLeg_angle >= 5.0f || leftLeg_angle <= -1.0f){
        leftLeg_direction *= -1;
      }
      if (rightLeg_angle >= 5.0f || rightLeg_angle <= -1.0f){
        rightLeg_direction *= -1;
      }
    }

    wait++;

    glUniformMatrix4fv(glGetUniformLocation(programScene, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programScene, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);


  glDeleteProgram(programScene);
  glDeleteVertexArrays(1, &VertexArrayID);

  glfwTerminate();

  return 0;
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = static_cast<float>(3.5 * deltaTime);

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    cameraPosLibre -= cameraSpeed * cameraFront;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    cameraPosLibre += cameraSpeed * cameraFront;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPosLibre -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPosLibre += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    cameraPosLibre += cameraSpeed * cameraUp;

  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    cameraPosLibre -= cameraSpeed * cameraUp;

  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
  {
    starting = true;
    movePlan = true;
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    isJumping = true;
    collision = true;
    speedVector = vec3(0., 5 * sin(v0_angle), -5 * cos(v0_angle));
    position_during_jump = bust.transform.position;
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    if (isCrounched)
    {
      right_leg_transform.transform.identity();
      left_leg_transform.transform.identity();
      bust.transform.updateRotationX(-80.f);
      right_leg_transform.transform.updateRotationX(80.f);
      right_leg_transform.transform.updateTranslate(vec3(0.1, -0.2f, 0.f));
      left_leg_transform.transform.updateRotationX(80.f);
      left_leg_transform.transform.updateTranslate(vec3(-0.05, -0.2, 0.));
      isCrounched = false;
    }
  }

   if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    if (!isCrounched)
    {
      right_leg_transform.transform.identity();
      left_leg_transform.transform.identity();
      bust.transform.updateRotationX(80.f);
      right_leg_transform.transform.updateRotationX(-80.f);
      right_leg_transform.transform.updateTranslate(vec3(0.1, -0.2f, 0.f));
      left_leg_transform.transform.updateRotationX(-80.f);
      left_leg_transform.transform.updateTranslate(vec3(-0.05, -0.2, 0.));
      isCrounched = true;
    }
  }

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}
