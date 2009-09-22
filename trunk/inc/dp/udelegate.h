#ifndef U_DELEGATE_H
#define U_DELEGATE_H

namespace huys
{

namespace dp
{

namespace utility
{

/// Delegate
class Delegate {
public:
      virtual void invoke()=0;
protected:
      Delegate(){}
      virtual ~Delegate(){}
};

/// NonTypeDelegate
class NonTypeDelegate : public Delegate
{
public:
   typedef void (*PFunc)(int);
public:
   void invoke()
   {
        m_pfn(m_iParam);
   }
   //
   NonTypeDelegate(PFunc pfn, int iParam)
   :m_pfn(pfn), m_iParam(iParam)
   {}
   //
   virtual ~NonTypeDelegate(){}
private:
   PFunc m_pfn;
   int m_iParam;
};

/// TypeDelegate
template <typename T>
class TypeDelegate : public Delegate
{
public:
    typedef void (T::*TPFunc)(int);
public:
   void invoke();
   TypeDelegate(const T &t, TypeDelegate<T>::TPFunc pfn, int iParam);
   ~TypeDelegate(){}
private:
   T m_t;
   void (T::*m_pfn)(int);
   int m_iParam;
};

template <typename T>
TypeDelegate<T>::TypeDelegate(const T &t, TypeDelegate<T>::TPFunc pfn, int iParam )
: m_t(t), m_pfn(pfn), m_iParam(iParam)
{}

template <typename T>
void TypeDelegate<T>::invoke()
{
   // cout << "TypeDelegate Invoke\r\n";
   (m_t.*m_pfn)(m_iParam);
}

}; // namespace utility

}; // namespace dp

}; // namespace huys


#endif // U_DELEGATE_H

