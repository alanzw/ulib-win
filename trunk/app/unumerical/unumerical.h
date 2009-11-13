/*
 *
 *   a \frac{\partial^2 u}{\partial x^2}
 * +2b \frac{\partial^2 u}{\partial x \partial y}
 * + c \frac{\partial^2 u}{\partial y^2}
 * + d \frac{\partial u}{\partial x}
 * + e \frac{\partial u}{\partial y}
 * + f u
 * + g
 * = 0
 */

/*
 8  5     2.0 6.0   2.0 2.0   4.0 4.0   4.0 1.0   6.0 5.0
 6.0 2.0  8.0 3.0   9.0 1.0    3    1 2 0   3  3 2 1
 3   5 2 3   4   6 4 2 5   3   7 6 5
 */

#include "adt/uvector.h"

class Vertex2D
{
public:

private:
    float x;
    float y;
};

class Element
{

};

typedef huys::ADT::UVector<Vertex2D> Node_table;
typedef huys::ADT::UVector<Element> Element_table;

class Mesh2D
{
public:

private:
    Node_table nodes;
    Element_table elements;
};

