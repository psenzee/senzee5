// Recursive Templater
//
// (c) Paul D. Senzee
// Senzee 5
// http://senzee.blogspot.com

#ifndef _TEMPLATER_H
#define _TEMPLATER_H

#include "evaluator.h"

/**
 * <p>Templater</p>
 *
 * <p>
 * Recursive template engine with escaping and lazy evaluation. Note that templater is case SENSITIVE.
 *
 * Use:
 *  1. Add items to Templater's map.
 *         templater.map()["bob"] = "joe";
 *  2. Evaluate a text item that references a map item.
 *         s = templater.eval("well, hello [!bob]");
 *         Returns "well, hello joe" in s;
 * </p>
 *
 * @author         Paul D. Senzee
 * @version        1.0
 *
 * TODO make this stream oriented (InputStream, etc..)
 */

#include <map>
#include <string>
#include <vector>
#include <sstream>

class Templater
{
public:

   typedef std::map<std::string, std::vector<std::string> > map_t;
   typedef std::string::iterator                            iterator_t;
   typedef std::ostringstream                               ostream_t;

   inline Templater(map_t &data) : m_data(data) {}

   inline map_t       &map()                       { return m_data; }
   inline std::string  get(const std::string &k)   { Evaluator ev; return ev.key (std::string(k), &m_data); }
   inline std::string  get(const char *k)          { return get(std::string(!k ? "" : k)); }
   inline std::string  eval(const std::string &s)  { Evaluator ev; return ev.eval(std::string(s), &m_data); }
   inline std::string  eval(const char *s)         { return eval(std::string(!s ? "" : s)); }

private:

   map_t &m_data;
};

#endif  // _TEMPLATER_H

