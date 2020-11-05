#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "MMDAgent.h"
#include "Image.h"
#include "Text.h"
#include <map>
#include <list>

using namespace std;

typedef map<string, Object2D*> ObjectContainer;

class Renderer
{
  MMDAgent *m_mmdagent;
  ObjectContainer m_objects;
  struct RenderObject
  {
    Object2D *obj;
  };
  list<RenderObject> m_renderList;
  int m_screenWidth, m_screenHeight;
  bool m_isRendering;

  void initialize();
  void clear();
public:
  Renderer();
  ~Renderer();
  void setup(MMDAgent *mmdagent);
  void addObject(const char *alias, Object2D *object);
  void deleteObject(const char *alias);
  void deleteObject(ObjectContainer::iterator &it);
  void deleteAllImages();
  void deleteAllTextBoxs();
  Object2D* getObject(const char *alias);
  Image* getImage(const char *alias);
  Text* getTextBox(const char *alias);
  void addRender(Object2D *obj, int layer);
  void deleteRender(Object2D *obj);
  void view2D();
  void view3D();
  void render();
};

#endif