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
glm::vec3 cameraPosLibre = glm::vec3(3.f, 3.f, 12.f);
glm::vec3 cameraPosOrbit = glm::vec3(5.0f, 5.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraOrbitTarget = glm::vec3(0., 0., 0.);

float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
float rotation_speed = 0.01f;

// Mecanique
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

//Declaration des matrices
glm::mat4 ModelMatrix;
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

// Declaration des booleens
bool collision = false;
bool firstMouse = true;
bool isCrounched = false;
bool isJumping = false;
bool starting = false;
bool horsZone = false;
bool isColliding = false;
bool normal_mode = false;
bool free_mode = false;
bool orbit_mode = false; 
bool movePlan;

// Autres variables utiles
float ismoving = 0.05;
int cpt = 0;
int wait = 0;

GLuint programScene;

// Declaration des objets de la scene
EntityRoot Racine;
skybox Skybox;
Entity head, right_leg, left_leg, right_arm, left_arm, bust, infinite_plane;
BBOX rLegBox, lLegBox, rocksBox;
Terrain terrain1, terrain2, terrain3, terrain4, terrain5, terrain6;
Entity left_leg_transform, right_leg_transform;
Entity Rocks, Wood, Tree;

// Declaration des textures
string textureTerrain("herbe.jpeg");
string textureRoche("rock_text.jpeg");
string textureBois("Image_0.jpeg");
string textureArbre("gloss.jpg");
string texturePeau("peau.jpeg");

string heightmap("Heightmap_Rocky.png");

// Positions initiales
vec3 position_character = vec3(10, 0, 0);
vec3 position_during_jump;

// Variables pour les angles de rotation des jambes
float leftLeg_angle = 0.0f;
float rightLeg_angle = 0.0f;

// Variables pour le sens de rotation des jambes
int leftLeg_direction = 1;
int rightLeg_direction = -1;

// Parametres de construction de la skybox
std::vector<std::string> faces = {"skyhsky/skyhsky_lf.png", "skyhsky/skyhsky_rt.png",
                                  "skyhsky/skyhsky_up.png", "skyhsky/skyhsky_dn.png",
                                  "skyhsky/skyhsky_ft.png", "skyhsky/skyhsky_bk.png"};

float skybox_size = 20.f;
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
  head = Entity("head.obj", texturePeau, 2, 4);

  bust = Entity("bust.obj", "rock.png", 2, 5);
  bust.transform.updateScaling(vec3(0.1, 0.1, 0.1));
  bust.transform.updateTranslate(position_character);

  right_leg = Entity("right_leg.obj", texturePeau, 2, 4);
  left_leg = Entity("left_leg.obj", texturePeau, 2, 4);

  right_arm = Entity("right_arm.obj", texturePeau, 2, 4);
  left_arm = Entity("left_arm.obj", texturePeau, 2, 4);

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

  Rocks = Entity("untitled.obj", textureRoche, 2, 3);
  Wood = Entity("wood_02.obj", textureBois, 2, 4);
  Wood.transform.updateScaling(vec3(0.5, 0.5, 0.5));
  Wood.transform.updateTranslate(vec3(-1.5, 0.5, 0.));

  //Tree = Entity("50k-tree-2.OBJ", textureArbre, 2, 6);
  //Tree.transform.updateScaling(vec3(0.1, 0.1, 0.1));

  rocksBox = BBOX(Rocks.sommets, vec3(0.1, 0.1, 0.1));
  rocksBox.addChildren(Rocks);
  rocksBox.transform.updateScaling(vec3(0.7, 0.7, 0.7));
  rocksBox.transform.updateTranslate(vec3(-1.2, 0.5, 0.));

  terrain2.addChildren(rocksBox);
  terrain1.addChildren(Wood);
  terrain3.addChildren(Tree);

  float offset = terrain1.FindMaxZ();
  cout << "offset = " << offset << endl;
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

  rLegBox = BBOX(right_leg.sommets, vec3(0.1, 0.1, 0.1));
  lLegBox = BBOX(left_leg.sommets, vec3(0.1, 0.1, 0.1));

  rLegBox.addChildren(right_leg);
  lLegBox.addChildren(left_leg);

  bust.addChildren(rLegBox);
  bust.addChildren(lLegBox);

  bust.transform.updateScaling(vec3(1 / 0.1, 1 / 0.1, 1 / 0.1));
  bust.transform.updateTranslate(vec3(0.35, 0., 4.));
  bust.transform.updateScaling(vec3(0.1, 0.1, 0.1));

  glUseProgram(programScene);
  GLuint LightID = glGetUniformLocation(programScene, "LightPosition_worldspace");

  double lastTime = glfwGetTime();
  int nbFrames = 0;

  ModelMatrix  = glm::mat4(1.f);
  ViewMatrix =  glm::mat4(1.f);
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

    if(normal_mode){
        ModelMatrix = glm::rotate(ModelMatrix, rotation_speed, glm::vec3(0., 1., 0.));
        ViewMatrix = glm::lookAt(cameraPosNormal, cameraTarget, cameraUp);
        ProjectionMatrix = glm::perspective(glm::radians(fov), (float)4/(float)3, 0.1f, 100.f);
    }

    if(free_mode){
        ModelMatrix = glm::mat4(1.f);
        ViewMatrix = glm::lookAt(cameraPosLibre, cameraPosLibre + cameraFront, cameraUp);
        ProjectionMatrix = glm::perspective(glm::radians(fov), (float)4/(float)3, 0.1f, 100.f);
    }

    if(orbit_mode){
        ModelMatrix = glm::mat4(1.f);
        ViewMatrix = glm::lookAt(cameraPosOrbit, cameraOrbitTarget, cameraUp);
        ProjectionMatrix = glm::perspective(glm::radians(fov), (float)4/(float)3, 0.1f, 100.f);
    }

    Racine.updateSelfAndChild();
    Racine.drawEntity(programScene);

    if (movePlan)
    {
      terrain1.transform.updateTranslate(vec3(0, 0, ismoving));
      if (cpt == 0)
      {
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

    cout << lLegBox.getBbmin().x - infinite_plane.transform.position.x << endl;

    // Collision sur l'axe Y,Z
    if (terrain2.transform.position.z > rLegBox.getBbmin().z + bust.transform.position.z + 1 - 0.4 && !isColliding && bust.transform.position.y < rocksBox.transform.position.y + 0.2 && (/* rocksBox.getBbmin().x <= rLegBox.getBbmax().x - infinite_plane.transform.position.x <= rocksBox.getBbmax().x ||  */rocksBox.getBbmin().x <= lLegBox.getBbmin().x - infinite_plane.transform.position.x <= rocksBox.getBbmax().x))
    {
      cout << "collision" << endl;
      // isColliding = true;
      movePlan = false;
      // bust.transform.updateScaling(vec3(1 / 0.1, 1 / 0.1, 1 / 0.1));
      // bust.transform.updateTranslate(vec3(0, 0, 1));
      // bust.transform.updateScaling(vec3(0.1, 0.1, 0.1));
      // movePlan = true;
    }

    // if ()
    // {
    //     cout << "collision" << endl;
    // }

    if (collision)
    {
      vec3 velocity = speedVector + acceleration * deltaTime;
      position_during_jump = position_during_jump + speedVector * deltaTime + acceleration * deltaTime * deltaTime;
      speedVector = velocity;
      if (position_during_jump.y < terrain1.sommets[0][1] || position_during_jump.y < terrain2.sommets[0][1] || position_during_jump.y < terrain3.sommets[0][1])
      {
        isJumping = false;
        speedVector = vec3(0., 0., 0.);
        // speedVector = speedVector - 2 * dot(normale_au_terrain, speedVector) * normale_au_terrain;
        position_during_jump = vec3(position_during_jump.x, 0., position_during_jump.z);
      }
      bust.transform.updateScaling(vec3(1 / 0.1, 1 / 0.1, 1 / 0.1));
      bust.transform.updateTranslate(vec3(0, speedVector.y * deltaTime, 0));
      bust.transform.updateScaling(vec3(0.1, 0.1, 0.1));
    }

    if (wait % 10 == 0 && !isJumping && starting)
    {

      rLegBox.transform.identity();
      rLegBox.transform.updateRotationX(rightLeg_angle);
      rLegBox.transform.updateTranslate(vec3(0.1, -0.2f, 0.f));

      lLegBox.transform.identity();
      lLegBox.transform.updateRotationX(leftLeg_angle);
      lLegBox.transform.updateTranslate(vec3(-0.05, -0.2, 0.));

      leftLeg_angle += 5.0f * leftLeg_direction;
      rightLeg_angle += 5.0f * rightLeg_direction;

      if (leftLeg_angle >= 5.0f || leftLeg_angle <= -1.0f)
      {
        leftLeg_direction *= -1;
      }
      if (rightLeg_angle >= 5.0f || rightLeg_angle <= -1.0f)
      {
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

  if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
      normal_mode = true;
      free_mode = false;
      orbit_mode = false;
  }

  if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
      normal_mode = false;
      free_mode = true;
      orbit_mode = false;
  }

  if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
      normal_mode = false;
      free_mode = false;
      orbit_mode = true;
  }

  if(normal_mode){
      if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
          cameraPosNormal.y = cameraPosNormal.z;
          ViewMatrix = glm::lookAt(cameraPosNormal, cameraTarget, cameraUp);
          glUniformMatrix4fv(glGetUniformLocation(programScene, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
      }
      if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
          rotation_speed += 0.01;
      if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
          rotation_speed -= 0.01;
  }

  if(free_mode){
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
          cameraPosLibre -= cameraSpeed * cameraFront;
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
          cameraPosLibre += cameraSpeed * cameraFront;
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
          cameraPosLibre -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
          cameraPosLibre += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
          cameraPosLibre += cameraSpeed * cameraUp;
      if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
          cameraPosLibre -= cameraSpeed * cameraUp;
  }

  if(orbit_mode){
      if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
          cameraPosOrbit += glm::normalize(cameraUp) * cameraSpeed;
      if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
          cameraPosOrbit -= glm::normalize(cameraUp) * cameraSpeed;
      if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
          cameraPosOrbit += glm::normalize(glm::cross(cameraOrbitTarget - cameraPosOrbit, cameraUp))*cameraSpeed;
      if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
          cameraPosOrbit -= glm::normalize(glm::cross(cameraOrbitTarget - cameraPosOrbit, cameraUp))*cameraSpeed;
  }

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
      bust.transform.updateScaling(vec3(1 / 0.1, 1 / 0.1, 1 / 0.1));
      rLegBox.transform.identity();
      lLegBox.transform.identity();
      bust.transform.updateRotationX(-80.f);
      rLegBox.transform.updateRotationX(80.f);
      rLegBox.transform.updateTranslate(vec3(0.1, -0.2f, 0.f));
      lLegBox.transform.updateRotationX(80.f);
      lLegBox.transform.updateTranslate(vec3(-0.05, -0.2, 0.));
      isCrounched = false;
      bust.transform.updateScaling(vec3(0.1, 0.1, 0.1));
    }
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    if (!isCrounched)
    {
      bust.transform.updateScaling(vec3(1 / 0.1, 1 / 0.1, 1 / 0.1));
      rLegBox.transform.identity();
      lLegBox.transform.identity();
      bust.transform.updateRotationX(80.f);
      rLegBox.transform.updateRotationX(-80.f);
      rLegBox.transform.updateTranslate(vec3(0.1, -0.2f, 0.f));
      lLegBox.transform.updateRotationX(-80.f);
      lLegBox.transform.updateTranslate(vec3(-0.05, -0.2, 0.));
      isCrounched = true;
      bust.transform.updateScaling(vec3(0.1, 0.1, 0.1));
    }
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    infinite_plane.transform.updateTranslate(vec3(-0.02, 0, 0));
  }

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    infinite_plane.transform.updateTranslate(vec3(0.02, 0, 0));
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}
