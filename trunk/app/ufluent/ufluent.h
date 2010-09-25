#ifndef U_FLUENT_H
#define U_FLUENT_H

#include "adt/uvector.h"

namespace huys
{

enum Fluent_Section_Index {
    FLUENT_SX_COMMNET        = 0,  // Comment Section
    FLUENT_SX_HEADER         = 1,  // Header Section
    FLUENT_SX_DIMENSION      = 2,  // Dimensions Section
    FLUENT_SX_MACHINE_CONFIG = 4,
    FLUENT_SX_NODE           = 10,
    FLUENT_SX_EDGE           = 11,
    FLUENT_SX_CELL           = 12,
    FLUENT_SX_FACE           = 13,
    FLUENT_SX_PERIODIC_FACE  = 18,
    FLUENT_SX_FACE_TREE      = 59,
    FLUENT_SX_CELL_TREE      = 58,
    FLUENT_SX_FACE_PARENTS   = 61,
    FLUENT_SX_RP_VAR         = 37, // Rampant variables
    FLUENT_SX_ZONE           = 39,
    FLUENT_SX_ZONE2          = 45,
    FLUENT_SX_PARTITION      = 40,
    FLUENT_SX_GRID_SIZE      = 33,
    FLUENT_SX_DOMAIN_VAR     = 64, // Domain variables
    FLUENT_SX_DATA_FIELD     = 300,
    FLUENT_SX_RESIDUALS      = 301,
    
};

struct FluentNode {
    int zone_id;
    int first_idx;
    int last_idx;
    int type;
    int nd;
};

struct FluentData
{
    int type;
    union {
        char* text;
        int value;
        void* data;
    };
};

typedef huys::ADT::UVector<FluentData> FluentDataArray;

class UFluentReader
{
public:
    UFluentReader();

    ~UFluentReader();
    
    bool read(const char *filename);
private:
    FluentDataArray _fda;
};

class UFluentScheme
{
public:

private:

};

}; // namespace huys

#endif // U_FLUENT_H
