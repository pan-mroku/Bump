#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <OgreVector3.h>

class Object
{
public:
  Ogre::SceneNode* Node; //to tylko skrót. Pamięcią zajmuje sie Ogre

  Object(){}
  Object(const std::string& meshFile, const Ogre::Vector3& position=Ogre::Vector3(0,0,0));

  void PrintFaceVertexCoords() const;
};

#endif
