/*
 * =====================================================================================
 *
 *       Filename:  graph.h
 *
 *    Description:  graph
 *
 *        Version:  1.0
 *        Created:  2009/7/2 20:52:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef HU_GRAPH_H
#define HU_GRAPH_H

namespace huys
{

namespace adt
{

template <typename T>
class UGraph
{
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
};

}; // namespace adt

}; // namespace huys

#endif // HU_GRAPH_H

