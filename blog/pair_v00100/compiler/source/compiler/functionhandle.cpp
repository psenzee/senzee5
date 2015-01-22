// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "functionhandle.h"
#include "streams.h"
#include "read_type.h"

void FunctionHandle::read(IInStream &s)
{
    id        = read_type<short>(s);
    stacksize = read_type<unsigned short>(s);
    localsize = read_type<unsigned short>(s);
    code      = read_prefix_str<unsigned short, std::string::value_type>(s);
}

void FunctionHandle::write(IOutStream &s) const
{
	write_type<short>(s, id);
    write_type<unsigned short>(s, stacksize);
    write_type<unsigned short>(s, localsize);
    write_prefix_str<unsigned short>(s, code);
}