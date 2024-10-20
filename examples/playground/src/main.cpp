
#include <ctime>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

// #include "Core/Scene/UI/uiscene.h"
#include "Core/common.h"
#include "Window/Mouse/mouse.h"
#include "Window/window.h"

#include "Physics/Physics3D/CollisionObject/Rigidbody/rigidbody.h"
#include "Physics/Physics3D/CollisionObject/Vehicle/raycastvehicle.h"

#ifndef ASSETS_ROOT_DIR
#define ASSETS_ROOT_DIR "/"
#endif

std::string path_obj_mountain = "/home/vincent/Development/Cpp/projectdraw/"
                                "examples/playground/assets/basic_mountain.obj";

GLfloat deltaTime = 0.0f, mouseX = 0, mouseY = 0, mousePitch = 0, mouseYaw = 0,
        mouseRoll = 0;

bool initialCameraMove = true;
PerspectiveCamera mainCamera;
OrthographicCamera testCamera;
OrthographicCamera overlayCamera;

Mesh *carTrace = nullptr;

btRaycastVehicle *vehicle = nullptr;
RaycastVehicle *testVehicle = nullptr;

Scene *mainScenePtr;

int sceneCamera = 0;

/*
void key_callback(sf::Window *window, int key, int scancode, int action, int
mode); void mouse_callback(sf::Window *window, double xpos, double ypos); void
mouse_scroll_callback(sf::Window *window, double xoffset, double yoffset);*/
void handle_key(Window &window);

bool emergencyBrake = false;

glm::vec3 toVec3(btVector3 vec) {
  return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
}

