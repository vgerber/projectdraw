find_package(Bullet CONFIG REQUIRED)

set(PHYSICS_SRC
    Physics3D/Util/conversion.h
    Physics3D/Util/debugdrawer.cpp
    Physics3D/Util/conversion.cpp
    Physics3D/Util/debugdrawer.h
    Physics3D/World/discreteworld.cpp
    Physics3D/World/discreteworld.h
    Physics3D/World/collisionworld.cpp
    Physics3D/World/collisionworld.h
    Physics3D/Collision/Shapes/cshape.cpp
    Physics3D/Collision/Shapes/cshape.h
    Physics3D/CollisionObject/Rigidbody/rigidbody.h
    Physics3D/CollisionObject/Rigidbody/rigidbody.cpp
    Physics3D/CollisionObject/Vehicle/raycastvehicle.cpp
    Physics3D/CollisionObject/Vehicle/raycastvehicle.h
    Physics3D/CollisionObject/collisionobject.cpp
    Physics3D/CollisionObject/collisionobject.h
    Physics3D/CollisionObject/Trigger/triggervolume.cpp
    Physics3D/CollisionObject/Trigger/triggervolume.h
)

add_library(physics ${PHYSICS_SRC}) 
target_link_libraries(physics PUBLIC core ${BULLET_LIBRARIES})