// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

class Object;
class ScriptVm;

namespace function
{

void register_common_functions(ScriptVm &svm);

Object *wait(Object **objects, unsigned count);
Object *ask(Object **objects, unsigned count);
Object *concat(Object **objects, unsigned count);
Object *string(Object **objects, unsigned count);

Object *open_out_file(Object **objects, unsigned count);
Object *open_in_file(Object **objects, unsigned count);

Object *open_out_string(Object **objects, unsigned count);

Object *standard_out(Object **objects, unsigned count);
Object *standard_error(Object **objects, unsigned count);

Object *write(Object **objects, unsigned count);
Object *flush(Object **objects, unsigned count);
Object *close(Object **objects, unsigned count);

}

#endif  // _FUNCTIONS_H