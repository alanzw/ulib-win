#ifndef U_COMPILER_H
#define U_COMPILER_H

inline bool isWatcom()
{
#ifdef __WATCOMC__
    return true;
#else
    return false;
#endif
}

inline bool isBorland()
{
#ifdef __BORLANDC__
    return true;
#else
    return false;
#endif
}

inline bool isCygwin()
{
#ifdef __CYGWIN__
    return true;
#else
    return false;
#endif
}

inline bool isDigitalMars()
{
#ifdef  __DMC__
    return true;
#else
    return false;
#endif
}

inline bool isGCC()
{
#ifdef  __GNUC__
    return true;
#else
    return false;
#endif
}

inline bool isICC()
{
#ifdef  __INTEL_COMPILER
    return true;
#else
    return false;
#endif
}

inline bool isMSVC()
{
#ifdef  _MSC_VER
    return true;
#else
    return false;
#endif
}

#endif // U_COMPILER_H
