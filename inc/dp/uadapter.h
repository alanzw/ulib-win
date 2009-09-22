
/*
 * =====================================================================================
 *
 *       Filename:  uadapter.h
 *
 *    Description:  adapter
 *
 *        Version:  1.0
 *        Created:  2009-7-22 16:41:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_ADAPTER_H
#define U_ADAPTER_H

namespace huys
{

namespace dp
{

namespace structual
{

//
class XXCircle
{
public:
   void displayIt(char *pszStr)
   {

   };
};

class Shape
{
public:
   virtual void display(char *pStr, int nNO) = 0;
};

class Circle
{
public:
    Circle()
    {
        xxcc = new XXCircle();
    }
    ~Circle()
    {
        delete xxcc;
    }
   void display(char *pStr, int nNO)
   {
      xxcc->displayIt(pStr);
   }
private:
   XXCircle *xxcc;
};

}; // namespace structual

}; // namespace dp

}; // namespace huys

#endif // ADAPTER_H_
