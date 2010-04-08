#ifndef U_TREE_H
#define U_TREE_H

#include "ubinary_tree.h"

namespace huys
{

namespace ADT
{

enum _Color {
    _red,
    _black
};

template <class Entry>
struct RB_node
{
    Entry data;
    RB_node<Entry> *left;
    RB_node<Entry> *right;
    _Color color;



    RB_node(const Record &new_entry)
    {
        color = red;
        data = new_entry;
        left = right = NULL;
    }

    RB_node()
    {
        color = red;
        left = right = NULL;
    }

    void set_color(_Color c) { color = c; }

    Color get_color() const { return color; }
};

/*
 * red-black tree
 */
template <class TK, class TV, class TAlloc>
class URBTree
{

public:
    URBTree()
    {
    }

    ~URBTree()
    {
    }

};

}; // namespace ADT

}; // namespace huys

#endif // U_TREE_H
