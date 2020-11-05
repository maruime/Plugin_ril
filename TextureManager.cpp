#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

 const Texture* TextureManager::loadImage(const char *file)
 {
   Item &item = m_textures[file];

   if (TEXTURE_UNINITIALIZEDID == item.loader.getID())
   {
     item.loader.load(file);
   }

   ++item.refcount;

   return &item.loader;
 }

 void TextureManager::deleteImage(const char *file)
 {
   ItemContainer::iterator it = m_textures.find(file);
   if (it == m_textures.end()) return;

  --it->second.refcount;
  if (0 < it->second.refcount) return;

  m_textures.erase(it);
 }