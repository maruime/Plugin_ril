#include "Renderer.h"

Renderer::Renderer()
{
  initialize();
}

Renderer::~Renderer()
{
  clear();
}

void Renderer::initialize()
{
  m_isRendering = false;
}

void Renderer::clear()
{
  ObjectContainer::iterator it = m_objects.begin();
  while (it != m_objects.end())
  {
    delete it->second;
    it = m_objects.erase(it);
  }
  m_renderList.clear();
}

void Renderer::setup(MMDAgent *mmdagent)
{
  m_mmdagent = mmdagent;
  m_mmdagent->getWindowSize(&m_screenWidth, &m_screenHeight);
}

void Renderer::addObject(const char *alias, Object2D *object)
{
  deleteObject(alias);
  m_objects[alias] = object;
}

void Renderer::deleteObject(const char *alias)
{
  ObjectContainer::iterator it = m_objects.find(alias);
  if (it == m_objects.end()) return;
  deleteObject(it);
}

void Renderer::deleteObject(ObjectContainer::iterator &it)
{
  it->second->hide();
  deleteRender(it->second);
  delete it->second;
  it = m_objects.erase(it);
}

void Renderer::deleteAllImages()
{
  ObjectContainer::iterator it = m_objects.begin();
  while (it != m_objects.end())
  {
    if (!it->second->isImage())
    {
      ++it;
      continue;
    }
    deleteObject(it);
  }
}

void Renderer::deleteAllTextBoxs()
{
  ObjectContainer::iterator it = m_objects.begin();
  while (it != m_objects.end())
  {
    if (!it->second->isText())
    {
      ++it;
      continue;
    }
    deleteObject(it);
  }
}

Object2D* Renderer::getObject(const char *alias)
{
  ObjectContainer::iterator it = m_objects.find(alias);
  if (it == m_objects.end()) return NULL;
  return it->second;
}

Image* Renderer::getImage(const char *alias)
{
  return (Image*)getObject(alias);
}

Text* Renderer::getTextBox(const char *alias)
{
  return (Text*)getObject(alias);
}

void Renderer::addRender(Object2D *obj, int layer)
{
  list<RenderObject>::iterator it;

  /* ���łɑ}�����Ă���ꍇ�͍폜 */
  deleteRender(obj);

  while (m_isRendering) MMDAgent_sleep(1);
  for (it = m_renderList.begin(); it != m_renderList.end(); ++it)
  {
    if (it->obj->layer() <= layer) continue;
    break;
  }
  RenderObject renderObj;
  obj->setLayer(layer);
  renderObj.obj = obj;
  m_renderList.insert(it, renderObj);
}

void Renderer::deleteRender(Object2D *obj)
{
  list<RenderObject>::iterator it;
  
  while (m_isRendering) MMDAgent_sleep(1);
  for (it = m_renderList.begin(); it != m_renderList.end(); ++it)
  {
    if (it->obj != obj) continue;
    m_renderList.erase(it);
    break;
  }
}

void Renderer::view2D()
{
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_PROJECTION);// �ˉe�ϊ��s��ݒ�
  glPushMatrix();// ���݂̎ˉe�ϊ��s���ۑ�
  glLoadIdentity();
  glOrtho(0, m_screenWidth, m_screenHeight, 0, -1000, 100);// ���ˉe�ϊ��ݒ�
  glMatrixMode(GL_MODELVIEW);// ���f���r���[�ϊ��s��ݒ�
  glPushMatrix();// ���݂̃��f���r���[�s���ۑ�
  glLoadIdentity();// �P�ʍs���ݒ�
  //glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::view3D()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glMatrixMode(GL_PROJECTION);// �ˉe�ϊ��s��ݒ�
  glPopMatrix();// �ˉe�ϊ��s��𕜌�
  glMatrixMode(GL_MODELVIEW);// ���f���r���[�ϊ��s��ݒ�
  glPopMatrix();// ���f���r���[�s��𕜌�
  glLoadIdentity();// �P�ʍs���ݒ�
}

void Renderer::render()
{
  list<RenderObject>::iterator it;

  view2D();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE); // Z�o�b�t�@��ǂݏo����p��
  m_isRendering = true;
  for (it = m_renderList.begin(); it != m_renderList.end(); ++it)
  {
    if (it->obj->isHidden()) continue;
    it->obj->render(m_mmdagent);
  }
  m_isRendering = false;
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

  view3D();
}