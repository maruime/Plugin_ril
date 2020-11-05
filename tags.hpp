#include <time.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

static RIL_FUNC(ch, vm)
{
   Ril *ril = (Ril*)ril_userdata(vm);

  ril->logger()->print(ril_getstring(vm, 0));

  return RIL_BREAKPAIR;
}

static RIL_FUNC(r, vm)
{
   Ril *ril = (Ril*)ril_userdata(vm);

  ril->logger()->newLine();

  return RIL_BREAKPAIR;
}

static RIL_FUNC(log, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  ril_tag_t *tag;

  tag = ril_getregisteredtag2(vm, RIL_TAG_CH);
  ril_pushfunction(vm, tag, RIL_CALLFUNC(ch), NULL, NULL, NULL, NULL);
  tag = ril_getregisteredtag2(vm, RIL_TAG_R);
  ril_pushfunction(vm, tag, RIL_CALLFUNC(r),  NULL, NULL, NULL, NULL);

  return RIL_NEXT;
}

static RIL_FUNC(endlog, vm)
{
  ril_popfunction(vm, ril_popfunction(vm, ril_workarea(vm)));

  return RIL_BREAKPAIR;
}

static RIL_FUNC(log_fontsize, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->logger()->setFontSize(ril_getfloat(vm, 0));

  return RIL_NEXT;
}

static RIL_FUNC(log_bgcolor, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->logger()->setBGColor(ril_getfloat(vm, 0), ril_getfloat(vm, 1), ril_getfloat(vm, 2), ril_getfloat(vm, 3));

  return RIL_NEXT;
}

static RIL_FUNC(log_color, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->logger()->setBGColor(ril_getfloat(vm, 0), ril_getfloat(vm, 1), ril_getfloat(vm, 2), ril_getfloat(vm, 3));

  return RIL_NEXT;
}

static RIL_FUNC(log_position, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->logger()->setPosition(ril_getinteger(vm, 0), ril_getinteger(vm, 1));

  return RIL_NEXT;
}

static RIL_FUNC(log_size, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->logger()->setSize(ril_getinteger(vm, 0), ril_getinteger(vm, 1));

  return RIL_NEXT;
}

static RIL_FUNC(sendcommand, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  ril->mmdagent()->sendCommandMessage(ril_getstring(vm, 0), ril_getstring(vm, 1));

  return RIL_NEXT;
}

static RIL_FUNC(sendevent, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
   ril->mmdagent()->sendEventMessage(ril_getstring(vm, 0), ril_getstring(vm, 1));

  return RIL_NEXT;
}

static RIL_FUNC(model_add, vm)
{
  static char buf[MMDAGENT_MAXBUFLEN];
  Ril *ril = (Ril*)ril_userdata(vm);

  sprintf(
      buf,
      "%s|%s|%f,%f,%f",
      ril_getstring(vm, 0),
      ril_getstring(vm, 1),
      ril_getfloat(vm, 2),
      ril_getfloat(vm, 3),
      ril_getfloat(vm, 4)
  );

  if (ril_has(vm, 5))
  {
    sprintf(
      buf + strlen(buf),
      "|%f,%f,%f",
      ril_getfloat(vm, 5),
      ril_getfloat(vm, 6),
      ril_getfloat(vm, 7)
    );
  }

  if (ril_has(vm, 8))
  {
    sprintf(
      buf + strlen(buf),
      "|%s",
      ril_getstring(vm, 8)
    );
  }

  if (ril_has(vm, 9))
  {
    sprintf(
      buf + strlen(buf),
      "|%s",
      ril_getstring(vm, 9)
    );
  }

  ril->mmdagent()->sendCommandMessage(MMDAGENT_COMMAND_MODELADD, buf);

  return RIL_NEXT;
}

static RIL_FUNC(model_change, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_MODELCHANGE,
    "%s|%s",
    ril_getstring(vm, 0),
    ril_getstring(vm, 1)
  );

  return RIL_NEXT;
}

static RIL_FUNC(model_delete, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_MODELDELETE,
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(motion_add, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_MOTIONADD,
    "%s|%s|%s|%s|%s|%s|%s",
    ril_getstring(vm, 0),
    ril_getstring(vm, 1),
    ril_getstring(vm, 2),
    ril_getstring(vm, 3),
    ril_getstring(vm, 4),
    ril_getstring(vm, 5),
    ril_getstring(vm, 6)
  );

  return RIL_NEXT;
}

