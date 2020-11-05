#ifndef __OBJECT2D_H__
#define __OBJECT2D_H__

#include "Texture.h"
#include <string>

class Object2D
{
protected:
  struct
  {
    GLfloat x, y, width, height;
  } m_vertex;
  GLfloat m_scaleX, m_scaleY;

  bool m_hide;
  int m_layer;

   /* initialize: initialize texture */
   virtual void initialize();

   /* clear: free texture */
   virtual void clear();

public:
  struct
  {
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;
  } m_color;

  /* Object2D: constructor */
  Object2D();

  /* Object2D: destructor */
  virtual ~Object2D();
  void show();
  void hide();
  bool isHidden();

  void setLayer(int layer);
  int layer();
  void setColor(const GLfloat r, const GLfloat g, const GLfloat b);
  void setAlpha(const GLfloat a);
  void setPosition(const GLfloat x, const GLfloat y, const bool absolute = true);
  void setSize(const GLfloat width, const GLfloat height);
  void setWidth(const GLfloat value);
  void setHeight(const GLfloat value);
  void setScaleX(const GLfloat value);
  void setScaleY(const GLfloat value);

  virtual bool isText() { return false; }
  virtual bool isImage() { return false; }

   /* render: render the textures */
   virtual void render(MMDAgent *mmdagent) = 0;

  GLfloat x();
  GLfloat y();
  GLfloat width();
  GLfloat height();
  GLfloat scaleX();
  GLfloat scaleY();
};

#endif