/* headers */

#ifndef ___RIL_LOGGER_H__
#define ___RIL_LOGGER_H__

#include "MMDAgent.h"
#include <list>

/* definitions */

#define RILLOGGER_LINESTEP   5
#define RILLOGGER_X  0
#define RILLOGGER_Y  0
#define RILLOGGER_TEXTWIDTH  256
#define RILLOGGER_TEXTHEIGHT 30
#define RILLOGGER_FONTSIZE 14
#define RILLOGGER_TEXTCOLOR 1.0f, 1.0f, 1.0f, 0.8f
#define RILLOGGER_BGCOLOR 0, 0, 0, 1

class Ril_Thread;

/* Ril_Logger: Debug output Ril status in OpenGL */
class Ril_Logger
{
private:
   Ril_Thread *m_parent;
  
   std::list<std::string> m_history;
   GLfloat m_fontSize;
   int m_x, m_y, m_width, m_height;
   struct
   {
     GLfloat red, green, blue, alpha;
   } m_bgcolor, m_color;

   /* initialize: initialize logger */
   void initialize();

public:

   /* Ril_Logger: constructor */
   Ril_Logger();

   /* ~Ril_Logger: destructor */
   ~Ril_Logger();

   /* setup: setup logger */
   void setup(Ril_Thread *parent);

   /* clear: free logger */
   void clear();
   void setFontSize(int size);
   void setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
   void setBGColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
   void setPosition(int x, int y);
   void setSize(int width, int height);

   void print(const char * format, ...);
   void newLine();
   void draw(const char *s);

   /* render: render log */
   void render();
};

#endif