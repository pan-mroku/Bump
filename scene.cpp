#include "scene.hpp"
#include <OgreSceneNode.h>

Scene::Scene(boost::property_tree::basic_ptree<std::string, std::string>& propertyTree):isCollisionSwitchStillOn(false)
{
  for(auto property : propertyTree) //std::pair<string,basic_ptree>
    if(property.first=="ObjectA")
      ObjectA=Object(property.second.data(),Ogre::Vector3(-1,0,0));
    else if(property.first=="ObjectB")
      ObjectB=Object(property.second.data(),Ogre::Vector3(1,0,0));
    else if(property.first=="DistanceSquared")
      {
        float distance=std::stof(property.second.data());
        DistanceSquared=distance*distance;
      }
}

void Scene::CheckSceneCollision()
{
  bool collision;
  Ogre::Vector3 positionA=ObjectA.Node->_getDerivedPosition();
  Ogre::Vector3 positionB=ObjectB.Node->_getDerivedPosition();
  
  if(positionA.squaredDistance(positionB)-DistanceSquared<0.1)
    collision=true;
  else
    collision=false;

  if(collision!=isCollisionSwitchStillOn)
    {
      isCollisionSwitchStillOn=collision;
      emit(ObjectsCollisionInSceneChanged(collision));
    }
    
}
