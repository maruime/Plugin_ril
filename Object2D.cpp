#include "Object2D.h"

/* Object2D::initialize: initialize texture */
void Object2D::initialize()
{
  setColor(1, 1, 1);
  setAlpha(1);
  setLayer(0);
  setScaleX(1);
  setScaleY(1);
  hide();
}

/* Object2D::clear: free texture */
void Object2D::clear()
{
   initialize();
}

void Object2D::show()
{
  m_hide = false;
}

void Object2D::hide()
{
  m_hide = true;
}

bool Object2D::isHidden()
{
  return m_hide;
}

/* TileTexture::TileTexture: constructor */
Object2D::Object2D()
{
   initialize();
}

/* TileTexture: destructor */
Object2D::~Object2D()
{
   clear();
}

void Object2D::setLayer(int layer)
{
  m_layer = layer;
}

int Object2D::layer()
{
  return m_layer;
}

void Object2D::setPosition(const GLfloat x, const GLfloat y, const bool absolute)
{
  if (absolute)
  {
    m_vertex.x = x;
    m_vertex.y = y;
  }
  else
  {
    m_vertex.x += x;
    m_vertex.y += y;
  }
}

GLfloat Object2D::x()
{
  return m_vertex.x;
}

GLfloat Object2D::y()
{
  return m_vertex.y;
}

void Object2D::setSize(const GLfloat width, const GLfloat height)
{
  setWidth(width);
  setHeight(height);
}

void Object2D::setWidth(const GLfloat value)
{
  m_vertex.width = value;
}

void Object2D::setHeight(const GLfloat value)
{
  m_vertex.height = value;
}

GLfloat Object2D::width()
{
  return m_vertex.width;
}

GLfloat Object2D::height()
{
  return m_vertex.height;
}

void Object2D::setScaleX(GLfloat value)
{
  m_scaleX = value;
}

void Object2D::setScaleY(GLfloat value)
{
  m_scaleY = value;
}

GLfloat Object2D::scaleX()
{
  return m_scaleX;
}

GLfloat Object2D::scaleY()
{
  return m_scaleY;
}

void Object2D::setColor(const GLfloat r, const GLfloat g, const GLfloat b)
{
  m_color.red = r;
  m_color.green = g;
  m_color.blue = b;
}

void Object2D::setAlpha(const GLfloat a)
{
  m_color.alpha = a;
}
