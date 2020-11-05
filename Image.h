#include "Object2D.h"
#include "Texture.h"

#include <string>

class Image : public Object2D
{
  struct
  {
    GLfloat x, y, width, height;
  } m_texCoord;

  std::string m_file;
  const Texture *m_texture;
  GLuint m_listIndex;       /* display list */
  bool m_listIndexValid;    /* true if m_listIndex was registered */

   /* resetDisplayList: reset display list */
   void resetDisplayList();

   /* initialize: initialize texture */
   void initialize();

   /* clear: free texture */
   void clear();
   void deleteTexture();

public:
   /* Image: constructor */
   Image();

   /* Image: destructor */
   ~Image();

   /* load: load a texture from file name */
   bool load(const char *file);
   const char* file();

  void setClipPosition(const int x, const int y);
  void setClipSize(const int width, const int height);

  bool isImage() { return true; }

  /* render: render the textures */
  void render(MMDAgent *mmdagent);
};