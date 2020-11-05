#include "Ril.h"
#include "Ril_Logger.h"
#include "Renderer.h"
#include "Queue.hpp"
#include <string>
#include <list>
#include <queue>

#define BUTTON_RELEASE 0
#define BUTTON_PRESS 1
#define BUTTON_HOLD 2

enum
{
  MOUSE_BUTTON_LEFT = 0,
  MOUSE_BUTTON_MIDDLE,
  MOUSE_BUTTON_RIGHT,
  MOUSE_BUTTON_SIZE,
};

struct Ril_Timer
{
  double endtime;
  Ril *script;
  Thread *thread;
  Ril_Timer(double endtime, Ril *script, Thread *thread)
  {
    this->endtime = endtime;
    this->script = script;
    this->thread = thread;
  }
};

/* Ril_Thread: thread of Ril */
class Ril_Thread
{
private:
   GLFWmutex m_mutex;
   GLFWcond m_cond;
   GLFWthread m_thread;
   bool m_kill;

   MMDAgent *m_mmdagent;
   Ril_Logger m_logger;
   Renderer m_renderer;
   std::list<Ril*> m_scripts;
   std::list<Ril_Timer> m_timers;

   bool m_isRunning;
   int m_screenWidth, m_screenHeight;
   ril_var_t *m_sharevar;

   /* initialize: initialize thread */
   void initialize();

   /* clear: free thread */
   void clear();

   Ril* newScript();

public:
   HDC m_hDC;
   HGLRC m_drawRC, m_imgRC;
   Queue<Event> m_events;

   /* Ril_Thraed: thread constructor */
   Ril_Thread();

   /* ~Ril_Thread: thread destructor */
   ~Ril_Thread();

   void deleteScripts();
   void deleteScript(std::list<Ril*>::iterator &it);

   Ril* loadScript(const char *file);

   /* loadAndStart: load FST and start thread */
   int loadAndStart(MMDAgent *mmdagent, const char *file);

   /* stopAndRelease: stop thread and release */
   void stopAndRelease();

   /* run: main loop */
   void run();

   void enqueueBuffer(const char *type, const char *args);
   void addTimer(double sec, Ril *script);
   void deleteTimer(Thread *thread);

   /* isRunning: check running */
   bool isRunning();

   void render();
   void renderLog();

   MMDAgent* mmdagent() { return m_mmdagent; };
   Renderer* renderer() { return &m_renderer; }
   int screenWidth() { return m_screenWidth; }
   int screenHeight() { return m_screenHeight; }
   int getMouseState(int type);
   void enableStandardOperation();
   void disableStandardOperation();
};
