#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <OgreVector3.h>

class Scene;

/** Klasa obiektów.
    @remarks
    Przechowuje w sobie wskaźniki na elementy potrzebne do zarządzania obiektem. Ma ułatwiać pracę z Ogre, żeby nie trzeba było przeszukiwać scen w poszukiwaniu obiektów. Umożliwia trzymanie scen jako osobnych obiektów klasy Scene.
*/
class Object
{
public:
  /** Wskaźnik na węzeł-rodzic obiektu.
      @remarks
      Pozwala na manipulację obiektem w przestrzeni 3D. \n
      Sam węzeł nie należy do nas. Pamięcią zajmuje się Ogre.
  */
  Ogre::SceneNode* Node; //to tylko skrót. Pamięcią zajmuje się Ogre

  /** Konstruktor.
      @param meshFile Nazwa pliku z zasobów (Resource), w którym znajduje sie siatka obiektu.
      @param position Początkowa pozycja obiektu.
  */
  Object(const std::string& meshFile, const Ogre::Vector3& position=Ogre::Vector3(0,0,0));

  /** Wypisuje współrzędne każdego wierzchołka każdej ściany na std::cout.
      @note
      Pozostałość po analizowaniu jak Ogre przetrzymuje obiekty w pamięci.
  */
  void PrintFaceVertexCoords() const;

protected:
  /** Domyślny konstruktor.
      @remarks
      Jest wymagany przez konstruktor Scene::Scene.
  */
  Object(){}

  friend class Scene;
};

#endif
