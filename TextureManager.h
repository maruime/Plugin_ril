#include "Texture.h"
#include <map>
#include <string>

using namespace std;

class TextureManager
{
  struct Item
  {
    Texture loader;
    int refcount;
    Item() { refcount = 0; }
  };
  typedef map<string, Item> ItemContainer;
  ItemContainer m_textures;
public:
  TextureManager();
  ~TextureManager();
  const Texture* loadImage(const char *file);
  void deleteImage(const char *file);
};