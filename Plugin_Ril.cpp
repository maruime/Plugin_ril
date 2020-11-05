/* definitions */

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif /* _WIN32 */

#define PLUGINRIL_NAME "Ril"

/* headers */

#include <locale.h>
#include "MMDAgent.h"
#include "Ril_Thread.h"

/* variables */

static Ril_Thread ril_thread;
static bool enable;
static bool enable_log;

/* extAppStart: load FST and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
   char *buf;
   int len;

   enable_log = false;
   buf = MMDAgent_strdup(mmdagent->getConfigFileName());
   len = MMDAgent_strlen(buf);
   if (len > 4) {
      buf[len-4] = '.';
      buf[len-3] = 'r';
      buf[len-2] = 'i';
      buf[len-1] = 'l';
      enable_log = 0 > ril_thread.loadAndStart(mmdagent, buf);
   }
   if(buf) free(buf);

   enable = true;
   mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINRIL_NAME);
}

/* extProcCommand: process command message */
EXPORT void extProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{
   if (enable == true) {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
         if(MMDAgent_strequal(args, PLUGINRIL_NAME)) {
            enable = false;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINDISABLE, PLUGINRIL_NAME);
         }
      }
   } else {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
         if(MMDAgent_strequal(args, PLUGINRIL_NAME)) {
            enable = true;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINRIL_NAME);
         }
      }
   }
}

/* extProcEvent: process event message */
EXPORT void extProcEvent(MMDAgent *mmdagent, const char *type, const char *args)
{
   if (!enable) return;

    if (ril_thread.isRunning() && type != NULL)
    {
      ril_thread.enqueueBuffer(type, args); /* enqueue */
    }
    if (MMDAgent_strequal(type, MMDAGENT_EVENT_KEY) && MMDAgent_strequal(args, "F"))
    {
       enable_log = !enable_log;
    }
}

/* extRender: render log */
EXPORT void extRender(MMDAgent *mmdagent)
{
   if (!enable) return;
   ril_thread.render();
   if (enable_log) ril_thread.renderLog();
}

/* extAppEnd: stop and free thread */
EXPORT void extAppEnd(MMDAgent *mmdagent)
{
   ril_thread.stopAndRelease();
}
