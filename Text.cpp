#include "Text.h"

Text::Text()
{
  initialize();
}

Text::~Text()
{
  clear();
}

void Text::initialize()
{
  setFontSize(16);
  Object2D::initialize();
}

void Text::setText(const char *s)
{
  m_text = s;
} 

const char* Text::text()
{
  return m_text.c_str();
}

void Text::setFontSize(const int size)
{
  m_fontSize =size;
}

void Text::render(MMDAgent *mmdagent)
{
  GLfloat x, y;

  glPushMatrix();

  glColor4f(m_color.red, m_color.green, m_color.blue, m_color.alpha);
  x = ((m_scaleX - 1) * m_fontSize) / 2;
  y = ((m_scaleY - 1) * m_fontSize) / 2;
  glTranslatef(m_vertex.x - x, m_vertex.y + y + m_fontSize, m_layer);
  glScalef(m_fontSize * m_scaleX, -m_fontSize * m_scaleY, 1);
  mmdagent->drawString(m_text.c_str());

  glPopMatrix();
}

int Text::fontSize()
{
  return m_fontSize;
}