#ifndef SCENE_HPP
#define SCENE_HPP

#include "object.hpp"
#include <boost/property_tree/ini_parser.hpp>
#include <QtCore/QObject>

class Scene:public QObject
{

Q_OBJECT

signals:
  void ObjectsCollisionInSceneChanged(bool SceneCollisionState);

protected:
  bool isCollisionSwitchStillOn;

public:
  /* Ogre::Vector3::squaredDistance
     This method is for efficiency - calculating the actual distance to another vector requires a square root, which is expensive in terms of the operations required. This method returns the square of the distance to another vector, i.e. the same as the distance but before the square root is taken. Use this if you want to find the longest / shortest distance without incurring the square root.
  */
  float DistanceSquared; 
  Object ObjectA, ObjectB;

  Scene(boost::property_tree::basic_ptree<std::string, std::string>& propertyTree);

  void CheckSceneCollision();
};

#endif