static RIL_FUNC(motion_change, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_MOTIONCHANGE,
    "%s|%s|%s",
    ril_getstring(vm, 0),
    ril_getstring(vm, 1),
    ril_getstring(vm, 2)
  );

  return RIL_NEXT;
}

static RIL_FUNC(motion_delete, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_MOTIONDELETE,
    "%s|%s",
    ril_getstring(vm, 0),
    ril_getstring(vm, 1)
  );

  return RIL_NEXT;
}

static RIL_FUNC(move_start, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_MOVESTART,
    "%s|%f,%f,%f|%s|%f",
    ril_getstring(vm, 0),
    ril_getfloat(vm, 1),
    ril_getfloat(vm, 2),
    ril_getfloat(vm, 3),
    ril_getstring(vm, 4),
    ril_getfloat(vm, 5)
  );

  return RIL_NEXT;
}

static RIL_FUNC(move_stop, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_MOVESTOP,
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(turn_start, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_TURNSTART,
    "%s|%f,%f,%f|%s|%f",
    ril_getstring(vm, 0),
    ril_getfloat(vm, 1),
    ril_getfloat(vm, 2),
    ril_getfloat(vm, 3),
    ril_getstring(vm, 4),
    ril_getfloat(vm, 5)
  );

  return RIL_NEXT;
}

static RIL_FUNC(turn_stop, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_TURNSTOP,
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(rotate_start, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_ROTATESTART,
    "%s|%f,%f,%f|%s|%f",
    ril_getstring(vm, 0),
    ril_getfloat(vm, 1),
    ril_getfloat(vm, 2),
    ril_getfloat(vm, 3),
    ril_getstring(vm, 4),
    ril_getfloat(vm, 5)
  );

  return RIL_NEXT;
}

