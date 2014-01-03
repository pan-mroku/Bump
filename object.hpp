#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <OgreVector3.h>

class Scene;
class CollisionDetector;

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
  Ogre::SceneNode* Node;

  /** Wskaźnik na obiekt przechowujący mesh.
      @remarks
      Ułatwia wydobycie danych bezpośrednio związanych z obiektem.*/
  Ogre::Entity* Entity;

  /** Wskaźnik na obiekt Ogre::Mesh.*/
  Ogre::Mesh* Mesh;

  /** Wektor przechowujący wierzchołki.
      @remarks
      Odczytywanie ich za każdym razem z buforów (Ogre::HardwareVertexBuffer) było niewydajne.\n
      Wybrałem wektor, żeby mieć operator[].*/
  std::vector<Ogre::Vector3> VerticesBuffer;

  /** Lista indeksów kolejnych wierzchołków tworzących trójkąty.
      @remarks
      Odczytywanie ich za każdym razem z buforów (Ogre::HardwareIndexBuffer) było niewydajne.*/
  std::vector<int> IndicesBuffer;

  /** Konstruktor.
      @param meshFile Nazwa pliku z zasobów (Resource), w którym znajduje sie siatka obiektu.
      @param position Początkowa pozycja obiektu.
      @param left określa, czy obiekt jest z lewej strony, czy prawej. Wpływa to na wektor ruchu.
  */
  Object(const std::string& meshFile, const Ogre::Vector3& position=Ogre::Vector3(-1,0,0), bool left=true);

  /** Wypisuje współrzędne każdego wierzchołka każdej ściany na std::cout.
      @note
      Pozostałość po analizowaniu jak Ogre przetrzymuje obiekty w pamięci.
  */
  void PrintFaceVertexCoords() const;

  /** Próba wygodniejszego wypisania wierzchołków.
      @note
      Próba nieudana. Podejście od strony EdgeList tak samo wymaga wywoływania lock na HardwareBuffer.*/
  void PrintFaceVertexCoordsCivilised() const;

  /** Przesuwa obiekt zgodnie z czasem, jaki upłynął od ostatniej klatki. */
  void Move(unsigned long delta);

  /** Odwraca ruch obiektu. */
  void FlipMoveVector();

  /** Zwraca bounding box obiektu.
      @remarks
      Ogre::WireBoundingBox *mWireBoundingBox z Ogre::SceneNode jest prywatnym polem, więc wystawimy AABB z Ogre::Entity (ostatecznie to jest obiekt z meshem. SceneNode to tylko węzeł sceny.*/
  const Ogre::AxisAlignedBox& GetBoundingBox() const;

  /** Odświerza bufory przechowujące wierzchołki i indeksy.
      @remarks
      Zakłada, że bufory rzeczywiście należy odźwierzyć. Nie sprawdza w żaden sposób, czy ma to sens, czy nie.
   */
  void UpdateBuffers();

protected:
  /** Domyślny konstruktor.
      @remarks
      Jest wymagany przez konstruktor Scene::Scene.
  */
  Object(){}

  Ogre::Vector3 moveVector;
  Ogre::Quaternion quat;

  friend class Scene;
  friend class CollisionDetector;
};

#endif
