#include "Argument.h"
#include <ctype.h>

#define VIMANAGER_SEPARATOR1       '|'
#define VIMANAGER_SEPARATOR2       ','

Argument::Argument()
{
  m_list.clear();
}

Argument::Argument(const char *args)
{
  m_list.clear();
  parse(args);
}

void Argument::parse(const char *args)
{
  const char *begin;

  for (; VIMANAGER_SEPARATOR1 != *args;)
  {
    while (isspace((unsigned char)*args) || VIMANAGER_SEPARATOR2 == *args) { args += mblen(args, MB_CUR_MAX); }
    begin = args;
    while (!iscntrl((unsigned char)*args) && VIMANAGER_SEPARATOR1 != *args &&
      VIMANAGER_SEPARATOR2 != *args) { args += mblen(args, MB_CUR_MAX); }
    if (begin >= args) break;

    m_list.resize(m_list.size() + 1);
    m_list.back().assign(begin, args - begin);
  }
}

void Argument::operator =(const char *args)
{
  parse(args);
}

bool Argument::compare(const Argument &args) const
{
  std::vector<std::string>::const_iterator it = m_list.begin();

  for (; it != m_list.end(); ++it)
  {
    std::vector<std::string>::const_iterator it2 = args.m_list.begin();
    for (; it2 != args.m_list.end(); ++it2)
    {
      if (*it == *it2) return true;
    }
  }

  return false;
}

bool Argument::operator ==(const Argument &args) const
{
  return compare(args);
}

bool Argument::operator !=(const Argument &args) const
{
  return !compare(args);
}

ArgumentList::ArgumentList()
{
  m_list.clear();
}

ArgumentList::ArgumentList(const char *args)
{
  m_list.clear();
  parse(args);
}

void ArgumentList::parse(const char *args)
{
  const char *begin;

  for (;;)
  {
    while (isspace((unsigned char)*args) || VIMANAGER_SEPARATOR1 == *args)
    {
      args += mblen(args, MB_CUR_MAX);
    }
    begin = args;
    while (!iscntrl((unsigned char)*args) && VIMANAGER_SEPARATOR1 != *args)
    {
      args += mblen(args, MB_CUR_MAX);
    }
    if (begin >= args) break;

    m_list.push_back(begin);
  }
}

void ArgumentList::operator =(const char *args)
{
  parse(args);
}

bool ArgumentList::compare(const ArgumentList &args) const
{
  std::vector<Argument>::const_iterator it = m_list.begin();
  std::vector<Argument>::const_iterator it2 = args.m_list.begin();

  if (args.m_list.size() != m_list.size()) return false;

  for (; it !=m_list.end(); ++it, ++it2)
  {
    if (*it != *it2) return false;
  }

  return true;
}

bool ArgumentList::operator ==(const ArgumentList &args) const
{
  return compare(args);
}

bool ArgumentList::operator !=(const ArgumentList &args) const
{
  return !compare(args);
}