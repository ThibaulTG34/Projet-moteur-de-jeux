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
#include <common/Shader.h>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPosNormal = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraPosLibre = glm::vec3(0.45f, 0.7f, 6.5f);
glm::vec3 cameraPosOrbit = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraOrbitTarget = glm::vec3(0., 0., 0.);

bool collision = false;
vec3 position_terrain_1 = vec3(2., 0., 0.);
vec3 position_terrain_2 = vec3(2., 0., 0.);
vec3 position_terrain_3 = vec3(2., 0., 0.);

vec3 position_bbox = vec3(0.5, 0., 4.5);

int poids_character = 2;

float v0_angle = 45.0f;
vec3 speedVector = vec3(10 * cos(v0_angle), 10 * sin(v0_angle), 0.);
vec3 gravite = vec3(0., -poids_character * 9.81, 0.);
vec3 acceleration = vec3(gravite.x / poids_character, gravite.y / poids_character,
                         gravite.z / poids_character);

vec3 normale_au_terrain;

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
float rotation_speed = 0.01f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// rotation
float angle = 0.;
float zoom = 1.;

GLuint programID;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

cube Obstacle;
// Entity character;
BBOX bbox;

// Terrain
Terrain terrain1, terrain2, terrain3, terrain4, terrain5, terrain6;
Entity plan_inf;

// Player
cube right_leg, left_leg;
Sphere head;
cube right_arm, left_arm;
cube bust;
bool isCrounched = false;
bool isJumping = false;
bool starting = false;
bool horsZone = false;
bool isColliding = false;

// Variables pour les angles de rotation des jambes
float leftLeg_angle = 0.0f;
float rightLeg_angle = 0.0f;

// Variables pour le sens de rotation des jambes
int leftLeg_direction = 1;
int rightLeg_direction = -1;

Entity left_leg_transform;
Entity right_leg_transform;

std::vector<std::string> faces1 = {"arch/left.png", "arch/right.png",
                                   "arch/up.png", "arch/down.png",
                                   "arch/front.png", "arch/back.png"};

std::vector<std::string> faces2 = {"nebuleuse/left.png", "nebuleuse/right.png",
                                   "nebuleuse/up.png", "nebuleuse/down.png",
                                   "nebuleuse/front.png", "nebuleuse/back.png"};

std::vector<std::string> faces3 = {"ciel/left.png", "ciel/right.png",
                                   "ciel/up.png", "ciel/down.png",
                                   "ciel/front.png", "ciel/back.png"};

std::vector<std::string> faces4 = {"shine/left.png", "shine/right.png",
                                   "shine/up.png", "shine/down.png",
                                   "shine/front.png", "shine/back.png"};

std::vector<std::string> faces5 = {"skyhsky/left.png", "skyhsky/right.png",
                                   "skyhsky/up.png", "skyhsky/down.png",
                                   "skyhsky/front.png", "skyhsky/back.png"};

std::vector<std::string> faces6 = {"skype/left.png", "skype/right.png",
                                   "skype/up.png", "skype/down.png",
                                   "skype/front.png", "skype/back.png"};

std::vector<std::string> faces7 = {"tron/left.png", "tron/right.png",
                                   "tron/up.png", "tron/down.png",
                                   "tron/front.png", "tron/back.png"};

std::vector<std::string> faces8 = {"interstellar/left.png", "interstellar/right.png",
                                   "interstellar/up.png", "interstellar/down.png",
                                   "interstellar/front.png", "interstellar/back.png"};
std::vector<glm::vec3> skVertices;
vector<unsigned short> indices;