static RIL_FUNC(rotate_stop, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_ROTATESTOP,
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(stage, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_STAGE,
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(lipsync_start, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_LIPSYNCSTART,
    "%s|%d",
    ril_getstring(vm, 0),
    ril_getinteger(vm, 1)
  );

  return RIL_NEXT;
}

static RIL_FUNC(lipsync_stop, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_LIPSYNCSTOP,
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(camera, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_CAMERA,
    "%f,%f,%f|%f,%f,%f|%s|%s|%f",
    ril_getfloat(vm, 0),
    ril_getfloat(vm, 1),
    ril_getfloat(vm, 2),
    ril_getfloat(vm, 3),
    ril_getfloat(vm, 4),
    ril_getfloat(vm, 5),
    ril_getstring(vm, 6),
    ril_getstring(vm, 7),
    ril_getfloat(vm, 8)
  );

  return RIL_NEXT;
}

static RIL_FUNC(camera_motion, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    MMDAGENT_COMMAND_CAMERA,
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(timer_start, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    "TIMER_START",
    "%s|%s",
    ril_getstring(vm, 0),
    ril_getstring(vm, 1)
  );

  return RIL_NEXT;
}

static RIL_FUNC(timer_stop, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    "TIMER_STOP",
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(execute, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    "EXECUTE",
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(key_post, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    "KEY_POST",
    "%s|%s|%s|%s|%s",
    ril_getstring(vm, 0),
    ril_getstring(vm, 1),
    ril_getstring(vm, 2),
    ril_getstring(vm, 3),
    ril_getstring(vm, 4)
  );

  return RIL_NEXT;
}

static RIL_FUNC(endsynth, vm) { return RIL_NEXT; }
static RIL_FUNC(synth, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  bool wait = ril_getbool(vm, 2);
  static const ril_signature_t TAG_END = ril_signature(ril_getregisteredtag(vm, "endsynth", NULL));

  if (wait)
  {
    ril->currentThread()->addWaitEvent("SYNTH_EVENT_STOP", ril_getstring(vm, 0));
  }

  ril_buffer_t *buf = ril_buffer_open(1, 1024);
  ril_buffer_write(buf, ril_getstring(vm, 0), strlen(ril_getstring(vm, 0)));
  ril_buffer_write(buf, "|", 1);
  ril_buffer_write(buf, ril_getstring(vm, 1), strlen(ril_getstring(vm, 1)));
  ril_buffer_write(buf, "|", 1);
  for (;;)
  {
    ril_nextcmd(vm);
    ril_cmdid_t cmd = ril_cmd(vm);
    if (RIL_ISTAG(vm, cmd, TAG_END))
    {
      break;
    }
    if (RIL_ISTAG(vm, cmd, RIL_TAG_CH))
    {
      ril_setarguments(vm, cmd);
      const char *str = ril_getstring(vm, 0);
      while ('\0' != *str)
      {
        ril_buffer_write(buf, str, 1);
        ++str;
      }
      continue;
    }
    if (RIL_ISTAG(vm, cmd, RIL_TAG_R))
    {
      *(char*)ril_buffer_malloc(buf, 1) = '\n';
      continue;
    }
  }
  *(char*)ril_buffer_malloc(buf, 1) = '\0';

  ril->mmdagent()->sendCommandMessage(
    "SYNTH_START",
    (const char*)ril_buffer_front(buf)
  );

  ril_buffer_close(buf);

  if (!wait)
  {
    return RIL_NEXT;
  }

  ril_nextcmd(vm);

  return RIL_STOP;
}

static RIL_FUNC(synth_stop, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->mmdagent()->sendCommandMessage(
    "SYNTH_STOP",
    "%s",
    ril_getstring(vm, 0)
  );

  return RIL_NEXT;
}

static RIL_FUNC(listen, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->addListener(ril_getstring(vm, 0), ril_getstring(vm, 1), ril_getstring(vm, 2), ril_getstring(vm, 3), ril_getinteger(vm, 4), ril_getbool(vm, 5));

  return RIL_NEXT;
}

static RIL_FUNC(listen_start, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->startListeners();

  return RIL_NEXT;
}

static RIL_FUNC(listen_delete, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  if (ril_has(vm, 0)) {
    /* 未実装 */
  }
  else {
    ril->deleteListeners();
  }

  return RIL_NEXT;
}

static RIL_FUNC(listen_stop, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->stopListeners();

  return RIL_NEXT;
}

static RIL_FUNC(endthread, vm) { return RIL_NEXT; }
static RIL_FUNC(thread, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  char alias[128];
  Thread *currentThread = ril->currentThread();
  ril_state_t *state = ril_getstate(vm);
  static const ril_signature_t TAG_END = ril_signature(ril_getregisteredtag(vm, "endthread", NULL));

  strcpy(alias, ril_getstring(vm, 0));

  for (;;)
  {
    ril_nextcmd(vm);
    ril_cmdid_t cmd = ril_cmd(vm);
    if (RIL_ISTAG(vm, cmd, TAG_END)) break;

    ril->setCurrentThread(ril->newThread(alias));

    ril_setarguments(vm, cmd);
    ril_copyarguments(vm, ril->currentThread()->state);
    ril_setstate(ril->currentThread()->state);
    ril_callmacro(vm, ril_cmdid2tag(vm, cmd));

    ril->setCurrentThread(currentThread);
    ril_setstate(state);
  }

  return RIL_NEXT;
}

static RIL_FUNC(thread_delete, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  if (ril_has(vm, 0)) {
    ril->deleteThread(ril_getstring(vm, 0), true);
  }
  else {
    ril->deleteThreads(true);
  }

  return RIL_NEXT;
}

static RIL_FUNC(thread_start, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->startThreads();

  return RIL_NEXT;
}

static RIL_FUNC(thread_stop, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  int argc = ril_countarguments(vm);

  if (0 == argc)
  {
    ril->stopOtherThreads(ril->currentThread()->alias.c_str());
    return RIL_NEXT;
  }

  if (ril_getbool(vm, 1))
  {
    ril->stopOtherThreads(ril_getstring(vm, 0));
  }
  else
  {
    ril->stopThreads(ril_getstring(vm, 0));
  }

  return RIL_NEXT;
}

static RIL_FUNC(wait, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->parent()->addTimer(ril_getfloat(vm, 0), ril);
  ril_nextcmd(vm);

  return RIL_STOP;
}

static RIL_FUNC(endwait, vm)
{
  ril_nextcmd(vm);

  return RIL_STOP;
}

static RIL_FUNC(wait_event, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  ril->currentThread()->addWaitEvent(ril_getstring(vm, 0), ril_getstring(vm, 1));

  return RIL_NEXT;
}

static RIL_FUNC(waitevent, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  ril->currentThread()->addWaitEvent(ril_getstring(vm, 0), ril_getstring(vm, 1));

  ril_nextcmd(vm);

  return RIL_STOP;
}

#define PI 3.1415926

static RIL_FUNC(sin, vm)
{
  ril_var_t *var = ril_getargument(vm, 1);
  ril_setfloat(vm, var, sin(ril_getinteger(vm, 0) * PI / 180));

  return RIL_NEXT;
}

static RIL_FUNC(cos, vm)
{
  ril_var_t *var = ril_getargument(vm, 1);
  ril_setfloat(vm, var, cos(ril_getinteger(vm, 0) * PI / 180));

  return RIL_NEXT;
}

static RIL_FUNC(rand, vm)
{
  ril_var_t *var = ril_getargument(vm, 0);
  float min = ril_getfloat(vm, 1);
  float max = ril_getfloat(vm, 2);
  float tmp;

  if (max < min)
  {
    tmp = max;
    max = min;
    min = tmp;
   }
   ril_setfloat(vm, var, min + (max - min) * (rand() - 0.0f) * (1.0f / (RAND_MAX - 0.0f))); /* 0.0f is RAND_MIN */

   return RIL_NEXT;
}

static RIL_FUNC(log_clear, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->logger()->clear();

  return RIL_NEXT;
}

static RIL_FUNC(image, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);
  const char *file = ril_getstring(vm, 1);
  const int x = ril_getinteger(vm, 2);
  const int y = ril_getinteger(vm, 3);
  const GLfloat opacity = ril_getfloat(vm, 4);
  const int clipx = ril_getinteger(vm, 6);
  const int clipy = ril_getinteger(vm, 7);

  Image *image = ril->renderer()->getImage(alias);
  if (NULL == image || !image->isImage())
  {
    image = new Image;
    image->hide();
    ril->renderer()->addObject(alias, image);
    ril->renderer()->addRender(image, ril_getinteger(vm, 5));
  }
  else
  {
    if (!ril_isnull(ril_getargument(vm, 5))) ril->renderer()->addRender(image, ril_getinteger(vm, 5));
  }

  Ril_Thread *parent = ril->parent();
  wglMakeCurrent(parent->m_hDC, parent->m_imgRC);
  image->load(file);
  wglMakeCurrent(parent->m_hDC, parent->m_drawRC);
  image->setPosition(x, y);
  image->setClipPosition(clipx, clipy);
  image->setAlpha(opacity);
  if (ril_has(vm, 8))
  {
    image->setClipSize(ril_getinteger(vm, 8), ril_getinteger(vm, 9));
  }

  return RIL_NEXT;
}

static RIL_FUNC(image_getfile, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);
  Image *image = ril->renderer()->getImage(alias);

  if (NULL == image || !image->isImage())
  {
    return RIL_NEXT;
  }

  ril_returnstring(vm, image->file());

  return RIL_NEXT;
}

