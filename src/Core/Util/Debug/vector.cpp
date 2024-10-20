#include "vector.h"

std::string printVec(glm::vec2 vec) {
  return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + ")";
}

std::string printVec(glm::vec3 vec) {
  return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," +
         std::to_string(vec.z) + ")";
}

std::string printVec(glm::vec4 vec) {
  return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," +
         std::to_string(vec.z) + "," + std::to_string(vec.w) + ")";
}

// std::string printVec(btVector3 vec) {
//   return "(" + std::to_string(vec.getX()) + "," + std::to_string(vec.getY())
//   +
//          "," + std::to_string(vec.getZ()) + ")";
// }