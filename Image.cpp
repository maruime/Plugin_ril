#include "Image.h"
#include "TextureManager.h"

static TextureManager textureManager;

/* Image::resetDisplayList: reset display list */
void Image::resetDisplayList()
{
   if (!m_listIndexValid) return;

   glDeleteLists(m_listIndex, 1);
   m_listIndexValid = false;
}

/* Image::initialize: initialize texture */
void Image::initialize()
{
   m_texture = NULL;
   m_listIndex = 0;
   m_listIndexValid = false;
   Object2D::initialize();
}

/* Image::clear: free texture */
void Image::clear()
{
  deleteTexture();
  Object2D::clear();
}

void Image::deleteTexture()
{
   resetDisplayList();
   if (NULL != m_texture)
   {
     m_texture = NULL;
     textureManager.deleteImage(m_file.c_str());
   }
}

/* TileTexture::TileTexture: constructor */
Image::Image()
{
   initialize();
}

/* TileTexture: destructor */
Image::~Image()
{
   clear();
}

/* TileTexture::load: load a texture from file name (wide char) */
bool Image::load(const char *file)
{
  deleteTexture();

  if (MMDAgent_strlen(file) <= 0) return false;

  m_texture = textureManager.loadImage(file);
  m_file = file;
  setClipPosition(0, 0);
  setClipSize(m_texture->width(), m_texture->height());

  return true;
}

const char* Image::file()
{
  return m_file.c_str();
}

void Image::setClipPosition(const int x, const int y)
{
  if (NULL == m_texture) return;
  m_texCoord.x = (GLfloat)x / m_texture->width();
  m_texCoord.y = (GLfloat)y / m_texture->height();
  resetDisplayList();
}

void Image::setClipSize(const int width, const int height)
{
  if (NULL == m_texture) return;
  setSize(width, height);
  m_texCoord.width = (GLfloat)width / m_texture->width();
  m_texCoord.height = (GLfloat)height / m_texture->height();
  resetDisplayList();
}

/* Image::render: render the textures */
void Image::render(MMDAgent *mmdagent)
{
  GLfloat x, y;
  if (m_listIndexValid)
  {
    /* call display list */
    glPushMatrix();
    glColor4f(m_color.red, m_color.green, m_color.blue, m_color.alpha);
    x = ((m_scaleX - 1) * m_vertex.width) / 2;
    y = ((m_scaleY - 1) * m_vertex.height) / 2;
    glTranslatef(m_vertex.x - x, m_vertex.y - y, 0);
    glScalef(m_scaleX, m_scaleY, 1);
    glCallList(m_listIndex);
    glPopMatrix();
    return;
  }

  /* create display list  */
  m_listIndex = glGenLists(1); /* get display list index */
  glNewList(m_listIndex, GL_COMPILE);

  /* start of draw */
  glPushMatrix();
  if (NULL != m_texture)
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture->getID());
  }
  glBegin(GL_QUADS);
  // 1: left top
  if (NULL != m_texture) glTexCoord2f(m_texCoord.x, m_texCoord.y);
  glVertex3f(0, 0, m_layer);
  // 2: left bottom
  if (NULL != m_texture) glTexCoord2f(m_texCoord.x, m_texCoord.y + m_texCoord.height);
  glVertex3f(0, m_vertex.height, m_layer);
  // 3: right bottom
  if (NULL != m_texture) glTexCoord2f(m_texCoord.x + m_texCoord.width, m_texCoord.y + m_texCoord.height);
  glVertex3f(m_vertex.width, m_vertex.height, m_layer);
  // 4: right top
  if (NULL != m_texture) glTexCoord2f(m_texCoord.x + m_texCoord.width, m_texCoord.y);
  glVertex3f(m_vertex.width, 0, m_layer);
  glEnd();

  /* end of draw */
  if (NULL != m_texture) glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  /* end of regist */
  glEndList();
  m_listIndexValid = true;
  render(mmdagent);
}
