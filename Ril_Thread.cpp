#include "MMDAgent.h"
#include "Ril_Thread.h"

static LONG originalWndProc;
int mouseStates[MOUSE_BUTTON_SIZE];
bool stopStandardOperation;

LRESULT  CALLBACK  wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  bool defWindowProc = false;

  switch (msg)
  {
  case WM_LBUTTONDOWN:
    mouseStates[MOUSE_BUTTON_LEFT] = BUTTON_PRESS;
    if (stopStandardOperation) defWindowProc = true;
    break;
  case WM_LBUTTONUP:
    mouseStates[MOUSE_BUTTON_LEFT] = BUTTON_RELEASE;
    if (stopStandardOperation) defWindowProc = true;
    break;
  case WM_MBUTTONDOWN:
    mouseStates[MOUSE_BUTTON_MIDDLE] = BUTTON_PRESS;
    if (stopStandardOperation) defWindowProc = true;
    break;
  case WM_MBUTTONUP:
    mouseStates[MOUSE_BUTTON_MIDDLE] = BUTTON_RELEASE;
    if (stopStandardOperation) defWindowProc = true;
    break;
  case WM_RBUTTONDOWN:
    mouseStates[MOUSE_BUTTON_RIGHT] = BUTTON_PRESS;
    if (stopStandardOperation) defWindowProc = true;
    break;
  case WM_RBUTTONUP:
    mouseStates[MOUSE_BUTTON_RIGHT] = BUTTON_RELEASE;
    if (stopStandardOperation) defWindowProc = true;
    break;
  case WM_MOUSEWHEEL:
    if (stopStandardOperation) defWindowProc = true;
    break;
  }

  if (defWindowProc)
  {
    return DefWindowProc(hWnd,msg,wParam,lParam);
  }

  return CallWindowProc((WNDPROC)originalWndProc, hWnd, msg, wParam, lParam);
}

static void mainThread(void *param)
{
  Ril_Thread *thread = (Ril_Thread*)param;

  thread->run();
}

/* Ril_Thread::initialize: initialize thread */
void Ril_Thread::initialize()
{
  m_isRunning = false;

  m_mutex = NULL;
  m_cond = NULL;
  m_thread = -1;
  m_sharevar = ril_newvar(NULL);
  for (int i = 0; i < MOUSE_BUTTON_SIZE; ++i) mouseStates[i] = BUTTON_RELEASE;
  stopStandardOperation = false;
}

/* Ril_Thread::clear: free thread */
void Ril_Thread::clear()
{
  m_kill = true;

  if (m_cond != NULL) glfwSignalCond(m_cond);

  /* stop thread & close mutex */
  if (m_mutex != NULL || m_cond != NULL || m_thread >= 0)
  {
    if (m_thread >= 0)
    {
      glfwWaitThread(m_thread, GLFW_WAIT);
      glfwDestroyThread(m_thread);
    }
    if (m_cond != NULL) glfwDestroyCond(m_cond);
    if (m_mutex != NULL) glfwDestroyMutex(m_mutex);
    glfwTerminate();
  }

  wglDeleteContext(m_imgRC);

  initialize();
  deleteScripts();
  while (!m_events.empty()) m_events.pop();
  m_timers.clear();
  ril_clearvar(NULL, m_sharevar);

  stopStandardOperation = false;
}

/* Ril_Thread::Ril_Thread: thread constructor */
Ril_Thread::Ril_Thread()
{
  initialize();
}

/* Ril_Thread::~Ril_Thread: thread destructor */
Ril_Thread::~Ril_Thread()
{
  clear();
  ril_deletevar(NULL, m_sharevar);
}

void Ril_Thread::deleteScripts()
{
  std::list<Ril*>::iterator it = m_scripts.begin();

  for (; it != m_scripts.end(); ++it) delete (*it);
  m_scripts.clear();
}

void Ril_Thread::deleteScript(std::list<Ril*>::iterator &it)
{
  delete (*it);
   it = m_scripts.erase(it);
}

Ril* Ril_Thread::newScript(void)
{
  Ril *script = new Ril(&m_logger, this);

  return script;
}

Ril* Ril_Thread::loadScript(const char *file)
{
  Ril *script = newScript();

  setlocale(LC_CTYPE, "");
  if (0 == script->load(file))
  {
    delete script;
    return NULL;
  }

  ril_set2array(script->m_vm, NULL, m_sharevar, "share");

  script->execute(Event("", ""));

  m_scripts.push_back(script);

  return script;
}

