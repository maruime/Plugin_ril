#include "MMDAgent.h"
#include "Ril_Thread.h"
#include "Ril.h"

struct Listener {
    bool isWait;
    int limit;
    ril_tag_t *tag;
    std::string alias;
    std::string type;
    ArgumentList args;
    ~Listener() {
    }
};

#include "Tags.hpp"

Thread::Thread()
{
  isWait = false;
  kill = false;
}

bool Thread::hasAnyOneEvents(const Event &e)
{
  std::vector<Event>::const_iterator it = waitEvents.begin();
  for (; it != waitEvents.end(); ++it)
  {
    if ((*it).m_type == e.m_type)
    {
      if ((*it).m_args.empty() || (*it).m_args == e.m_args) return true;
    }
  }

  return false;
}

bool Thread::hasWaitEvent()
{
  return 0 < waitEvents.size();
}

void Thread::addWaitEvent(const char *type, const char *args)
{
  Event &e = addWaitEvent();
  e.m_type = type;
  e.m_args = args;
}

Event& Thread::addWaitEvent()
{
  waitEvents.resize(waitEvents.size() + 1);
  return waitEvents.back();
}

int Thread::execute()
{
  ril_setstate(state);
  return ril_execute(ril_state2vm(state));
}

void Thread::stop()
{
  ++stopCounter;
}

void Thread::start()
{
  if (0 >= stopCounter) return;
  --stopCounter;
}

bool Thread::isStopped()
{
  return 0 < stopCounter;
}

static void errorhandler(RILVM vm, const char *msg)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->logger()->print("Ril: %s", msg);
}

void Ril::addconstvarbyinteger(const char *name, int value)
{
  ril_var_t *var;
  var = ril_createvar(m_vm, NULL, name);
  ril_setinteger(m_vm, var, value);
  ril_setconst(var);
}

void Ril::addconstvarbystring(const char *name)
{
  ril_var_t *var;
  var = ril_createvar(m_vm, NULL, name);
  ril_setstring(m_vm, var, name);
  ril_setconst(var);
}

