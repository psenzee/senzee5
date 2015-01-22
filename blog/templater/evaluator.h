// Recursive Templater
//
// (c) Paul D. Senzee
// Senzee 5
// http://senzee.blogspot.com

#ifndef _EVALUATOR_H
#define _EVALUATOR_H

#include <map>
#include <string>
#include <vector>
#include <sstream>

class Evaluator
{
   enum { START = '[', END = ']', KEY = '!', LITERAL = '`' };
    
   typedef std::map<std::string, std::vector<std::string> > map_t;
   typedef std::string::iterator                            iterator_t;
   typedef std::ostringstream                               ostream_t;

public:

   inline Evaluator() : m_data(0) {}

   inline std::string key(std::string &s, map_t *data)                         { return key(s.begin(), s.end(), data); }
   inline std::string eval(std::string &s, map_t *data)                        { return eval(s.begin(), s.end(), data); }
   inline std::string key(iterator_t begin, iterator_t end, map_t *data)
   {
       m_data = data;
       m_it   = begin;
       m_end  = end;
       return m_key();
   }
   inline std::string eval(iterator_t begin, iterator_t end, map_t *data)
   {
       m_data = data;
       m_it   = begin;
       m_end  = end;
       return m_eval();
   }

private:


   iterator_t                    m_it,
                                 m_end;

   map_t                        *m_data;

   std::string m_lookup(const std::string &key);
   std::string m_key(void);
   std::string m_eval(void);
   std::string m_reeval(char terminal);
   std::string m_literal(void);
   std::string m_until(char terminal);
   std::string m_bracketed(void);
};

#endif  // _EVALUATOR_H