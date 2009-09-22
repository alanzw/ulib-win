/*
 * =====================================================================================
 *
 *       Filename:  funcptr.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009-7-10 5:08:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */


#ifndef  U_FUNC_PTR_H
#define  U_FUNC_PTR_H

namespace huys
{

namespace ADT
{

class FuncPtr0_;
typedef FuncPtr0_ (*FuncPtr0)();
class FuncPtr0_
{
public:
  FuncPtr0_( FuncPtr0 p ) : p_( p ) { }
  operator FuncPtr0() { return p_; }
private:
  FuncPtr0 p_;
};

template <typename TRet>
struct Func0
{
    typedef TRet (*Function_Ptr)(void);
    typedef Function_Ptr type;
};

}; // namespace ADT

}; // namespace huys

#endif   // U_FUNC_PTR_H

