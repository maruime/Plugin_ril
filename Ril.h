#ifndef __RIL_H__
#define __RIL_H__

#include "MMDAgent.h"
#include "Ril_Logger.h"
#include "Renderer.h"
#include "Argument.h"

#include <ril.h>
#include <vector>
#include <map>
#include <string>
#include <list>

#define VIMANAGER_RECOG_EVENT_STOP "RECOG_EVENT_STOP"

class Ril_Thread;
typedef struct Listener Listener;
typedef struct Thread Thread;
typedef std::list<Listener> ListenerList;
typedef std::list<Thread> ThreadList;

class Event {
public:
  std::string m_type;
  ArgumentList m_args;

  Event()
  {
  }
  Event(const char *type, const char *args)
  {
    m_type = type;
    m_args = args;
  }
  void operator =(const Event &e)
  {
    m_type = e.m_type;
    m_args = e.m_args;
  }
  bool operator ==(const Event &e) const
  {
    return (m_type == e.m_type && m_args == e.m_args);
  }
  bool empty() const
  {
    return m_type.empty();
  }
  void clear()
  {
    m_type.clear();
  }
};

struct Thread
{
    int stopCounter;
    std::vector<Event> waitEvents;
public:
    ril_state_t *state;
    std::string alias;
    bool isWait;
    bool kill;

    Thread();
    bool hasAnyOneEvents(const Event &e);
    bool hasWaitEvent();
    void addWaitEvent(const char *type, const char *args);
    Event& addWaitEvent();
    int execute();
    void stop();
    void start();
    bool isStopped();
};

class Ril
{
  Ril_Logger *m_logger;
  Ril_Thread *m_thread;
  RILVM m_vm;
  ThreadList m_threads;
  Thread *m_currentThread;
  std::map<std::string, ListenerList> m_listeners;
  int m_listenerStopCounter;
  bool m_isRunning;
public:
  friend Ril_Thread;
  Ril(Ril_Logger* logger, Ril_Thread *thread);
  ~Ril();
  Ril_Thread* parent();
  MMDAgent* mmdagent();
  Ril_Logger* logger();
  Renderer* renderer();
  bool load(const char *file);
  void addconstvarbyinteger(const char *name, int value);
  void addconstvarbystring(const char *name);
  void deleteThreads(bool soft = false);
  void deleteThread(const char *alias, bool soft = false);
  void deleteThread(ThreadList::iterator &it);
  void deleteListeners();
  Thread* newThread(const char *alias);
  void addListener(const char *alias, const char *tagname, const char *type, const char *args, const int limit, const bool attention);
  void startListeners();
  void stopListeners();
  bool isStopListeners();
  void startThreads();
  void stopThreads();
  void stopThreads(const char *alias);
  void stopOtherThreads(const char *alias);
  void setCurrentThread(Thread *thread);
  Thread* currentThread() { return m_currentThread; };
  void checkListeners(const Event &e);
  void setArgumentsByEvent(const Event &e);
  void execute(const Event &e);
  bool hasAnyOneEvents(const std::vector<Event> &eList, const Event &e);
  void kill();
  bool isRunning();
};

#endif
