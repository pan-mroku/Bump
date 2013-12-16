#include "collisiondetector.hpp"

CollisionDetector::CollisionDetector():
  isCollisionSwitchStillOn(false),
  ActiveAlgorithm(CollisionAlgorithmEnum::None)
{
}

void CollisionDetector::CheckCollision(const Object& objectA, const Object& objectB)
{
  bool collision;
  switch(ActiveAlgorithm)
    {
    case None:
      collision=NoneCollisionAlgorithm(objectA, objectB);
      break;
    }

  if( collision != isCollisionSwitchStillOn )
    {
      isCollisionSwitchStillOn=collision;
      emit(ObjectsCollisionInAlgorithmChanged(collision));
    }
}

bool CollisionDetector::NoneCollisionAlgorithm(const Object& objectA, const Object& objectB)
{
  return false;
}