static RIL_FUNC(image_clip,vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);

  Image *image = ril->renderer()->getImage(alias);
  if (NULL == image || !image->isImage())
  {
    return RIL_NEXT;
  }

  image->setClipPosition(ril_getfloat(vm, 1), ril_getfloat(vm, 2));
  if (ril_has(vm, 3)) image->setClipSize(ril_getfloat(vm, 3), ril_getfloat(vm, 4));

  return RIL_NEXT;
}

static RIL_FUNC(image_getsize, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);
  Image *image = ril->renderer()->getImage(alias);

  if (NULL == image || !image->isImage())
  {
    return RIL_NEXT;
  }

  ril_setfloat(vm, ril_getargument(vm, 1), image->width());
  ril_setfloat(vm, ril_getargument(vm, 2), image->height());

  return RIL_NEXT;
}

static RIL_FUNC(image_deleteall, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->renderer()->deleteAllImages();

  return RIL_NEXT;
}

static RIL_FUNC(textbox_deleteall, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->renderer()->deleteAllTextBoxs();

  return RIL_NEXT;
}

typedef struct
{
  Text *textbox;
  std::string *buf;
} textbox_t;

static RIL_FUNC(textbox_ch, vm)
{
  textbox_t *workarea = (textbox_t*)ril_getshareddata(ril_currenttag(vm));

  *workarea->buf += ril_getstring(vm, 0);

  return RIL_NEXT;
}

