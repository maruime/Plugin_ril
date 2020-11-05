#include "Object2D.h"

class Text : public Object2D
{
  std::string m_text;
  int m_fontSize;

  void initialize();
public:
  Text();
  ~Text();
  void setFontSize(const int size);
  void setText(const char *s);
  const char* text();

  bool isText() { return true; }

  void render(MMDAgent *mmdagent);
  int fontSize();
};