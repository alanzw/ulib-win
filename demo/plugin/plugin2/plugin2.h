#ifndef U_PLIUGIN_2_H
#define U_PLIUGIN_2_H

#include <string>

#include "uplugin.h"
/*
 * =====================================================================================
 *        Class:  UPlugin
 *  Description:  The wapper for wondows plugin
 * =====================================================================================
 */
class ULIB_API UPlugin : public IPlugin
{
public:
    /* ====================  LIFECYCLE     ======================================= */
    UPlugin();                             /* constructor */
    ~UPlugin();
    /* ====================  OPERATORS     ======================================= */

    /* ====================  OPERATIONS    ======================================= */
    virtual const char * getName() const;
    virtual void setName(const char *name);
    //
    virtual IPluginHost getHost() const;
    virtual void setHost(const IPluginHost &iph);
    //
    virtual void go();

    /* ====================  ACCESS        ======================================= */

    /* ====================  INQUIRY       ======================================= */

    /* ====================  DATA MEMBERS  ======================================= */
protected:

private:
    std::string m_name;
    IPluginHost m_iph;
}; /* -----  end of class  UPlugin  ----- */

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
 
ULIB_API IPlugin * retrieveIPP();
ULIB_API void freeIPP();

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif // U_PLIUGIN_2_H

