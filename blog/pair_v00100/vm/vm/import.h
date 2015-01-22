// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _IMPORT_H
#define _IMPORT_H

#include <string>

#include "object.h"
#include "streams.h"

extern const Type type_import;

class Import : public GcObject
{
    std::string m_name;

public:

	inline Import(const std::string &name) : m_name(name) {}
	inline Import(const char *name)        : m_name(!name ? "" : name) {}

	const std::string    &name()                     const { return m_name; }
    IOutStream           &print(IOutStream &s)       const { s.write(m_name.data(), m_name.size()); return s; }
    const Type           &type()                     const { return type_import; }

    Import               *cast_import()                    { return this; }
    const Import         *cast_import()              const { return this; }
};

#endif  // _IMPORT_H