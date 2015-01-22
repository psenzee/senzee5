// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "unify.h"
#include "object.h"
#include "pairutil.h"
#include "strings.h"
#include <map>
#include <string>

/*

Unifier
Common Lisp version by Mark Kantrowitz
Java and C++ version by Paul D. Senzee

*/

/*
//////////////////////////////////////////////////////////////////

;;; ****************************************************************
;;; Unification Algorithm ******************************************
;;; ****************************************************************
;;;
;;; Random implementations of unification.
;;;
;;; Written by Mark Kantrowitz, mkant@cs.cmu.edu, October 15, 1990.

// Ported to Java/C++ by Paul D. Senzee

// PAUL'S NOTE: NO OCCURENCE CHECK, INFINITE LOOPS POSSIBLE

(defvar *failure* 'failed)

(defun xor (a b)
  (or (and a (not b))
      (and b (not a))))

(defun nor (a b)
  (and (not a) (not b)))

(defun variablep (item)
  "A variable is of the form ?name, where name is a symbol."
  (and (symbolp item)
       (char= (char (symbol-name item) 0)
              #\?)))

;;; ********************************
;;; Iterative Unfication ***********
;;; ********************************
(defun unify (pattern data &optional env trace)

  "This is a fast iterative implementation of unification. It eliminates
   the need for a stack in a manner similar to tail-recursion. We model the
   flow of control in unification by saving untested pattern and data elements
   on a \"continuation stack\". At any point of the program, we are either
   updating the iteration variables or testing a pattern element against
   a data element (which must then be either atoms or variables). If this
   test fails, we return *failure* immediately. Otherwise, we accumulate
   any substitutions in the environment, which will ultimately be returned."

  (let ((rest-pattern nil)		; these act as continuations
            (rest-data    nil)
             binding)
    (loop
     (when trace
       ;; For debugging.
       (format t "~&Pattern:~T~A  ~&Data:~T~A  ~&Environment:~T~A"
               pattern data env))
     (cond ((or (and pattern (atom pattern))
                (and data (atom data)))
            ;; We have a pattern and a data to match, at least one
            ;; of which is a non-nil atom.
            (cond ((eq pattern data)
                   ;; If pattern and data are identical, test next elements.
                   (setf data nil pattern nil))
                  ;; Note: we aren't doing any sort of occurrence check
                  ;; to see if variable lookup will lead to infinite
                  ;; loops. For example, (?a ?b) against (?b ?a), or
                  ;; even ?a against (b ?a).
                  ((variablep data)
                   ;; Lookup the variable, if possible.
                   (setf binding (assoc data env))
                   (if binding
                       ;; If there's a data binding, substitute and try again.
                       (setf data (cdr binding))
                     ;; If no data binding, add one and move on.
                     (setf env (acons data pattern env)
                           data nil pattern nil)))
                  ((variablep pattern)
                   (setf binding (assoc pattern env))
                   (if binding
                       (setf pattern (cdr binding))
                     (setf env (acons pattern data env)
                           data nil pattern nil)))
                  (t
                   ;; Match failed. Probably because of data-pattern mismatch.
                   (return *failure*))))
           ((nor pattern data)
            ;; If we've run out of pattern and data (both nil), check the
            ;; rest-pattern and rest-data.
            (cond ((xor rest-pattern rest-data)
                   ;; If we have a mismatch, fail.
                   (return  *failure*))
                  ((nor rest-pattern rest-data)
                   ;; If we've run out there too, exit with the bindings.
                   (return env))
                  (t
                   ;; Otherwise, pop from the remainder to get the next pair.
                   (setf pattern (pop rest-pattern))
                   (setf data    (pop rest-data)))))
           ((and (listp pattern) (listp data))
            ;; We have two lists, one of which isn't nil.
            ;; Break it apart into bite-size chunks.
            (push (rest pattern) rest-pattern)
            (setf pattern (first pattern))
            (push (rest data) rest-data)
            (setf data (first data)))))))

*/

static bool isvar(Object *a)
{
    /*
    Symbol *symbol = 0;
    if (a && (symbol = a->cast_symbol()))
    {
        std::string &s = symbol->str();
        return !s.empty() && s.data()[0] == '?';
    }
    return false;
    */
    return a && a->cast_match();
}

static Object *pop(Object **p)
{
    Pair *pp = 0;
    if (!*p || !(pp = (*p)->cast_pair()))
        return 0;
    Object *u = pp->head;
    *p = pp->tail;
    return u;
}
    
static void push(Object **p, Object *e)
{
    *p = PairUtil::pair(e, *p);
}

static Pair *succeeded_no_substitutions()
{
    return PairUtil::pair(PairUtil::pair(Types::create_true(), Types::create_true()), 0);
}

Object *Unifier::unify(Object *pattern, Object *data)
{
    Object *rest_pattern = 0,
           *rest_data    = 0,
           *bindings     = 0;
    while (true)
    {
        if ((pattern && !pattern->cast_pair()) || (data && !data->cast_pair()))
        {
            if (Object::equal(pattern, data))
                data = pattern = 0;
            else if (isvar(data))
            {
                Pair *bound = PairUtil::findassoc(bindings, data);
                if (bound)
                    data = bound->tail;
                else
                {
                    push(&bindings, PairUtil::pair(data, pattern));
                    data = pattern = 0;
                }
            }
            else if (isvar(pattern))
            {
                Pair *bound = PairUtil::findassoc(bindings, pattern);
                if (bound)
                    pattern = bound->tail;
                else
                {
                    push(&bindings, PairUtil::pair(pattern, data));
                    data = pattern = 0;
                }
            }
            else return 0;
        }
        else if (!pattern && !data)
        {
            if ((rest_pattern || rest_data) && !(rest_pattern && rest_data))
                return 0;
            else if (!rest_pattern && !rest_data)
                return bindings ? bindings : succeeded_no_substitutions();
            else
            {
                pattern = pop(&rest_pattern);
                data    = pop(&rest_data);
            }
        }
        else if ((!pattern || PairUtil::cast_pair(pattern)) && (!data || PairUtil::cast_pair(data)))
        {
            push(&rest_pattern, PairUtil::tail(pattern));
            pattern = PairUtil::head(pattern);
            push(&rest_data, PairUtil::tail(data));
            data = PairUtil::head(data);
        }
    }   
}