int main() {

  WindowInfo wInfo;
  wInfo.maximized = false;
  wInfo.cursorLeave = false;
  Window window(wInfo, 800, 600, "Test123");

  Size windowSize = window.getSize();

  initCore();

  DiscreteWorld physicsWorld;

  std::vector<std::string> skybox_faces;
  // create camera
  mainCamera.translate(glm::vec3(5.0f, 15.0f, 5.0f));
  mainCamera.setClipping(mainCamera.getClippingNear(), 500.0f);
  mainCamera.setSize(windowSize.width, windowSize.height);

  PerspectiveCamera carCamera;
  carCamera.setClipping(carCamera.getClippingNear(), 100.0f);
  carCamera.setSize(windowSize.width, windowSize.height);

  testCamera = OrthographicCamera(glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f),
                                  glm::vec3(0.0f, -1.0f, 0.0f));
  testCamera.translate(glm::vec3(1.0f, 20.0f, 1.0f));
  testCamera.setForward(glm::vec3(1.0f, 0.0f, 1.0f));
  testCamera.setClipping(0.1f, 65.0f);
  testCamera.setSize(200, 200 * (windowSize.height / windowSize.width));

  overlayCamera = OrthographicCamera(glm::vec3(0.0f, 0.0f, -10.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f),
                                     glm::vec3(0.0f, 0.0f, -1.0f));
  overlayCamera.setClipping(0.0f, 10.0f);
  overlayCamera.setSize(200.0f, 200 * (windowSize.height / windowSize.width));

  /*
  for (glm::vec3 point : localViewFrustum.nearCorners) {
          geoLight.addPoint(point);
  }*/

  //
  // Light
  //
  DirectionalLight dLight = DirectionalLight();
  // dLight.direction = glm::vec3(-2.0f, -2.0f, -2.0f);
  dLight.ambient = glm::vec3(0.3f, 0.3f, 0.3f);
  dLight.diffuse = glm::vec3(1.0f, 0.9f, 0.7f);
  dLight.specular = dLight.diffuse;
  dLight.intensity = .3f;
  dLight.changeDirection(glm::vec3(-1.0f, -0.4f, -1.0f));

  glm::vec3 light_color(0.0f, 0.0f, 1.0f);
  glm::vec3 light_spec(0.0f, 0.0f, 1.0f);

  PointLight pLightLeft;
  pLightLeft.intensity = 1.0f;
  pLightLeft.translate(glm::vec3(4.0f, 4.0f, 0.0f));
  pLightLeft.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
  pLightLeft.diffuse = light_color;
  pLightLeft.specular = light_spec;
  pLightLeft.setDistance(200.0f);

  PointLight pLightRight;
  pLightRight.intensity = 1.0f;
  pLightRight.translate(glm::vec3(4.0f, 4.0f, 0.0f));
  pLightRight.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
  pLightRight.diffuse = light_color;
  pLightRight.specular = light_spec;
  pLightRight.setDistance(200.0f);

  light_color = glm::vec3(1.0f, 1.0f, 1.0f);
  light_spec = glm::vec3(1.0f, 1.0f, 1.0f);
  PointLight pLight2;
  pLight2.intensity = 1.f;
  pLight2.translate(glm::vec3(0.0f, 0.0f, 0.0f));
  pLight2.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
  pLight2.diffuse = light_color;
  pLight2.specular = light_spec;
  pLight2.setDistance(300.0f);

  SpotLight frontLight;
  frontLight.intensity = 0.6f;
  frontLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
  frontLight.setDistance(200.0f);
  frontLight.setCutOff(12.0, 20.0);

  Mesh test_obj(&path_obj_mountain[0]);

  //
  // TEXT FreeType
  //
  Text text_fps(ResourceManager::GetPath("/Fonts/VeraMono.ttf").c_str(), 60);
  // Shaders[SHADER_FONT].use();
  text_fps.scale(0.01f, 0.01f, 0.01f);
  // glUniformMatrix4fv(glGetUniformLocation(Shaders[SHADER_FONT].getId(),
  // "model"), 1, GL_FALSE, glm::value_ptr(text_fps.getModelMatrix()));

  //
  //
  //

  Scene scene_main(windowSize.width, windowSize.height);
  mainScenePtr = &scene_main;

  OrthographicCamera camera(glm::vec3(0.0f, 0.0f, 1.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f));
  camera.setClipping(-2.0f, 200.0f);
  camera.setSize(100, (windowSize.height / windowSize.width) * 100.0f);
  camera.translate(glm::vec3(0.0f, 0.0f, -100.0f));

  // Size camSize{-1.0f, -1.0f, 0.0f, 2.0f, 2.0f, 0.0f};
  scene_main.addObject(mainCamera);
  // camSize = {0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f};
  scene_main.addObject(testCamera);
  // camSize = {-1.0f, -1.0f, 0.0f, 2.0f, 2.0f, 0.0f};
  scene_main.addObject(camera);
  // scene_main.enableCamera(camera, false);

  // SceneCameraConfig testCamConfig = scene_main.getCameraConfig(testCamera);
  // testCamConfig.dLightVisible = false;
  // testCamConfig.pLightVisible = false;
  // testCamConfig.sLightVisible = false;
  // testCamConfig.ParticleVisible = false;
  // scene_main.configureCamera(testCamera, testCamConfig);

  scene_main.addObject(dLight);
  // dLight.change_direction(glm::vec3(-1.0f, 0.0f, 0.0f));
  dLight.shadow = true;

  scene_main.addObject(pLightLeft);
  scene_main.addObject(pLightRight);

  scene_main.addObject(pLight2);
  pLightLeft.translate(glm::vec3(2.0f, 1.0f, 0.0f));
  pLight2.translate(glm::vec3(5.0f, 1.0f, 0.0f));

  scene_main.addObject(frontLight);

  scene_main.addObject(test_obj);

  Size size_small;
  size_small.height = 0.1f;
  size_small.width = 0.1f;
  size_small.depth = 0.1f;

  Size size_medium;
  size_medium.height = 1.0f;
  size_medium.width = 1.0f;
  size_medium.depth = 1.0f;

  Size size_big;
  size_big.height = 5.0f;
  size_big.width = 5.0f;
  size_big.depth = 5.0f;

  test_obj.scaleToHeight(50.0f);
  Size size = test_obj.getSize();
  test_obj.translate(glm::vec3(-250.0, -4.0f, -250.0f));
  // test_obj.setCenter(glm::vec3(0.5f, 0.5f, 1.7f));
  test_obj.settings.boxVisible = false;
  test_obj.settings.drawType = DrawType::TRIANGLEG;

  pLightLeft.addChild(new Mesh(pd::quad(0.5f, 0.5f, 0.5f, glm::vec4(0.8f))));
  pLightRight.addChild(new Mesh(pd::quad(0.5f, 0.5f, 0.5f, glm::vec4(0.8f))));
  pLight2.addChild(new Mesh(pd::quad(0.5f, 0.5f, 0.5f, glm::vec4(0.8f))));

  // cube.scaleToSize(size_medium);
  // cube.translate(glm::vec3(1.0f, 5.0f, 0.0f));
  // cube.visibleNormal = true;

  std::vector<float> heightData = {
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 3.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f,
      0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
  };

  Mesh testHeightField(pd::heightfieldStep(10, 10, heightData,
                                           glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)));
  testHeightField.translate(glm::vec3(20.0f, 0.1f, 20.0f));
  testHeightField.settings.normalVisible = true;

  // {
  //   Model *thfPtr = testHeightField.getModelPtr();
  //   BasicMesh mesh = thfPtr->getMeshes()[0];

  //   std::vector<Vertex> vertices = mesh.getVertices();
  //   for (Vertex &v : vertices) {
  //     v.Color = v.Position.y > 0.2f ? glm::vec4(0.5f, 0.1f, 0.1f, 1.0f)
  //                                   : glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
  //   }
  //   mesh.setVertices(vertices);
  //   thfPtr->setMesh(0, mesh);
  // }

  scene_main.addObject(testHeightField);

  // Instancing

  Mesh instancerModel =
      pd::quad(0.5f, 0.5f, 0.5f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  // Instancer instancer(instancerModel, 100);

  // for (int i = 0; i < instancer.getModelMatrices().size(); i++) {

  //   glm::vec3 position;
  //   position.x +=
  //       (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 20.0f;
  //   position.y +=
  //       (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 20.0f;
  //   position.z +=
  //       (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 200.0f;

  //   glm::mat4 mmatrix = glm::translate(glm::mat4(1.0f), position);
  //   instancer.setModelMatrix(mmatrix, i);
  // }

  // scene_main.addInstancer(instancer);

  Mesh test_rect =
      pd::cone(1.0f, 1.0f, 10.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  test_rect.translate(glm::vec3(0.0f, 5.0f, -2.0f));

  Mesh cone = pd::cone(1.0f, 1.0f, 8.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  cone.translate(glm::vec3(10.0f, 0.0f, 10.0f));
  scene_main.addObject(cone);

  test_rect.settings.drawType = DrawType::TRIANGLEG;
  scene_main.addObject(test_rect);

  // TODO: Fix geometry
  carTrace = new Mesh();
  scene_main.addObject(*carTrace);

  Text text_description =
      Text(ResourceManager::GetPath("/Fonts/VeraMono.ttf").c_str(), 60);
  text_description.translate(glm::vec3(0.0f, 0.1f, -10.0f));
  text_description.rotate(Rotator(glm::radians(-30.0f), glm::vec3(0, 1, 0)));
  text_description.settings.useCustomColor = true;
  text_description.settings.customColor = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
  text_description.setText("This is a great test level!");
  scene_main.addObject(text_description);

  std::vector<Drawable *> cubes;

  glm::vec3 cubes_position(0.0f, 0.5f, 15.0f);

  srand(static_cast<unsigned>(time(0)));
  for (size_t x = 0; x < 3; x++) {
    for (size_t y = 0; y < 3; y++) {
      for (size_t z = 0; z < 3; z++) {
        Mesh *cube;
        glm::vec4 cubeColor(
            static_cast<float>(rand() / static_cast<float>(RAND_MAX)),
            static_cast<float>(rand() / static_cast<float>(RAND_MAX)),
            static_cast<float>(rand() / static_cast<float>(RAND_MAX)), 1.0f);
        cube = new Mesh(pd::quad(1.f, 1.0f, 1.0f, cubeColor));
        cube->translate(cubes_position +
                        glm::vec3(x * 1.0f, y * 1.0f, z * 1.0f));
        cubes.push_back(cube);
        scene_main.addObject(*cubes[cubes.size() - 1]);
      }
    }
  }

  Mesh borderGround =
      pd::quad(500.0f, 5.0f, 500.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

  Size sizeGround = borderGround.getSize();
  GLfloat heightScale = 2.0f;

  Mesh borderAnchor, borderBack, borderFront, borderLeft, borderRight;
  borderBack =
      pd::quad(sizeGround.width + 6.0f, sizeGround.height * heightScale, 5.0f,
               glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
  borderFront =
      pd::quad(sizeGround.width + 6.0f, sizeGround.height * heightScale, 5.0f,
               glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
  borderLeft = pd::quad(5.0f, sizeGround.height * heightScale, sizeGround.depth,
                        glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
  borderRight = pd::quad(5.0f, sizeGround.height * heightScale,
                         sizeGround.depth, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

  scene_main.addObject(borderGround);
  scene_main.addObject(borderBack);
  scene_main.addObject(borderFront);
  scene_main.addObject(borderLeft);
  scene_main.addObject(borderRight);

  //
  // Vehicle
  //
  GLfloat carMass = 1000.0f;
  GLfloat carWheelThickness = 0.2f;

  Mesh carChassis =
      pd::quad(4.0f, 2.0f, 7.0f, glm::vec4(0.1f, 0.3f, 0.8f, 0.7f));
  carChassis.settings.xrayVisible = false;
  carChassis.settings.xrayColor = glm::vec4(1.0f, 0.1f, 0.0f, 1.0f);
  carChassis.settings.outlineVisible = true;
  carChassis.settings.outlineThickness = 0.1f;

  std::vector<Mesh *> carWheels;
  for (int i = 0; i < 4; i++) {
    Mesh *wheel = new Mesh(pd::quad(carWheelThickness, 0.5, 0.5,
                                    glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)));
    carWheels.push_back(wheel);
  }

  {
    glm::vec3 carCenter = carChassis.getPosition();
    Size carSize = carChassis.getSize();

    carWheels[0]->translate(glm::vec3(
        carCenter.x + (carSize.width * 0.5f + carWheelThickness * 0.5f),
        carCenter.y - carSize.height * 0.5f,
        carCenter.z + carSize.depth * 0.3f));
    carWheels[1]->translate(glm::vec3(
        carCenter.x - (carSize.width * 0.5f + carWheelThickness * 0.5f),
        carCenter.y - carSize.height * 0.5f,
        carCenter.z + carSize.depth * 0.3f));
    carWheels[2]->translate(glm::vec3(
        carCenter.x + (carSize.width * 0.5f + carWheelThickness * 0.5f),
        carCenter.y - carSize.height * 0.5f,
        carCenter.z - carSize.depth * 0.3f));
    carWheels[3]->translate(glm::vec3(
        carCenter.x - (carSize.width * 0.5f + carWheelThickness * 0.5f),
        carCenter.y - carSize.height * 0.5f,
        carCenter.z - carSize.depth * 0.3f));
  }

  pLightLeft.translate(carChassis.getPosition() +
                       glm::vec3(-carChassis.getSize().width * 0.5f + 0.25,
                                 carChassis.getSize().height * 0.5f,
                                 -carChassis.getSize().depth * 0.3f));
  // pLightLeft.setCenterInWorld(carChassis.getPosition());

  pLightRight.translate(carChassis.getPosition() +
                        glm::vec3(carChassis.getSize().width * 0.5f - 0.75f,
                                  carChassis.getSize().height * 0.5f,
                                  -carChassis.getSize().depth * 0.3f));
  // pLightRight.setCenterInWorld(carChassis.getPosition());

  Mesh carDistanceLine;
  {
    Size carSize = carChassis.getSize();
    glm::vec3 carFront(0.0f, 0.0f, -carSize.depth * 0.5f);
    carDistanceLine.settings.useCustomColor = true;
    carDistanceLine.settings.customColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    carDistanceLine.line(carFront, carFront + glm::vec3(0.0f, 0.0f, -20.0f));

    carDistanceLine.settings.drawType = DrawType::LINEG;
  }

  Mesh carSelectedCircle;
  {
    Size carSize = carChassis.getSize();
    carSelectedCircle = pd::circle(carSize.depth * 0.6f, 30.0f,
                                   glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
    carSelectedCircle.settings.drawType = DrawType::LINEG;
    carSelectedCircle.settings.lineThickness = 5.0f;
    carSelectedCircle.rotate(Rotator(glm::radians(90.0f), glm::vec3(1, 0, 0)));
  }

  scene_main.addObject(carDistanceLine);
  scene_main.addObject(carSelectedCircle);

  scene_main.addObject(carChassis);
  for (auto wheel : carWheels) {
    scene_main.addObject(*wheel);
  }

  carTrace->settings.drawType = DrawType::LINEG;
  carTrace->settings.lineThickness = 3;

  // cube.translate(glm::vec3(0.0f, 3.0f, 0.0f));

  std::vector<RigidBody *> rigidBodys;

  borderGround.settings.boxVisible = false;

  // calc positions in compound
  borderBack.translate(borderGround.getPosition() -
                       glm::vec3(1.0f, 0.0f, 1.0f));
  // borderBack.setCenterInWorld(borderGround.getPositionCenter());
  borderFront.translate(borderGround.getPosition() -
                        glm::vec3(1.0f, 0.0f, -sizeGround.depth));
  // borderFront.setCenterInWorld(borderGround.getPositionCenter());
  borderLeft.translate(borderGround.getPosition() -
                       glm::vec3(1.0f, 0.0f, 0.0f));
  // borderLeft.setCenterInWorld(borderGround.getPositionCenter());
  borderRight.translate(borderGround.getPosition() +
                        glm::vec3(sizeGround.width, 0.0f, 0.0f));
  // borderRight.setCenterInWorld(borderGround.getPositionCenter());

  // cube.translateCenter(glm::vec3(0.0f, 10.0f, 10.0f));
  // cube.rotate(glm::radians(glm::vec3(40.0f, 40.0f, 40.0
  // add borderGround as borderGround plane
  collision::CollisionShape borderGroundShape(
      collision::generateCube(borderGround.getSize()));
  collision::CollisionShape borderFrontShape(
      collision::generateCube(borderFront.getSize()));
  collision::CollisionShape borderBackShape(
      collision::generateCube(borderBack.getSize()));
  collision::CollisionShape borderLeftShape(
      collision::generateCube(borderLeft.getSize()));
  collision::CollisionShape borderRightShape(
      collision::generateCube(borderRight.getSize()));
  {
    btCompoundShape *borderCompound = new btCompoundShape();
    btTransform transform;
    transform.setIdentity();

    glm::vec3 posCenter =
        glm::vec3(0.0f, 0.0f, 0.0f); // borderGround.getPositionCenter();
    transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
    borderCompound->addChildShape(transform, borderGroundShape.getShape());

    posCenter = borderBack.getPosition();
    transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
    borderCompound->addChildShape(transform, borderFrontShape.getShape());

    posCenter = borderFront.getPosition();
    transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
    borderCompound->addChildShape(transform, borderBackShape.getShape());

    posCenter = borderLeft.getPosition();
    transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
    borderCompound->addChildShape(transform, borderLeftShape.getShape());

    posCenter = borderRight.getPosition();
    transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
    borderCompound->addChildShape(transform, borderRightShape.getShape());

    collision::CollisionShape borderCompoundShape(borderCompound);

    RigidBody rbody(borderCompoundShape, 0.0f, RigidType::STATIC);
    rbody.getCollisionObjectHandle()->setFriction(btScalar(1.0f));
    rbody.linkDrawable(borderAnchor);
    // rbody.getBody()->setCollisionFlags(rbody.getBody()->getCollisionFlags() |
    // btRigidBody::CF_KINEMATIC_OBJECT);
    // rbody.getBody()->setActivationState(DISABLE_DEACTIVATION);
    rbody.visibleAABB = false;
    rigidBodys.push_back(new RigidBody(rbody));

    borderAnchor.translate(glm::vec3(0.0f, -sizeGround.height * 0.5f, 0.0f));
    rbody.refreshBody();

    physicsWorld.addCollisionObject(*rigidBodys[rigidBodys.size() - 1], 1, 3);
  }

  // set cube as sceond item
  {

    for (auto cube : cubes) {
      cube->scaleToHeight(1.0f);
      collision::CollisionShape cube_shape(
          collision::generateCube(cube->getSize()));
      RigidBody rbodyCube(cube_shape, 80.0f, RigidType::DYNAMIC);
      rbodyCube.linkDrawable(*cube);
      rbodyCube.visibleAABB = false;
      rigidBodys.push_back(new RigidBody(rbodyCube));
      physicsWorld.addCollisionObject(*rigidBodys[rigidBodys.size() - 1], 2, 3);
    }
  }

  cubes[cubes.size() - 1]->scaleToHeight(0.5f);
  rigidBodys[rigidBodys.size() - 1]->refreshBody();

  {
    collision::CollisionShape text_shape(
        collision::generateSphere(test_rect.getSize().width * 0.5));
    RigidBody rbody(text_shape, 100.0f, RigidType::STATIC);
    rbody.linkDrawable(test_rect);
    rigidBodys.push_back(new RigidBody(rbody));
    physicsWorld.addCollisionObject(*rigidBodys[rigidBodys.size() - 1], 2, 3);
  }

  // set vehicle physics
  Size carSize = carChassis.getSize();
  carSize.height = 0.1f;
  collision::CollisionShape carChassisShape =
      collision::CollisionShape(collision::generateCube(carSize));

  {
    btTransform transform;
    transform.setIdentity();

    // btCompoundShape *compoundShape = new btCompoundShape();
    // transform.setOrigin(toBtVec3(carAnchor.getPosition() + glm::vec3(0.0f,
    // 0.5f * carChassis.getSize().height, 0.0f)));
    // compoundShape->addChildShape(transform, carChassisShape.getShape());
    // collision::CollisionShape carHullShape(compoundShape);

    testVehicle = new RaycastVehicle(carChassisShape, carMass, physicsWorld);

    btScalar connectionHeight(0.5f);
    btVector3 wheelConnectionPoint(carChassis.getSize().width * 0.6,
                                   connectionHeight,
                                   carChassis.getSize().depth * 0.3);
    btVector3 wheelConnectionPoints[] = {
        btVector3(1, 1, 1), btVector3(-1, 1, 1), btVector3(1, 1, -1),
        btVector3(-1, 1, -1)};

    for (int i = 0; i < carWheels.size(); i++) {
      RaycastVehicleWheel wheel;
      wheel.drawable = carWheels[i];
      wheel.canSteer = i > 1;
      wheel.contactPoint =
          toVec3(wheelConnectionPoint * wheelConnectionPoints[i]);
      testVehicle->addWheel(wheel);
    }

    physicsWorld.addCollisionObject(*testVehicle, 0, 0);
    vehicle = testVehicle->getVehicleObjectHandle();

    carChassis.translate(
        glm::vec3(0.0f, carChassis.getSize().height * 1.0f, 0.0f));
    testVehicle->refreshBody();

    physicsWorld.addCollisionObject(*rigidBodys[rigidBodys.size() - 1], 2, 3);
  }

  test_obj.scaleToHeight(100.0f);
  collision::CollisionShape terrainShape =
      collision::CollisionShape(collision::generateTriangleMesh(test_obj));
  {
    RigidBody rbody(terrainShape, 0.0f, RigidType::STATIC);
    rbody.linkDrawable(test_obj);
    rbody.visibleAABB = true;
    rigidBodys.push_back(new RigidBody(rbody));
    physicsWorld.addCollisionObject(*rigidBodys[rigidBodys.size() - 1], 1, 14);
  }

  testHeightField.scaleToHeight(40.0f);

  collision::CollisionShape heightShape = collision::CollisionShape(
      collision::generateTriangleMesh(testHeightField));
  {
    RigidBody rbody(heightShape, 0.0f, RigidType::STATIC);
    rbody.linkDrawable(testHeightField);
    rbody.visibleAABB = true;
    rigidBodys.push_back(new RigidBody(rbody));

    testHeightField.translate(glm::vec3(40.0f, 0.07f, 40.0f));
    rbody.refreshBody();
    physicsWorld.addCollisionObject(*rigidBodys[rigidBodys.size() - 1], 1, 14);
  }

  Scene sceneOverlay(windowSize.width, windowSize.height);
  sceneOverlay.addObject(overlayCamera);
  overlayCamera.translate(glm::vec3(50.0f, 50.0f, 0.0f));

  // {
  //   SceneCameraConfig config = sceneOverlay.getCameraConfig(overlayCamera);
  //   config.dLightVisible = false;
  //   config.pLightVisible = false;
  //   config.sLightVisible = false;
  //   sceneOverlay.configureCamera(overlayCamera, config);
  // }

  Mesh overlayRect =
      pd::rectangle(50.0f, 15.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  overlayRect.translate(glm::vec3(-25.0f, 120.0f, -5.0f));

  sceneOverlay.addObject(overlayRect);
  sceneOverlay.addObject(text_fps);
  text_fps.translate(glm::vec3(-40.0f, 115.0f, 0.0f));
  text_fps.settings.useCustomColor = true;
  text_fps.settings.customColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  text_fps.scale(0.2f, 0.2f, 0.3f);

  /*
  for (auto body : rigidBodys) {
          scene_main.addRigidBody((*body));
  }
  */

  Mesh testCubeD(pd::quad(5.0f, 5.0f, 5.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
  Mesh testCubeK(pd::quad(5.0f, 5.0f, 5.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  Mesh testCubeS(pd::quad(5.0f, 5.0f, 5.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
  testCubeS.scale(2.0f, 2.0f, 2.0f);

  {
    RigidBody rbody = RigidBody(collision::generateCube(testCubeD.getSize()),
                                1000.0f, RigidType::DYNAMIC);
    rbody.linkDrawable(testCubeD);
    rigidBodys.push_back(new RigidBody(rbody));
    physicsWorld.addCollisionObject(*rigidBodys[rigidBodys.size() - 1], 1, 7);
  }

  {
    RigidBody rbody = RigidBody(collision::generateCube(testCubeD.getSize()),
                                0.0f, RigidType::DYNAMIC);
    rbody.linkDrawable(testCubeK);
    rigidBodys.push_back(new RigidBody(rbody));
    physicsWorld.addCollisionObject(*rigidBodys[rigidBodys.size() - 1], 1, 7);
  }

  {
    RigidBody rbody = RigidBody(collision::generateCube(testCubeD.getSize()),
                                0.0f, RigidType::DYNAMIC);
    // rbody.linkDrawable(testCubeS);
    rigidBodys.push_back(new RigidBody(rbody));
    physicsWorld.addCollisionObject(*rigidBodys[rigidBodys.size() - 1], 1, 7);
  }

  scene_main.addObject(testCubeD);
  scene_main.addObject(testCubeK);
  scene_main.addObject(testCubeS);

  Mesh particle(pd::quad(1.0f, 1.0f, 1.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
  // PointGenerator3D pgTest(particle, 10);
  // scene_main.addObject(pgTest);
  // scene_main.addAnimatable(pgTest);

  // window.getWindow()->setFramerateLimit(60);

  sf::Clock clock;
  GLfloat lastTime = 0.00f;
  deltaTime = 0.0f;
  float fpsDelay = 1.0f, fpsCounter = fpsDelay;

  while (window.getWindow()->isOpen()) {
    clearScreen(glm::vec4(dLight.diffuse, 1.0f));

    // if (sceneCamera == 0) {
    //   scene_main.enableCamera(mainCamera, true);
    //   scene_main.enableCamera(testCamera, false);
    // } else {
    //   scene_main.enableCamera(mainCamera, true);
    //   scene_main.enableCamera(testCamera, true);
    // }

    lastTime = clock.getElapsedTime().asMilliseconds();
    window.update();

    handle_key(window);

    // borderGround.translateCenter(glm::vec3(0.0f, 0.0f, 0.0f));
    /*if(lastTime > 0.0f) {
            borderAnchor.rotate(glm::radians(glm::vec3(cos((lastTime) * 1.5f) *
    0.0f, lastTime * 00.0f, cos((lastTime) * 1.0f) * 0.0f)));
            rigidBodys[0]->syncBody();
    }*/

    physicsWorld.update(deltaTime);
    // sceneOverlay.updatePhysics(deltaTime);

    // synchronize walls to ground

    borderGround.translate(borderAnchor.getPosition());
    borderBack.translate(borderGround.getPosition() -
                         glm::vec3(1.0f, 0.0f, 1.0f));
    borderFront.translate(borderGround.getPosition() -
                          glm::vec3(1.0f, 0.0f, -sizeGround.depth));
    borderLeft.translate(borderGround.getPosition() -
                         glm::vec3(1.0f, 0.0f, 0.0f));
    borderRight.translate(borderGround.getPosition() +
                          glm::vec3(sizeGround.width, 0.0f, 0.0f));

    borderGround.rotate(borderAnchor.getRotator());
    borderBack.rotate(borderGround.getRotator());
    borderFront.rotate(borderGround.getRotator());
    borderLeft.rotate(borderGround.getRotator());
    borderRight.rotate(borderGround.getRotator());

    Point mousePos = window.getCursorPosition();
    // printf("%f,%f \n", mousePos.position.x, mousePos.position.y);

    text_description.scale(glm::vec3(1.0f, 1.0f, 0.01f));
    text_description.scaleToHeight(1.0f);

    glm::vec3 camPos = mainCamera.getPosition();
    std::pair<Point, Point> mouseRay =
        getCameraMousePosition(window, mainCamera);
    btVector3 rayStart = toBtVec3(camPos);
    btVector3 rayEnd = toBtVec3(camPos + mouseRay.second.position * 2.0f *
                                             mainCamera.getClippingFar());
    btCollisionWorld::ClosestRayResultCallback RayCallback(rayStart, rayEnd);
    // RayCallback.m_collisionFilterMask = 3;
    RayCallback.m_collisionFilterGroup = 2;
    auto raycastResult = physicsWorld.rayCast(
        glm::vec3(rayStart.x(), rayStart.y(), rayStart.z()),
        glm::vec3(rayEnd.x(), rayEnd.y(), rayEnd.z()));
    // if (RayCallback.hasHit()) {
    //   RigidBody *rBody = nullptr;
    //   for (auto body : rigidBodys) {
    //     if (body->getBody() == RayCallback.m_collisionObject) {
    //       rBody = body;
    //       break;
    //     }
    //   }
    //   rBody->getBody()->applyCentralImpulse(btVector3(0.0f, 500.0f, 0.0f));

    //   rayEnd = RayCallback.m_hitPointWorld;
    //   btVector3 normal = RayCallback.m_hitNormalWorld;
    //   pLight2.translate(toVec3(rayEnd) + glm::vec3(0.0f, 1.0f, 0.0f));
    // }

    // pgTest.tra(carChassis.getPositionCenter() -
    //                    testVehicle->getFront() * 4.0f);

    // pLight2.intensity = (sin(glfwGetTime()) * 0.5 + 0.5);
    // scene_main.draw(deltaTime);

    // update vehicle lights
    {
      btTransform transform;
      transform = vehicle->getChassisWorldTransform();
      glm::vec3 carPosition = toVec3(transform.getOrigin());

      frontLight.translate(carChassis.getPosition() +
                           glm::vec3(0.0f, 2.0f, 0.0f));
      frontLight.setDirection(testVehicle->getForward() +
                              glm::vec3(0.0f, -0.05f, 0.0f));

      pLightLeft.translate(carChassis.getPosition() +
                           glm::vec3(-carChassis.getSize().width * 0.5f + 0.25f,
                                     carChassis.getSize().height * 0.5f,
                                     -carChassis.getSize().depth * 0.3f));
      pLightLeft.rotate(carChassis.getRotator());

      pLightRight.translate(carChassis.getPosition() +
                            glm::vec3(carChassis.getSize().width * 0.5f - 0.75f,
                                      carChassis.getSize().height * 0.5f,
                                      -carChassis.getSize().depth * 0.3f));
      pLightRight.rotate(carChassis.getRotator());

      pLightLeft.intensity =
          (sin(lastTime * 0.001f * 18) * 0.5 + 0.5 < 0.5 ? 0.0f : 1.0f);
      pLightRight.intensity =
          (sin(lastTime * 0.001f * 19 + glm::pi<GLfloat>()) * 0.5 + 0.5 < 0.5
               ? 0.0f
               : 1.0f);

      carCamera.translate(carPosition + glm::vec3(0.0f, 1.5f, 0.0f));
      glm::vec3 carFront =
          testVehicle->getForward() * glm::vec3(1.0f, 1.0f, 1.0f);
      carCamera.setForward(carFront - carCamera.getPosition());
      mainCamera.setForward(mainCamera.getPosition() - carPosition);

      carDistanceLine.translate(carPosition);
      carDistanceLine.rotate(carChassis.getRotator());

      // text_fps.scaleToWidth(test_rect.getSize().width);
      // text_fps.setText(std::to_string((int)abs(vehicle->getCurrentSpeedKmHour()))
      // + " km/h");
      if (fpsCounter <= 0.0f) {
        text_fps.setText(std::to_string((int)(1.0f / deltaTime)));
        fpsCounter = fpsDelay;
      } else {
        fpsCounter -= deltaTime;
      }

      {
        //   std::vector<Point> points = carDistanceLine.getPoints();
        //   btVector3 rayStartc = toBtVec3(carDistanceLine.getModelMatrix() *
        //                                  glm::vec4(points[0].position, 1.0f));
        //   btVector3 rayEnd = toBtVec3(carDistanceLine.getModelMatrix() *
        //                               glm::vec4(points[1].position, 1.0f));
        //   btCollisionWorld::ClosestRayResultCallback RayCallback(rayStart,
        //                                                          rayEnd);
        //   scene_main.getPhysicsWorld()->rayTest(rayStart, rayEnd,
        //   RayCallback); if (false && RayCallback.hasHit()) {
        //     rayEnd = RayCallback.m_hitPointWorld;
        //     btVector3 normal = RayCallback.m_hitNormalWorld;
        //     // std::cout << rayEnd.getY() << std::endl;
        //     pLight2.translate(
        //         glm::vec3(rayEnd.getX(), rayEnd.getY(), rayEnd.getZ()));
        //     std::cout << "to close!!" << std::endl;

        //     vehicle->setBrake(500, 0);
        //     vehicle->setBrake(500, 1);
        //     vehicle->setBrake(500, 2);
        //     vehicle->setBrake(500, 3);

        //     emergencyBrake = true;
        //     carDistanceLine.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        //   } else {
        //     emergencyBrake = false;
        //     carDistanceLine.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        //   }
      }
      carTrace->settings.useCustomColor = true;
      carTrace->settings.customColor = glm::vec4(
          glm::min(abs(vehicle->getCurrentSpeedKmHour() / 300.0f), 1.0f), 0.0f,
          0.0f, 1.0f);
      carTrace->lineTo(carPosition);
      carSelectedCircle.rotate(carChassis.getRotator());
      carSelectedCircle.scale(glm::vec3(
          1.0f,
          glm::min(abs(vehicle->getCurrentSpeedKmHour() / 300.0f), 1.0f) *
                  2.0f +
              1.0f,
          1.0f));
      carSelectedCircle.translate(
          glm::vec3(carPosition.x, 0.1f, carPosition.z));
      // testCamera.translate(carChassis.getPositionCenter() -
      // testVehicle->getFront() + glm::vec3(0.0f, 1.0f, 0.0f));
      // testCamera.lookAt(carPosition + testVehicle->getFront() +
      // glm::vec3(0.0f, 1.0f, 0.0f));

      testCamera.translate(carChassis.getPosition() +
                           glm::vec3(0.0f, 60.0f, 0.0f));
    }

    scene_main.draw(deltaTime);
    sceneOverlay.draw(deltaTime);

    window.getWindow()->display();
    GLfloat currentTime = clock.getElapsedTime().asMilliseconds();
    deltaTime = 0.001f * (currentTime - lastTime);
    lastTime = currentTime;
  }

  // for (auto body : rigidBodys) {
  //   while (body->getBody()->getNumConstraintRefs()) {
  //     btTypedConstraint *constraint = body->getBody()->getConstraintRef(0);
  //     scene_main.getPhysicsWorld()->removeConstraint(constraint);
  //   }
  // }

  scene_main.dispose();
  // sceneOverlay.dispose();

  dLight.dispose();

  // free memory
  for (auto rbody : rigidBodys) {
    rbody->dispose();
    delete rbody;
  }
  rigidBodys.clear();

  for (auto cube : cubes) {
    delete cube;
  }

  carChassisShape.dispose();

  carWheels.clear();
  delete vehicle;

  terrainShape.dispose();

  return 0;
}

void handle_key(Window &window) {

  float mouseSpeed = 0.0f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
    mouseSpeed = 150.0f;
  } else {
    mouseSpeed = 30.0f;
  }

  glm::vec3 camPos = mainCamera.getPosition();
  glm::vec3 camFront = mainCamera.getForward();
  glm::vec3 camRight = mainCamera.getRight();
  glm::vec3 camUp = mainCamera.getUp();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    mainCamera.translate(camPos += camFront * mouseSpeed * deltaTime);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    mainCamera.translate(camPos -= camFront * mouseSpeed * deltaTime);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    mainCamera.translate(camPos -= camRight * mouseSpeed * deltaTime);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    mainCamera.translate(camPos += camRight * mouseSpeed * deltaTime);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    mainCamera.translate(camPos -= camUp * mouseSpeed * deltaTime);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    mainCamera.translate(camPos += camUp * mouseSpeed * deltaTime);

  sf::Event e;
  while (window.getWindow()->pollEvent(e)) {
    if (e.type == sf::Event::Closed) {
      window.getWindow()->close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.getWindow()->close();
    }

    vehicle->setBrake(0, 0);
    vehicle->setBrake(0, 1);
    vehicle->setBrake(0, 2);
    vehicle->setBrake(0, 3);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !emergencyBrake) {
      vehicle->applyEngineForce(500, 0);
      vehicle->applyEngineForce(500, 1);
      vehicle->applyEngineForce(10000, 2);
      vehicle->applyEngineForce(10000, 3);
    } else {
      if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        vehicle->applyEngineForce(0, 0);
        vehicle->applyEngineForce(0, 1);
        vehicle->applyEngineForce(0, 2);
        vehicle->applyEngineForce(0, 3);
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      vehicle->applyEngineForce(-5000, 0);
      vehicle->applyEngineForce(-5000, 1);
      vehicle->applyEngineForce(-500, 2);
      vehicle->applyEngineForce(-500, 3);
    }
    vehicle->setSteeringValue(btScalar(0.0), 2);
    vehicle->setSteeringValue(btScalar(0.0), 3);

    float steering =
        (1.0f - std::fminf(fabs(testVehicle->getVehicleObjectHandle()
                                    ->getCurrentSpeedKmHour()) /
                               150.0f,
                           0.93f)) *
        1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      vehicle->setSteeringValue(steering, 2);
      vehicle->setSteeringValue(steering, 3);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      vehicle->setSteeringValue(-steering, 2);
      vehicle->setSteeringValue(-steering, 3);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      vehicle->applyEngineForce(btScalar(0), 0);
      vehicle->applyEngineForce(btScalar(0), 1);
      vehicle->applyEngineForce(btScalar(0), 2);
      vehicle->applyEngineForce(btScalar(0), 3);
      vehicle->setBrake(btScalar(400), 0);
      vehicle->setBrake(btScalar(400), 1);
      vehicle->setBrake(btScalar(400), 2);
      vehicle->setBrake(btScalar(400), 3);
    }
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
    //   btRigidBody *chassis =
    //       static_cast<btRigidBody
    //       *>(testVehicle->getCollisionObjectHandle());
    //   chassis->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    //   chassis->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
    //   Mesh *dchassis = vehicle->ge();
    //   dchassis->translate(glm::vec3(0.0f, 2.0f, 0.0f));
    //   dchassis->rotate(Rotator(0.0f, 0.0f, 0.0f));
    //   testVehicle->refreshBody();
    //   carTrace->clear();
    // }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)) {
      mainScenePtr->renderMode = RenderMode::POINTR;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
      mainScenePtr->renderMode = RenderMode::LINER;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
      mainScenePtr->renderMode = RenderMode::FILLR;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
      sceneCamera = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
      sceneCamera = 1;
    }
  }
}

/*
void key_callback(sf::Window * window, int key, int scancode, int action, int
mode)
{
        if (action == GLFW_PRESS) {
                keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
                keys[key] = false;
        }

        if (keys[GLFW_KEY_ESCAPE])
                glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(sf::Window * window, double xpos, double ypos)
{
}

void mouse_scroll_callback(sf::Window * window, double xoffset, double
yoffset)
{
}

void handle_key()
{
        float mouseSpeed = 0.0f;
        if (keys[GLFW_KEY_LEFT_SHIFT]) {
                mouseSpeed = 150.0f;
        }
        else {
                mouseSpeed = 30.0f;
        }

        glm::vec3 camPos = mainCamera.getPosition();
        glm::vec3 camFront = mainCamera.getFront();
        glm::vec3 camRight = mainCamera.getRight();
        glm::vec3 camUp = mainCamera.getUp();
        if (keys[GLFW_KEY_W])
                mainCamera.translate(camPos += camFront * mouseSpeed *
deltaTime); if (keys[GLFW_KEY_S]) mainCamera.translate(camPos -= camFront *
mouseSpeed * deltaTime); if (keys[GLFW_KEY_A]) mainCamera.translate(camPos -=
camRight * mouseSpeed * deltaTime); if (keys[GLFW_KEY_D])
                mainCamera.translate(camPos += camRight * mouseSpeed *
deltaTime); if (keys[GLFW_KEY_E]) mainCamera.translate(camPos -= camUp *
mouseSpeed * deltaTime); if (keys[GLFW_KEY_Q]) mainCamera.translate(camPos +=
camUp * mouseSpeed * deltaTime); else if(!emergencyBrake) {
vehicle->setBrake(0, 0); vehicle->setBrake(0, 1); vehicle->setBrake(0, 2);
vehicle->setBrake(0, 3);
        }
        if (keys[GLFW_KEY_UP] && !emergencyBrake) {
                vehicle->applyEngineForce(500, 0);
                vehicle->applyEngineForce(500, 1);
                vehicle->applyEngineForce(10000, 2);
                vehicle->applyEngineForce(10000, 3);
        }
        else {
                if (!keys[GLFW_KEY_DOWN]) {
                        vehicle->applyEngineForce(0, 0);
                        vehicle->applyEngineForce(0, 1);
                        vehicle->applyEngineForce(0, 2);
                        vehicle->applyEngineForce(0, 3);
                }
        }
        if (keys[GLFW_KEY_DOWN]) {
                vehicle->applyEngineForce(-5000, 0);
                vehicle->applyEngineForce(-5000, 1);
                vehicle->applyEngineForce(-500, 2);
                vehicle->applyEngineForce(-500, 3);
        }
        vehicle->setSteeringValue(btScalar(0.0), 2);
        vehicle->setSteeringValue(btScalar(0.0), 3);
        float steering = (1.0f -
std::fminf(std::fabsf(testVehicle->getVehicle()->getCurrentSpeedKmHour()) /
150.0f, 0.93f)) * 1.0f; if (keys[GLFW_KEY_LEFT]) {
                vehicle->setSteeringValue(steering, 2);
                vehicle->setSteeringValue(steering, 3);
        }
        if (keys[GLFW_KEY_RIGHT]) {
                vehicle->setSteeringValue(-steering, 2);
                vehicle->setSteeringValue(-steering, 3);
        }
        if (keys[GLFW_KEY_SPACE]) {
                vehicle->applyEngineForce(0, 0);
                vehicle->applyEngineForce(0, 1);
                vehicle->applyEngineForce(0, 2);
                vehicle->applyEngineForce(0, 3);
                vehicle->setBrake(400, 0);
                vehicle->setBrake(400, 1);
                vehicle->setBrake(400, 2);
                vehicle->setBrake(400, 3);
        }
        if (keys[GLFW_KEY_F]) {
                RigidBody *chassis = testVehicle->getChassis();
                chassis->getBody()->setLinearVelocity(btVector3(0.0f, 0.0f,
0.0f)); chassis->getBody()->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
                Drawable *dchassis = chassis->getDrawable();
                dchassis->translateCenter(glm::vec3(0.0f, 2.0f, 0.0f));
                dchassis->rotate(0.0f, 0.0f, 0.0f);
                chassis->syncBody();
                carTrace->clear();
        }

        if (keys[GLFW_KEY_M]) {
                if (mainScenePtr->renderMode == RenderMode::FILLR) {
                        mainScenePtr->renderMode = RenderMode::LINER;
                }
                else {
                        mainScenePtr->renderMode = RenderMode::FILLR;
                }
        }

}
*/