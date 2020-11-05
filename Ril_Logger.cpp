/* headers */

#include "Ril_Logger.h"
#include "Ril_Thread.h"

/* Ril_Logger::initialize: initialize logger */
void Ril_Logger::initialize()
{
   m_parent = NULL;
   setFontSize(RILLOGGER_FONTSIZE);
   setBGColor(RILLOGGER_BGCOLOR);
   setColor(RILLOGGER_TEXTCOLOR);
   setPosition(RILLOGGER_X, RILLOGGER_Y);
   setSize(RILLOGGER_TEXTWIDTH, RILLOGGER_TEXTHEIGHT);
}

/* Ril_Logger::clear: free logger */
void Ril_Logger::clear()
{
   m_history.clear();
   newLine();
}

/* Ril_Logger::Ril_Logger: constructor */
Ril_Logger::Ril_Logger()
{
   initialize();
   newLine();
}

/* Ril_Logger::~Ril_Logger: destructor */
Ril_Logger::~Ril_Logger()
{
}

/* Ril_Logger::setup: setup logger */
void Ril_Logger::setup(Ril_Thread *parent)
{
   m_parent = parent;
}

void Ril_Logger::setFontSize(int size)
{
  m_fontSize = size;
}

void Ril_Logger::setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  m_color.red = red;
  m_color.green = green;
  m_color.blue = blue;
  m_color.alpha = alpha;
}

void Ril_Logger::setBGColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  m_bgcolor.red = red;
  m_bgcolor.green = green;
  m_bgcolor.blue = blue;
  m_bgcolor.alpha = alpha;
}

 void Ril_Logger::setPosition(int x, int y)
 {
  m_x = x;
  m_y = y;
 }

 void Ril_Logger::setSize(int width, int height)
 {
   m_width = width;
   m_height = height;
 }

void Ril_Logger::print(const char * format, ...)
{
  char buf[1024];
  va_list arg;

  va_start(arg, format); 
  vsprintf(buf, format, arg);
  va_end(arg);

  m_history.back() += buf;
}

void Ril_Logger::newLine()
{
  if (m_height < m_history.size())
  {
    m_history.erase(m_history.begin());
  }
  m_history.push_back("");
}

void Ril_Logger::draw(const char *s)
{
  glScalef(m_fontSize, -m_fontSize, 1);
  m_parent->mmdagent()->drawString(s);
}

/* Ril_Logger::render: render log */
void Ril_Logger::render()
{
  if (m_parent == NULL) return;

  /* start of draw */
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);// 射影変換行列設定
  glPushMatrix();// 現在の射影変換行列を保存
  glLoadIdentity();
  glOrtho(0, m_parent->screenWidth(), m_parent->screenHeight(), 0, -1, 1);// 正射影変換設定
  glMatrixMode(GL_MODELVIEW);// モデルビュー変換行列設定
  glPushMatrix();// 現在のモデルビュー行列を保存
  glLoadIdentity();// 単位行列を設定

  /* show the history */
  int width = m_width * (m_fontSize) + m_fontSize * 2;
  int height = m_height * (m_fontSize) + m_fontSize * 2;
  glPushMatrix();
  glTranslatef(m_x, m_y, 0);
  glColor4f(m_bgcolor.red, m_bgcolor.green, m_bgcolor.blue, m_bgcolor.alpha);
  glBegin(GL_QUADS);
  glVertex3i(0, 0, 0);
  glVertex3i(0, height, 0);
  glVertex3i(width, height, 0);
  glVertex3i(width, 0, 0);
  glEnd();

  glTranslatef(m_fontSize, m_fontSize * 2, 0);
  glColor4f(m_color.red, m_color.green, m_color.blue, m_color.alpha);
  std::list<std::string>::iterator it = m_history.begin();
  for (; it != m_history.end(); ++it) {
    glPushMatrix();
    draw((*it).c_str());
    glPopMatrix();
    glTranslatef(0, m_fontSize + RILLOGGER_LINESTEP, 0);
  }
  glPopMatrix();

  /* end of draw */
  glEnable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);// 射影変換行列設定
  glPopMatrix();// 射影変換行列を復元
  glMatrixMode(GL_MODELVIEW);// モデルビュー変換行列設定
  glPopMatrix();// モデルビュー行列を復元
  glLoadIdentity();// 単位行列を設定
}