vec3 position_character;
bool movePlan;

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
  // glEnable(GL_CULL_FACE);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");

  // Shader ourShader("model_loading_vs.glsl", "model_loading_fs.glsl");

  // Only load a Model
  // Model ourModel("backpack/backpack.obj");

  // Model + anim
  //  Model ourModel("vampire/dancing_vampire.dae");
  //  Animation runAnimation("vampire/dancing_vampire.dae", &ourModel);
  //  Animator animator(&runAnimation);

  float size = 30.f;

  skVertices.push_back(vec3(-size, size, size));
  skVertices.push_back(vec3(size, size, size));
  skVertices.push_back(vec3(-size, -size, size));
  skVertices.push_back(vec3(size, -size, size));
  skVertices.push_back(vec3(size, size, -size));
  skVertices.push_back(vec3(size, -size, -size));
  skVertices.push_back(vec3(-size, size, -size));
  skVertices.push_back(vec3(-size, -size, -size));

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(1);
  indices.push_back(3);
  indices.push_back(2);

  indices.push_back(1);
  indices.push_back(4);
  indices.push_back(3);
  indices.push_back(4);
  indices.push_back(5);
  indices.push_back(3);

  indices.push_back(4);
  indices.push_back(6);
  indices.push_back(5);
  indices.push_back(6);
  indices.push_back(7);
  indices.push_back(5);

  indices.push_back(6);
  indices.push_back(0);
  indices.push_back(7);
  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(7);

  indices.push_back(2);
  indices.push_back(3);
  indices.push_back(7);
  indices.push_back(3);
  indices.push_back(5);
  indices.push_back(7);

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(6);
  indices.push_back(1);
  indices.push_back(4);
  indices.push_back(6);

  EntityRoot Racine;

  std::vector<std::vector<std::string>> faces;
  faces.push_back(faces1);
  faces.push_back(faces2);
  faces.push_back(faces3);
  faces.push_back(faces4);
  faces.push_back(faces5);
  faces.push_back(faces6);
  faces.push_back(faces7);
  faces.push_back(faces8);
  skybox SK(faces, skVertices, indices, 2);
  Racine.addChildren(SK);

  string textureTerrain("mur.jpg");
  string heightmap("Heightmap_Rocky.png");
  string cubepath("cube.off");
  string textureCube("mars.jpg");

  const string file = "lava.jpeg";
  const string normalMAP = "murNormal.png";

  terrain1 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programID);
  float offset = terrain1.FindMaxZ();
  terrain2 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programID);
  terrain3 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programID);
  terrain4 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programID);
  terrain5 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programID);
  terrain6 = Terrain(16, 4, 2, textureTerrain, heightmap, 1, programID);

  Obstacle = cube(1, 0.2, 0.2, 0.2, 4);
  plan_inf = Entity();
  SK.addChildren(plan_inf);

  right_leg = cube(1, 0.05, 0.2, 0.05, 3);
  left_leg = cube(1, 0.05, 0.2, 0.05, 3);
  bust = cube(1, 0.1, 0.19, 0.1, 3);
  left_arm = cube(1, 0.05, 0.2, 0.05, 4);
  right_arm = cube(1, 0.05, 0.2, 0.05, 4);

  head = Sphere("mars.jpg", 50, 50, 0.09);

  Entity Head = Entity("character/tete.obj", "sun.jpg", 1);

  left_leg_transform = Entity();
  right_leg_transform = Entity();

  terrain1.transform.updateTranslate(position_terrain_1);

  // character = Entity("homme.obj", "lava.jpeg", 1);
  // bbox.addChildren(character);

  // bbox.transform.updateTranslate(position_bbox);
  // bbox.transform.updateScaling(vec3(0.0008, 0.0008, 0.0008));
  SK.addChildren(bbox);
  // SK.addChildren(Obstacle);

  // Racine.addChildren(Obstacle);
  // SK.addChildren(terrain2);
  // SK.addChildren(terrain3);
  // SK.addChildren(terrain4);
  // SK.addChildren(terrain5);
  // SK.addChildren(terrain6);

  plan_inf.addChildren(terrain1);
  plan_inf.addChildren(terrain2);
  plan_inf.addChildren(terrain3);
  plan_inf.addChildren(terrain4);
  plan_inf.addChildren(terrain5);
  plan_inf.addChildren(terrain6);

  SK.addChildren(Head);

  vec3 pos2 = vec3(2, 0, -offset - 2);
  vec3 pos3 = vec3(2, 0, -2 * offset - 4);
  vec3 pos4 = vec3(2, 0, -3 * offset - 6);
  vec3 pos5 = vec3(2, 0, -4 * offset - 8);
  vec3 pos6 = vec3(2, 0, -5 * offset - 10);

  terrain2.transform.updateTranslate(pos2);
  terrain3.transform.updateTranslate(pos3);
  terrain4.transform.updateTranslate(pos4);
  terrain5.transform.updateTranslate(pos5);
  terrain6.transform.updateTranslate(pos6);

  // terrain4.transform.updateTranslate(position_terrain_1);
  // terrain5.transform.updateTranslate(position_terrain_1);
  // terrain6.transform.updateTranslate(position_terrain_1);

  cout << terrain6.transform.position.z << endl;

  terrain2.addChildren(Obstacle);

  // Racine.addChildren(right_leg);
  // Racine.addChildren(left_leg);
  Racine.addChildren(bust);
  // Racine.addChildren(left_arm);
  // Racine.addChildren(right_arm);
  // Racine.addChildren(head);

  bbox = BBOX();

  // vec3 bbMin = bbox.computeBbMin(vec3(0.1, 0.1, 0.1), head.sommets);
  // vec3 bbMax = bbox.computeBbMax(vec3(0.1, 0.1, 0.1), head.sommets);
  // bbox.computeBBOXVertices(bbMin, bbMax);
  // bbox.computeBBOXIndices();

  // bbox.addChildren(head);

  bust.addChildren(head);
  bust.addChildren(left_arm);
  bust.addChildren(right_arm);
  bust.addChildren(left_leg_transform);
  bust.addChildren(right_leg_transform);
  left_leg_transform.addChildren(left_leg);
  right_leg_transform.addChildren(right_leg);

  bust.transform.updateTranslate(vec3(0.4, 0.2, 4.));

  left_leg_transform.transform.updateTranslate(vec3(-0.05, -0.2, 0.));
  right_leg_transform.transform.updateTranslate(vec3(0.1, -0.2, 0.));

  left_arm.transform.updateTranslate(vec3(-0.05, 0.2, -0.05));
  right_arm.transform.updateTranslate(vec3(0.1, 0.2, -0.05));

  head.transform.updateTranslate(vec3(0.05, 0.28, 0.05));

  left_arm.transform.updateRotationX(90.f);
  right_arm.transform.updateRotationX(90.f);

  left_leg.transform.updateTranslate(vec3(0., 0., 0.));

  Obstacle.transform.updateTranslate(vec3(-1.5, 0., 0.));

  vec3 v1 = terrain1.sommets[terrain1.indices[0]] - terrain1.sommets[terrain1.indices[1]];
  vec3 v2 = terrain1.sommets[terrain1.indices[0]] - terrain1.sommets[terrain1.indices[2]];
  normale_au_terrain = cross(v1, v2);
  normale_au_terrain = normalize(normale_au_terrain);

  glUseProgram(programID);
  GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

  double lastTime = glfwGetTime();
  int nbFrames = 0;

  ViewMatrix = glm::mat4(1.f);
  ProjectionMatrix = glm::mat4(1.f);
  movePlan = false;

  int wait = 0;
  float fall = 0.0005f;
  float move = 0.05;
  int cpt = 0;

  do
  {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    ProjectionMatrix = glm::perspective(glm::radians(fov), (float)4 / (float)3, 0.1f, 100.f);
    ViewMatrix = glm::lookAt(cameraPosLibre, cameraPosLibre + cameraFront, cameraUp);

    Racine.updateSelfAndChild();
    Racine.drawEntity(programID);

    if (movePlan)
    {

      terrain1.transform.updateTranslate(vec3(0, 0, move));
      if (cpt == 0)
        terrain2.transform.updateTranslate(vec3(0, 0, move));
      terrain3.transform.updateTranslate(vec3(0, 0, move));
      terrain4.transform.updateTranslate(vec3(0, 0, move));
      terrain5.transform.updateTranslate(vec3(0, 0, move));
      terrain6.transform.updateTranslate(vec3(0, 0, move));
      // terrain2.InfinitePlane(move);
      // terrain3.InfinitePlane(move);
      // terrain4.InfinitePlane(move);
      // terrain5.InfinitePlane(move);
      // terrain6.InfinitePlane(move);
      // cout << terrain1.transform.position.z << endl;
    }

    float max1 = terrain1.FindMaxX();
    float min1 = terrain1.FindMinX();
    // float maX3 = terrain3.FindMinZ();
    // float maX4 = terrain4.FindMinZ();
    // float maX5 = terrain5.FindMinZ();
    // float maX6 = terrain6.FindMinZ();

    if (terrain1.transform.position.z - 2.5 > bust.transform.position.z)
    {
      // terrain1.InfinitePlane(maX6);
      // terrain1.transform.identity();
      terrain1.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain2.transform.position.z - 2.5 > bust.transform.position.z)
    {
      // terrain2.InfinitePlane(4.8 * offset);
      // terrain2.transform.identity();
      terrain2.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain3.transform.position.z - 2.5 > bust.transform.position.z)
    {
      // terrain3.InfinitePlane(4.8 * offset);
      // terrain3.transform.identity();
      terrain3.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain4.transform.position.z - 2.5 > bust.transform.position.z)
    {
      // terrain4.InfinitePlane(4.8 * offset);
      // terrain4.transform.identity();
      terrain4.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain5.transform.position.z - 2.5 > bust.transform.position.z)
    {
      // terrain5.InfinitePlane(4.8 * offset);
      // terrain5.transform.identity();
      terrain5.transform.updateTranslate(vec3(0, 0, pos6.z));
    }
    if (terrain6.transform.position.z - 2.5 > bust.transform.position.z)
    {
      // terrain6.InfinitePlane(4.8 * offset);
      // terrain6.transform.identity();
      terrain6.transform.updateTranslate(vec3(0, 0, pos6.z));
    }

    if (collision)
    {
      vec3 velocity = speedVector + acceleration * deltaTime;
      position_bbox = position_bbox + speedVector * deltaTime + acceleration * deltaTime * deltaTime;
      speedVector = velocity;
      if (position_bbox.y - 0.2 < terrain1.sommets[0][1] || position_bbox.y < terrain2.sommets[0][1] || position_bbox.y < terrain3.sommets[0][1])
      {
        isJumping = false;
        speedVector = vec3(0., 0., 0.);
        // speedVector = speedVector - 2 * dot(normale_au_terrain, speedVector) * normale_au_terrain;
        position_bbox = vec3(position_bbox.x, 0., position_bbox.z);
      }
      bust.transform.updateTranslate(vec3(0, speedVector.y * deltaTime, 0));
    }
    // cout << plan_inf.transform.position.x << endl;

    // if (bust.transform.position.x < plan_inf.transform.position.x /*  || bust.transform.position.x < terrain1.transform.position.x */)
    // {
    //   horsZone = true;
    //   bust.transform.updateRotationZ(fall);
    //   bust.transform.updateTranslate(vec3(-speedVector.x * deltaTime, -speedVector.y * deltaTime * fall*100, 0));
    // }
    // else
    // {
    //   horsZone = false;
    // }

    // cout << Obstacle.transform.position.z << endl;
    // vec3 new_posLeg = vec3(right_leg.transform.position.x, right_leg.transform.position.y, right_leg.transform.position.z + move);
    // if (right_leg.Collision(terrain2.transform.position, vec3(1, 1, 1), right_leg.transform.position, vec3(0.05, 0.28, 0.05)))
    // {
    //   cout << "COLLISION" << endl;
    // }

    cout << right_leg.FindMaxPoint().x << endl;
    if (terrain2.transform.position.z > right_leg.FindMinPoint().z + bust.transform.position.z - 0.2 && !isColliding)
    {
      isColliding = true;
      movePlan = false;
      bust.transform.updateTranslate(vec3(0, 0, 1));
      movePlan = true;
    }
    // else
    // {
    // isColliding = false;
    // movePlan = true;
    // }

    if (wait % 10 == 0 && !isJumping && starting)
    {
      right_leg_transform.transform.identity();
      right_leg_transform.transform.updateRotationX(rightLeg_angle);
      right_leg_transform.transform.updateTranslate(vec3(0.1, -0.2f, 0.f));

      left_leg_transform.transform.identity();
      left_leg_transform.transform.updateRotationX(leftLeg_angle);
      left_leg_transform.transform.updateTranslate(vec3(-0.05, -0.2, 0.));

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

    glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

    wait++;

    // ourShader.use();

    /*For model loading
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)4 / (float)3, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPosLibre, cameraPosLibre + cameraFront, cameraUp);
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    ourShader.setMat4("model", model);
    ourModel.Draw(ourShader);*/

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

  terrain1.clearBuffers();
  terrain2.clearBuffers();
  terrain3.clearBuffers();

  glDeleteProgram(programID);
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
    // if (!Cube.Collision(Cube.transform.position, vec3(0.2, 0.2, 0.2), Obstacle.transform.position, vec3(0.2, 0.2, 0.2)))
    // {
    // Cube.transform.updateTranslate(vec3(0, 0, 0.01));
    // }
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    // if (!Cube.Collision(Cube.transform.position, vec3(0.2, 0.2, 0.2), Obstacle.transform.position, vec3(0.2, 0.2, 0.2)))
    // {
    //   Cube.transform.updateTranslate(vec3(0, 0, -0.01));
    // }
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

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    isJumping = true;
    collision = true;
    speedVector = vec3(0., 5 * sin(v0_angle), -5 * cos(v0_angle));
    position_bbox = bust.transform.position;
  }

  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
  {
    starting = true;
    movePlan = true;
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

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    if (!horsZone)
      plan_inf.transform.updateTranslate(vec3(-0.01, 0, 0));
  }

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    if (!horsZone)
      plan_inf.transform.updateTranslate(vec3(0.01, 0, 0));
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}