/* Ril_Thread::loadAndStart: load FST and start thread */
int Ril_Thread::loadAndStart(MMDAgent *mmdagent, const char *file)
{
  if (mmdagent == NULL) return -1;

  m_mmdagent = mmdagent;

  mmdagent->getWindowSize(&m_screenWidth, &m_screenHeight);
  m_renderer.setup(mmdagent);
  m_logger.setup(this);

  /* create OpenGL context */
  m_hDC = wglGetCurrentDC();
  m_drawRC = wglGetCurrentContext();
  m_imgRC = wglCreateContext(m_hDC);
  wglShareLists(m_drawRC, m_imgRC);

  /* change window procedure */
  originalWndProc = GetWindowLong(WindowFromDC(m_hDC), GWL_WNDPROC);
  SetWindowLong(WindowFromDC(m_hDC), GWL_WNDPROC, (LONG)wndProc);

  if (NULL == loadScript(file)) return -1;
  m_isRunning = true;

  /* start thread */
  glfwInit();
  m_mutex = glfwCreateMutex();
  m_cond = glfwCreateCond();
  m_thread = glfwCreateThread(mainThread, this);
  if (m_mutex == NULL || m_cond == NULL || m_thread < 0)
  {
     clear();
     return -1;
  }

  return 0;
}

/* Ril_Thread::stopAndRelease: stop thread and release */
void Ril_Thread::stopAndRelease()
{
   clear();
}

/* Ril_Thread::isRunning: check running */
bool Ril_Thread::isRunning()
{
   return m_isRunning;
}

void Ril_Thread::run()
{
  m_kill = false;

  for (;;)
  {
    /* wait transition event */
    if (m_events.empty())
    {
      glfwLockMutex(m_mutex);
      while (m_events.empty())
      { 
        if (0 < m_timers.size())
        {
          Ril_Timer &timer = m_timers.front();

          double waitTime = timer.endtime - MMDAgent_getTime();
          if (0 < waitTime) glfwWaitCond(m_cond, m_mutex, waitTime / 1000);

          if (timer.endtime <= MMDAgent_getTime())
          {
            Ril_Timer timer  = m_timers.front();
            m_timers.pop_front();
            timer.script->setCurrentThread(timer.thread);
            timer.thread->start();
            timer.thread->execute();
          }
        }
        else
        {
          glfwWaitCond(m_cond, m_mutex, GLFW_INFINITY);
        }
        if (m_kill == true) return;
      }
      glfwUnlockMutex(m_mutex);
    }

    std::list<Ril*>::iterator it = m_scripts.begin();
    Event e = m_events.front();
    while (it != m_scripts.end())
    {
      (*it)->execute(e);
      if (!(*it)->isRunning())
      {
        deleteScript(it);
        continue;
      }
      ++it;
    }
    m_events.pop();
  }
}

/* Ril_Thread::enqueueBuffer: enqueue buffer to check */
void Ril_Thread::enqueueBuffer(const char *type, const char *args)
{
  /* wait buffer */
  glfwLockMutex(m_mutex);

  m_events.push(new Event(type, args));

  /* start state transition thread */
  if (m_events.size() <= 1) glfwSignalCond(m_cond);

   /* release buffer */
   glfwUnlockMutex(m_mutex);
}

void Ril_Thread::addTimer(double sec, Ril *script)
{
  double endtime = MMDAgent_getTime() + sec * 1000;

  std::list<Ril_Timer>::iterator it = m_timers.begin();
  for (; it != m_timers.end(); ++it)
  {
    if (endtime < (*it).endtime) break;
  }
  m_timers.insert(it, Ril_Timer(endtime, script, script->currentThread()));
  script->currentThread()->stop();
}

void Ril_Thread::deleteTimer(Thread *thread)
{
  std::list<Ril_Timer>::iterator it = m_timers.begin();
  while (it != m_timers.end())
  {
    if ((*it).thread == thread)
    {
      it = m_timers.erase(it);
      continue;
    }
    ++it;
  }
}

void Ril_Thread::render()
{
  m_renderer.render();
}

void Ril_Thread::renderLog()
{
  m_logger.render();
}

int Ril_Thread::getMouseState(int type)
{
  if (type >= MOUSE_BUTTON_SIZE) return -1;
  if (mouseStates[type] == BUTTON_PRESS)
  {
    mouseStates[type]  = BUTTON_HOLD;
    return BUTTON_PRESS;
  }
  return mouseStates[type];
}

void Ril_Thread::enableStandardOperation()
{
  stopStandardOperation = false;
}

void Ril_Thread::disableStandardOperation()
{
  stopStandardOperation = true;
}

