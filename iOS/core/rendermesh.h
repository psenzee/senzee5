#ifndef _RENDERMESH_H
#define _RENDERMESH_H

#include "geometry.h"

#include <string>
#include <vector>

class RenderMeshSection
{
public:

    std::string name,
                material;
    Geometry    geometry;

    RenderMeshSection() {}
    RenderMeshSection(const Geometry &geometry) : geometry(geometry) {}
    unsigned vertex_count() const { return geometry.vertex_count(); }
};

class RenderMesh
{
public:	
	std::vector<RenderMeshSection> sections;
};

#endif  // _RENDERMESH_H_