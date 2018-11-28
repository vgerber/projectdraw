#pragma once


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <math.h>

class Rotator {
public:
    Rotator();
    Rotator(glm::quat rotation);

    void rotateEuler(float x, float y, float z);
    void rotateEuler(glm::vec3 rotation);

    void rotate(glm::quat rotation);

    void rotateAxis(float radians, glm::vec3 rotationAxis);

    void applyRotation(glm::quat rotation);
    void applyRotation(glm::vec3 euler);
    void applyRotation(Rotator rotator);

    void vectorRotation(glm::vec3 vecFrom, glm::vec3 vecTo);

    void interpolate(glm::quat rotation, float factor);
    void interpolate(Rotator rotator, float factor);

    glm::vec3 getRotationAxis();
    glm::vec3 getRotationEuler();
    glm::quat getRotation();
    glm::mat4 getRotationMatrix();

    bool isEqual(Rotator rotator);
    bool isEqual(glm::quat rotation);


private:
      glm::quat rotation;
};