static RIL_FUNC(textbox_r, vm)
{
  textbox_t *workarea = (textbox_t*)ril_getshareddata(ril_currenttag(vm));

  *workarea->buf += '\n';

  return RIL_NEXT;
}

static RIL_FUNC(textbox, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  std::string str;
  const char *alias = ril_getstring(vm, 0);
  const int fontSize =ril_getinteger(vm, 2);
  static const ril_signature_t TAG_END = ril_signature(ril_getregisteredtag(vm, "endtextbox", NULL));

  Text *textbox = ril->renderer()->getTextBox(alias);
  if (NULL == textbox || !textbox->isText())
  {
    textbox = new Text;
    textbox->hide();
    ril->renderer()->addObject(alias, textbox);
    ril->renderer()->addRender(textbox, ril_getinteger(vm, 1));
  }
  else
  {
    if (!ril_isnull(ril_getargument(vm, 1))) ril->renderer()->addRender(textbox, ril_getinteger(vm, 1));
  }
  textbox->setFontSize(fontSize);

  textbox_t *workarea = (textbox_t*)ril_mallocworkarea(vm, sizeof(textbox_t));
  ril_tag_t *tag;

  workarea->textbox = textbox;
  workarea->buf = new std::string();
  tag = ril_getregisteredtag2(vm, RIL_TAG_CH);
  ril_pushfunction(vm, tag, RIL_CALLFUNC(textbox_ch), NULL, NULL, NULL, workarea);
  tag = ril_getregisteredtag2(vm, RIL_TAG_R);
  ril_pushfunction(vm, tag, RIL_CALLFUNC(textbox_r),  NULL, NULL, NULL, workarea);

  return RIL_NEXT;
}

static RIL_FUNC(endtextbox, vm)
{
  textbox_t *workarea = (textbox_t*)ril_workarea(vm);

  workarea->textbox->setText(workarea->buf->c_str());
  delete workarea->buf;
  ril_popfunction(vm, ril_popfunction(vm, workarea + 1));

  return RIL_BREAKPAIR;
}

static RIL_FUNC(textbox_getsize, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);
  Text *textbox = ril->renderer()->getTextBox(alias);
  float width, height;

  if (NULL == textbox || !textbox->isText())
  {
    return RIL_NEXT;
  }

  const char *text = textbox->text();
  width = strlen(text) * textbox->fontSize() / 2;
  height = textbox->fontSize();

  ril_setfloat(vm, ril_getargument(vm, 1), width);
  ril_setfloat(vm, ril_getargument(vm, 2), height);

  return RIL_NEXT;
}

static RIL_FUNC(object_delete, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  ril->renderer()->deleteObject(ril_getstring(vm, 0));
  
  return RIL_NEXT;
}

static RIL_FUNC(object_color, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);
  const GLfloat red = ril_getfloat(vm, 1);
  const GLfloat green = ril_getfloat(vm, 2);
  const GLfloat blue = ril_getfloat(vm, 3);
  const GLfloat alpha = ril_getfloat(vm, 4);

  Object2D *obj = ril->renderer()->getObject(alias);
  if (NULL == obj) return RIL_NEXT;

  if (0 <= red)  obj->m_color.red = red;
  if (0 <= green)  obj->m_color.green = green;
  if (0 <= blue)  obj->m_color.blue = blue;
  if (0 <= alpha)  obj->m_color.alpha = alpha;

  return RIL_NEXT;
}

static RIL_FUNC(object_move, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  Object2D *obj = ril->renderer()->getObject(ril_getstring(vm, 0));
  if (NULL == obj) return RIL_NEXT;

  obj->setPosition(ril_getfloat(vm, 1), ril_getfloat(vm, 2), ril_getbool(vm, 3));

  return RIL_NEXT;
}

static RIL_FUNC(object_layer, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);
  int layer = ril_getinteger(vm, 1);

  Object2D *obj = ril->renderer()->getObject(alias);
  if (NULL == obj) return RIL_NEXT;

  ril->renderer()->addRender(obj, layer);

  return RIL_NEXT;
}

