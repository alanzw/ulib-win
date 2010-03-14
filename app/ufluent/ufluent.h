#ifndef U_FLUENT_H
#define U_FLUENT_H

namespace huys
{

enum Fluent_Section_Index {
    FLUENT_SX_COMMNET        = 0,  // Comment Section
    FLUENT_SX_HEADER         = 1,  // Header Section
    FLUENT_SX_DIMENSION      = 2,  // Dimensions Section
    FLUENT_SX_NODE           = 10,
    FLUENT_SX_PERIODIC_FACE  = 18,
    FLUENT_SX_FACE           = 12,
    FLUENT_SX_FACE_TREE      = 59,
    FLUENT_SX_CELL_TREE      = 58,
    FLUENT_SX_FACE_PARENTS   = 61,
    FLUENT_SX_ZONE           = 39,
    FLUENT_SX_PARTITION      = 40,
    FLUENT_SX_GRID_SIZE      = 33,
    FLUENT_SX_DATA_FIELD     = 300,
    FLUENT_SX_RESIDUALS      = 301
};

class UFluentReader
{
public:
    UFluentReader();

    ~UFluentReader();
private:

};

class UFluentScheme
{
public:

private:

};

}; // namespace huys

#endif // U_FLUENT_H
