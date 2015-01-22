// Recursive Templater
//
// (c) Paul D. Senzee
// Senzee 5
// http://senzee.blogspot.com

#include "evaluator.h"

std::string Evaluator::m_lookup(const std::string &key)
{
   ostream_t   result;
   std::string u;

   const std::vector<std::string> &data = (*m_data)[key];

   if (data.empty())
       return "";

   result << data[0];

   Evaluator ev;
   return ev.eval(std::string(result.str()), m_data);
}

std::string Evaluator::m_key()
{
   char ch;
   ostream_t result;
   while (m_it != m_end && (ch = *m_it++) != END)
   {
       if (ch == START) result << m_bracketed();
       else             result << ch;
   }
   return m_lookup(result.str());
}

std::string Evaluator::m_eval()
{
   if (!m_data || m_it == m_end)
       return "";

   char ch;
   ostream_t result;
   while (m_it != m_end)
   {
       ch = *m_it++;
       if (ch == START) result << m_bracketed();
       else             result << ch;
   }

   return result.str();
}

std::string Evaluator::m_reeval(char terminal)
{
   Evaluator ev;
   return ev.eval(m_until(terminal), m_data);
}

std::string Evaluator::m_literal()
{
   return m_until(LITERAL);
}

std::string Evaluator::m_until(char terminal)
{
   char         ch;
   ostream_t    result;
   int          state = 0;

   while (m_it != m_end)
   {
       ch = *m_it++;

       if (state == 0 && ch == terminal)
           state = 1;
       else if (state == 1)
       {
           if (ch == END)
               break;
           else if (ch == terminal)
           {
               state = 1;
               result << ch;
           }
           else
           {
               state = 0;
               result << terminal;
               result << ch;
           }
       }
       else result << ch;
   }

   return result.str();
}

std::string Evaluator::m_bracketed()
{
   // this method called when the prior character is m_chStart
   ostream_t result;

   char ch = *m_it;

   if (ch == KEY)
   {
       m_it++;
       result << m_key();
   }
   else if (ch == LITERAL)
   {
       m_it++;
       result << m_literal();
   }
   else
   {
       // take care of the normal case of ..[...].. with no special symbols
       //  as it is likely to appear in the plaintext.
       result << (char)START;
       while (m_it != m_end &&
                 (ch = *m_it++) != END)
       {
           if (ch == START)
               result << m_bracketed();
           else result << ch;
       }
       if (ch == END)
           result << ch;
   }

   return result.str();
}