static RIL_FUNC(object_show, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);

  Object2D *obj = ril->renderer()->getObject(alias);
  if (NULL == obj) return RIL_NEXT;
  obj->show();

  return RIL_NEXT;
}

static RIL_FUNC(object_hide, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);

  Object2D *obj = ril->renderer()->getObject(alias);
  if (NULL == obj) return RIL_NEXT;
  obj->hide();

  return RIL_NEXT;
}

static RIL_FUNC(object_getposition, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);
  Object2D *object = ril->renderer()->getObject(alias);

  if (NULL == object)
  {
    return RIL_NEXT;
  }

  ril_setfloat(vm, ril_getargument(vm, 1), object->x());
  ril_setfloat(vm, ril_getargument(vm, 2), object->y());

  return RIL_NEXT;
}

static RIL_FUNC(object_size, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);
  Object2D *object = ril->renderer()->getObject(alias);

  if (NULL == object)
  {
    return RIL_NEXT;
  }

  if (!ril_isnull(ril_getargument(vm, 1))) object->setWidth(ril_getfloat(vm, 1));
  if (!ril_isnull(ril_getargument(vm, 2))) object->setHeight(ril_getfloat(vm, 2));

  return RIL_NEXT;
}

static RIL_FUNC(object_scale, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *alias = ril_getstring(vm, 0);
  Object2D *object = ril->renderer()->getObject(alias);

  if (NULL == object)
  {
    return RIL_NEXT;
  }

  if (!ril_isnull(ril_getargument(vm, 1))) object->setScaleX(ril_getfloat(vm, 1));
  if (!ril_isnull(ril_getargument(vm, 2))) object->setScaleY(ril_getfloat(vm, 2));

  return RIL_NEXT;
}

static RIL_FUNC(dir, vm)
{
  DIRECTORY *dp;
  char buf[MMDAGENT_MAXBUFLEN];
  const char *dir = ril_getstring(vm, 0);
  char ext[32];
  ril_var_t *parentvar = ril_newvar(vm), *var;

  ext[0] = '.';
  strcpy(ext + 1, ril_getstring(vm, 1));

  dp = MMDAgent_opendir(dir);
  if (dp != NULL)
  {
    while (MMDAgent_readdir(dp, buf))
    {
      if ('*' != ext[1] && !MMDAgent_strtailmatch(buf, ext)) continue;
      if ('.' == buf[0] && ('\0' == buf[1] || ('.' == buf[1] && '\0' == buf[2]))) continue;
      var = ril_createvar(vm, parentvar, NULL);
      ril_setstring(vm, var, buf);
    }
    MMDAgent_closedir(dp);
  }

  ril_return(vm, parentvar);
  ril_deletevar(vm, parentvar);

  return RIL_NEXT;
}

static RIL_FUNC(isdir, vm)
{
  const char *path  = ril_getstring(vm, 0);

	ril_returninteger(vm, PathIsDirectory(path));

  return RIL_NEXT;
}

static RIL_FUNC(script_add, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  const char *file = ril_getstring(vm, 0);

  ril->parent()->loadScript(file);

  return RIL_NEXT;
}

static RIL_FUNC(kill, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->kill();

  return RIL_EXIT;
}

static RIL_FUNC(mouse_position, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  int x, y, width, height;

  ril->mmdagent()->getMousePosition(&x, &y);
  ril->mmdagent()->getWindowSize(&width, &height);

  x = (double)x * ((double)ril->parent()->screenWidth() / width);
  y = (double)y * ((double)ril->parent()->screenHeight() / height);
  ril_setinteger(vm, ril_getargument(vm, 0), x);
  ril_setinteger(vm, ril_getargument(vm, 1), y);

  return RIL_NEXT;
}

static RIL_FUNC(mouse_button, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);
  int type = ril_getinteger(vm, 0);

  ril_returninteger(vm, ril->parent()->getMouseState(type));

  return RIL_NEXT;
}

static RIL_FUNC(stdinput_enable, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->parent()->enableStandardOperation();

  return RIL_NEXT;
}

static RIL_FUNC(stdinput_disable, vm)
{
  Ril *ril = (Ril*)ril_userdata(vm);

  ril->parent()->disableStandardOperation();

  return RIL_NEXT;
}