#ifndef U_DX_STL_H
#define U_DX_STL_H

/*==================================================================
STL file format:

An StL (¡°StereoLithography¡±) file is a triangular representation of
a 3-dimensional surface geometry. The surface is tessellated or
broken down logically into a series of small triangles (facets).
Each facet is described by a perpendicular direction and three
points representing the vertices (corners) of the triangle. These
data are used by a slicing algorithm to determine the cross sections
of the 3-dimensional shape to be built by the fabber.

An StL file consists of a list of facet data. Each facet is uniquely
identified by a unit normal (a line perpendicular to the triangle and
with a length of 1.0) and by three vertices (corners). The normal and
each vertex are specified by three coordinates each, so there is a total
of 12 numbers stored for each facet.

Facet orientation. The facets define the surface of a 3-dimensional object.
As such, each facet is part of the boundary between the interior and the
exterior of the object. The orientation of the facets (which way is ¡°out¡±
and which way is ¡°in¡±) is specified redundantly in two ways which must be
consistent. First, the direction of the normal is outward. Second, the
vertices are listed in counterclockwise order when looking at the object
from the outside (right-hand rule). 

Vertex-to-vertex rule. Each triangle must share two vertices with each of
its adjacent triangles. In other words, a vertex of one triangle cannot
lie on the side of another.


ASCII:

solid name
facet normal ni nj nk
   outer loop
     vertex v1x v1y v1z
     vertex v2x v2y v2z
     vertex v3x v3y v3z'
   endloop
 endfacet
endsolid name



BINARY:

UINT8[80]         -   Header
UINT32            -   Number of triangles
foreach triangle
  REAL32[3]       -    Normal vector
  REAL32[3]       -    Vertex 1
  REAL32[3]       -    Vertex 2
  REAL32[3]       -    Vertex 3
  UINT16          -    Attribute byte count
end

===================================================================*/
struct UVertex3D {
    double x;
    double y;
    double z;
};

struct UFacet {
    UVertex3D nv;
    UVertex3D v[3];
};

enum USTL_type {UST_ascci, UST_binary};



#endif // U_DX_STL_H