Ril::Ril(Ril_Logger *logger, Ril_Thread *thread)
{
  m_isRunning = true;
  m_logger = logger;
  m_thread = thread;

  m_vm = ril_open();
  ril_setuserdata(m_vm, this);
  ril_seterrorhandler(m_vm, errorhandler);

  addconstvarbystring(MMDAGENT_EVENT_MODELADD);
  addconstvarbystring(MMDAGENT_EVENT_MODELDELETE);
  addconstvarbystring(MMDAGENT_EVENT_MODELCHANGE);
  addconstvarbystring(MMDAGENT_EVENT_MOTIONADD);
  addconstvarbystring(MMDAGENT_EVENT_MOTIONDELETE);
  addconstvarbystring(MMDAGENT_EVENT_MOTIONCHANGE);
  addconstvarbystring(MMDAGENT_EVENT_MOVESTART);
  addconstvarbystring(MMDAGENT_EVENT_MOVESTOP);
  addconstvarbystring(MMDAGENT_EVENT_TURNSTART);
  addconstvarbystring(MMDAGENT_EVENT_TURNSTOP);
  addconstvarbystring(MMDAGENT_EVENT_ROTATESTART);
  addconstvarbystring(MMDAGENT_EVENT_ROTATESTOP);
  addconstvarbystring(MMDAGENT_EVENT_LIPSYNCSTART);
  addconstvarbystring(MMDAGENT_EVENT_LIPSYNCSTOP);
  addconstvarbystring(MMDAGENT_EVENT_DRAGANDDROP);
  addconstvarbystring(MMDAGENT_EVENT_KEY);
  addconstvarbystring("RECOG_EVENT_START");
  addconstvarbystring(VIMANAGER_RECOG_EVENT_STOP);
  addconstvarbystring("TIMER_EVENT_START");
  addconstvarbystring("TIMER_EVENT_STOP");
  addconstvarbystring("SYNTH_EVENT_START");
  addconstvarbystring("SYNTH_EVENT_STOP");
  addconstvarbystring("GLOBAL");
  addconstvarbystring("LOCAL");
  addconstvarbystring("LOOP");
  addconstvarbystring("ONCE");
  addconstvarbystring("PART");
  addconstvarbystring("FULL");
  addconstvarbystring("ON");
  addconstvarbystring("OFF");
  addconstvarbyinteger("SCREEN_WIDTH", m_thread->screenWidth());
  addconstvarbyinteger("SCREEN_HEIGHT", m_thread->screenHeight());
  addconstvarbyinteger("MOUSE_BUTTON_LEFT", MOUSE_BUTTON_LEFT);
  addconstvarbyinteger("MOUSE_BUTTON_RIGHT", MOUSE_BUTTON_RIGHT);
  addconstvarbyinteger("MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE);
  addconstvarbyinteger("BUTTON_PRESS", BUTTON_PRESS);
  addconstvarbyinteger("BUTTON_RELEASE", BUTTON_RELEASE);
  addconstvarbyinteger("BUTTON_HOLD", BUTTON_HOLD);

  ril_tag_t *tag;
  RIL_REGISTERTAG(m_vm, model_add, "alias, file, x, y, z, rx = 0, ry = 0, rz = 0");
  RIL_REGISTERTAG(m_vm, model_add, "alias, file, x, y, z, rx = 0, ry = 0, rz = 0, parent");
  RIL_REGISTERTAG(m_vm, model_add, "alias, file, x, y, z, rx = 0, ry = 0, rz = 0, parent, bone");
  RIL_REGISTERTAG(m_vm, model_change, "alias, file");
  RIL_REGISTERTAG(m_vm, model_delete, "alias");
  RIL_REGISTERTAG(m_vm, motion_add, "model, alias, file, frame = FULL, repeat = LOOP, smooth = ON, relocation = ON");
  RIL_REGISTERTAG(m_vm, motion_change, "model, alias, file");
  RIL_REGISTERTAG(m_vm, motion_delete, "model, alias");
  RIL_REGISTERTAG(m_vm, move_start, "alias, x = 0, y = 0, z = 0, base = LOCAL, speed");
  RIL_REGISTERTAG(m_vm, move_stop, "alias");
  RIL_REGISTERTAG(m_vm, turn_start, "alias, x = 0, y = 0, z = 0, base = LOCAL, speed");
  RIL_REGISTERTAG(m_vm, turn_stop, "alias");
  RIL_REGISTERTAG(m_vm, rotate_start, "alias, x = 0, y = 0, z = 0, base = LOCAL, speed");
  RIL_REGISTERTAG(m_vm, rotate_stop, "alias");
  RIL_REGISTERTAG(m_vm, stage, "file");
  RIL_REGISTERTAG(m_vm, lipsync_start, "alias, time");
  RIL_REGISTERTAG(m_vm, lipsync_stop, "alias");
  RIL_REGISTERTAG(m_vm, camera, "x, y, z, rx, ry, rz, distance, fovy, time");
  ril_registertag(m_vm, "camera", "file", RIL_CALLFUNC(camera_motion));
  RIL_REGISTERTAG(m_vm, timer_start, "alias, value");
  RIL_REGISTERTAG(m_vm, timer_stop, "alias");
  RIL_REGISTERTAG(m_vm, execute, "file");
  RIL_REGISTERTAG(m_vm, key_post, "class, key, shift = ON, ctrl = ON, alt = ON");
  tag = RIL_REGISTERTAG(m_vm, synth, "model, voice, wait = true");
  ril_setpairtag(tag, RIL_REGISTERTAG(m_vm, endsynth, NULL));
  RIL_REGISTERTAG(m_vm, synth_stop, "model");

  tag = ril_getregisteredtag(m_vm, "ch", "value");
  ril_setexecutehandler(tag, RIL_CALLFUNC(ch));
  tag = ril_getregisteredtag(m_vm, "r", NULL);
  ril_setexecutehandler(tag, RIL_CALLFUNC(r));

  tag = RIL_REGISTERTAG(m_vm, log, NULL);
  ril_useworkarea(tag, true);
  ril_setpairtag(tag, RIL_REGISTERTAG(m_vm, endlog, NULL));
  RIL_REGISTERTAG(m_vm, log_fontsize, "size");
  RIL_REGISTERTAG(m_vm, log_bgcolor, "red, green, blue, alpha");
  RIL_REGISTERTAG(m_vm, log_color, "red, green, blue, alpha");
  RIL_REGISTERTAG(m_vm, log_position, "x, y");
  RIL_REGISTERTAG(m_vm, log_size, "width, height");

  RIL_REGISTERTAG(m_vm, sendcommand, "type, args = \"\"");
  RIL_REGISTERTAG(m_vm, sendevent, "type, args = \"\"");
  RIL_REGISTERTAG(m_vm, listen, "alias = \"\", call, event, args = \"\", limit = 0, wait = true");
  RIL_REGISTERTAG(m_vm, listen_delete, NULL);
  RIL_REGISTERTAG(m_vm, listen_start, NULL);
  RIL_REGISTERTAG(m_vm, listen_stop, NULL);
  tag = RIL_REGISTERTAG(m_vm, thread, "alias = \"\"");
  ril_setpairtag(tag, RIL_REGISTERTAG(m_vm, endthread, NULL));
  RIL_REGISTERTAG(m_vm, thread_delete, NULL);
  RIL_REGISTERTAG(m_vm, thread_delete, "alias");
  RIL_REGISTERTAG(m_vm, thread_start, NULL);
  RIL_REGISTERTAG(m_vm, thread_stop, NULL);
  RIL_REGISTERTAG(m_vm, thread_stop, "alias, isnot=false");
  RIL_REGISTERTAG(m_vm, rand, "var, min, max = 0");
  tag = RIL_REGISTERTAG(m_vm, wait, "time = 0");
  /*
  ril_setchildtag(tag, ril_registertag(m_vm, "event", "type, args = \"\"", RIL_CALLFUNC(wait_event)));
  ril_setpairtag(tag, RIL_REGISTERTAG(m_vm, endwait, NULL));
  */
  RIL_REGISTERTAG(m_vm, waitevent, "type, args = \"\"");
  RIL_REGISTERTAG(m_vm, sin, "degree, &var");
  RIL_REGISTERTAG(m_vm, cos, "degree, &var");
  RIL_REGISTERTAG(m_vm, log_clear, NULL);

  RIL_REGISTERTAG(m_vm, image, "alias, file, x = 0, y = 0, opacity = 1, layer = null, clipx = 0, clipy = 0");
  RIL_REGISTERTAG(m_vm, image, "alias, file, x = 0, y = 0, opacity = 1, layer = null, clipx = 0, clipy = 0, clipwidth, clipheight");
  RIL_REGISTERTAG(m_vm, image_getfile, "alias");
  RIL_REGISTERTAG(m_vm, image_clip, "alias, x, y");
  RIL_REGISTERTAG(m_vm, image_clip, "alias, x, y , width, height");
  ril_registertag(m_vm, "image_delete", NULL, RIL_CALLFUNC(image_deleteall));
  ril_registertag(m_vm, "image_delete", "alias = \"\"", RIL_CALLFUNC(object_delete));
  ril_registertag(m_vm, "image_color", "alias, red = -1, green = -1, blue = -1, alpha = -1", RIL_CALLFUNC(object_color));
  ril_registertag(m_vm, "image_move", "alias, x = 0, y = 0, absolute = false", RIL_CALLFUNC(object_move));
  ril_registertag(m_vm, "image_show", "alias", RIL_CALLFUNC(object_show));
  ril_registertag(m_vm, "image_hide", "alias", RIL_CALLFUNC(object_hide));
  ril_registertag(m_vm, "image_getposition", "alias, &x, &y", RIL_CALLFUNC(object_getposition));
  ril_registertag(m_vm, "image_size", "alias, width = null, height = null", RIL_CALLFUNC(object_size));
  RIL_REGISTERTAG(m_vm, image_getsize, "alias, &width, &height");
  ril_registertag(m_vm, "image_scale", "alias, x = null, y = null", RIL_CALLFUNC(object_scale));

  tag = RIL_REGISTERTAG(m_vm, textbox, "alias, layer = null, fontsize = 18");
  ril_useworkarea(tag, true);
  ril_setpairtag(tag, RIL_REGISTERTAG(m_vm, endtextbox, NULL));
  ril_registertag(m_vm, "textbox_delete", NULL, RIL_CALLFUNC(textbox_deleteall));
  ril_registertag(m_vm, "textbox_delete", "alias = \"\"", RIL_CALLFUNC(object_delete));
  ril_registertag(m_vm, "textbox_color", "alias, red = -1, green = -1, blue = -1, alpha = -1", RIL_CALLFUNC(object_color));
  ril_registertag(m_vm, "textbox_move", "alias, x = 0, y = 0, absolute = false", RIL_CALLFUNC(object_move));
  ril_registertag(m_vm, "textbox_show", "alias", RIL_CALLFUNC(object_show));
  ril_registertag(m_vm, "textbox_hide", "alias", RIL_CALLFUNC(object_hide));
  ril_registertag(m_vm, "textbox_getposition", "alias, &x, &y", RIL_CALLFUNC(object_getposition));
  RIL_REGISTERTAG(m_vm, textbox_getsize, "alias, &width, &height");
  ril_registertag(m_vm, "textbox_scale", "alias, x = null, y = null", RIL_CALLFUNC(object_scale));

  RIL_REGISTERTAG(m_vm, dir, "path = \"\", ext = \"*\"");
  RIL_REGISTERTAG(m_vm, isdir, "path");
  RIL_REGISTERTAG(m_vm, script_add, "file, alias = \"\"");
  RIL_REGISTERTAG(m_vm, kill, NULL);
  RIL_REGISTERTAG(m_vm, mouse_position, "&x, &y");
  RIL_REGISTERTAG(m_vm, mouse_button, "type");
  RIL_REGISTERTAG(m_vm, stdinput_enable, NULL);
  RIL_REGISTERTAG(m_vm, stdinput_disable, NULL);

  m_listenerStopCounter = 0;
}

Ril::~Ril()
{
  deleteListeners();
  deleteThreads();
  ril_close(m_vm);
}

Ril_Thread* Ril::parent() { return m_thread; }
MMDAgent* Ril::mmdagent() { return m_thread->mmdagent(); };
Ril_Logger* Ril::logger() { return m_logger; };
Renderer* Ril::renderer() { return m_thread->renderer(); };

bool Ril::load(const char *file)
{
  if (RIL_FAILED(ril_loadfile(m_vm, file))) return false;

  newThread("main");
  
  return true;
}

void Ril::deleteThreads(bool soft)
{
  ThreadList::iterator it = m_threads.begin();
  while (it != m_threads.end())
  {
    deleteThread(it);
  }
}

void Ril::deleteThread(const char *alias, bool soft)
{
  ThreadList::iterator it = m_threads.begin();
  while (it != m_threads.end())
  {
    if ((*it).alias != alias)
    {
      ++it;
      continue;
    }
    if (soft)
    {
      (*it).kill = true;
      ++it;
      continue;
    }
    deleteThread(it);
  }
}

void Ril::deleteThread(ThreadList::iterator &it)
{
  mmdagent()->sendEventMessage("THREAD_EVENT_DELETE", "%s", (*it).alias.c_str());
  /* delete ril_state_t */
  ril_deletestate((*it).state);
  /* delete booked timers */
  parent()->deleteTimer(&(*it));
  if ((*it).isWait) startListeners();
  it = m_threads.erase(it);
}

void Ril::deleteListeners()
{
  m_listeners.clear();
}

Thread* Ril::newThread(const char *alias)
{ 
  Thread thread;

  thread.state = ril_newstate(m_vm);
  thread.alias = alias;
  thread.stopCounter = 0;

  m_threads.push_back(thread);

  return &m_threads.back();
}

void Ril::addListener(const char *alias, const char *tagname, const char *type, const char *args, const int limit, const bool wait)
{
  ril_tag_t *tag = ril_getregisteredtag(m_vm, tagname, "args");
  if (NULL == tag) tag = ril_getregisteredtag3(m_vm, tagname, NULL, false, false);
  if (NULL == tag)
  {
    m_logger->print("[listen] Failed searching the tag %s", tagname);
    m_logger->newLine();
    return;
  }

  m_listeners[type].resize(m_listeners[type].size() + 1);
  Listener &listener = m_listeners[type].back();

  listener.alias = alias;
  listener.tag = tag;
  listener.type = type;
  listener.args = args;
  listener.limit = limit;
  listener.isWait = wait;
}

void Ril::startListeners()
{
  if (0 >= m_listenerStopCounter) return;
  --m_listenerStopCounter;
}

void Ril::stopListeners()
{
  ++m_listenerStopCounter;
}

bool Ril::isStopListeners()
{
  return 0 < m_listenerStopCounter;
}

void Ril::startThreads()
{
  ThreadList::iterator it = m_threads.begin();
  for (; it != m_threads.end(); ++it)
  {
    if (0 >= (*it).stopCounter) continue;
    (*it).stop();
  }
}

void Ril::stopThreads()
{
  ThreadList::iterator it = m_threads.begin();
  for (; it != m_threads.end(); ++it)
  {
    (*it).stop();
  }
}

void Ril::stopThreads(const char *alias)
{
  ThreadList::iterator it = m_threads.begin();
  for (; it != m_threads.end(); ++it)
  {
    if ((*it).alias != alias) continue;
    (*it).stop();
  }
}

void Ril::stopOtherThreads(const char *alias)
{
  ThreadList::iterator it = m_threads.begin();
  for (; it != m_threads.end(); ++it)
  {
    if ((*it).alias == alias) continue;
    (*it).stop();
  }
}

void Ril::checkListeners(const Event &e)
{
  ListenerList &list = m_listeners[e.m_type];
  ListenerList::iterator it = list.begin();
  Thread *thread;

  for (; it != list.end(); ++it)
  {
    Listener &listener = *it;
    
    if (!listener.args.empty() && listener.args != e.m_args) continue;

    thread = newThread(listener.alias.c_str());
    if (listener.isWait)
    {
      stopListeners();
      thread->isWait = true;
    }
    m_currentThread = thread;
    ril_setstate(thread->state);
    setArgumentsByEvent(e);
    ril_callmacro(m_vm, listener.tag);
    if (0 < listener.limit)
    {
      --listener.limit;
      if (0 < listener.limit) continue;
      it = list.erase(it);
      --it;
    }
  }
}

void Ril::setArgumentsByEvent(const Event &e)
{
  int i, k;
  const ArgumentList &argList = e.m_args;

  ril_cleararguments(ril_getstate(m_vm));
  ril_var_t *parent = ril_getargument(m_vm, 0), *var;

  const std::vector<Argument>args = argList.m_list;
  std::vector<Argument>::const_iterator it = args.begin();
  for (i = 0; it != args.end(); ++it, ++i)
  {
    const std::vector<std::string> &args2 =(*it).m_list;
    std::vector<std::string>::const_iterator it2 =args2.begin();
    var = ril_createvar(m_vm, parent, NULL);
    for (k = 0; it2 != args2.end(); ++it2, ++k)
    {
      ril_setstring(m_vm, ril_createvar(m_vm, var, NULL), (*it2).c_str());
    }
  }
}

void Ril::execute(const Event &e)
{
  ThreadList::iterator it = m_threads.begin();
  while (it != m_threads.end())
  {
    /* 削除の予約がされていた場合 */
    if ((*it).kill)
    {
      deleteThread(it);
      continue;
    }
    setCurrentThread(&(*it));

    /* イベント待機のチェック */
    if (m_currentThread->hasWaitEvent())
    {
      if (m_currentThread->hasAnyOneEvents(e))
      {
        m_currentThread->waitEvents.clear();
      }
      else
      {
        ++it;
        continue;
      }
    }
    if (m_currentThread->isStopped())
    {
      ++it;
      continue;
    }
    if (RIL_EXIT == m_currentThread->execute())
    {
      deleteThread(it);
      continue;
    }
    ++it;
  }

  if (!isStopListeners())
  {
    checkListeners(e);
  }

  if (!m_listeners.size() && !m_threads.size())
  {
    kill();
  }
}

void Ril::setCurrentThread(Thread *thread)
{
  m_currentThread = thread;
}

void Ril::kill()
{
  m_isRunning = false;
}

bool Ril::isRunning()
{
  return m_isRunning;
}