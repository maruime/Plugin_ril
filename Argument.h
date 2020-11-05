#include <vector>
#include <string>

class Argument
{
  bool compare(const Argument &args) const;
  void parse(const char *args);
public:
  std::vector<std::string> m_list;

  Argument();
  Argument(const char *args);
  void operator =(const char *args);
  bool operator ==(const Argument &args) const;
  bool operator !=(const Argument &args) const;
};

class ArgumentList
{
  bool compare(const ArgumentList &args) const;
  void parse(const char *args);
public:
  std::vector<Argument> m_list;

  ArgumentList();
  ArgumentList(const char *args);
  void operator =(const char *args);
  bool operator ==(const ArgumentList &args) const;
  bool operator !=(const ArgumentList &args) const;
  bool empty() const { return 0 == m_list.size(); }
};