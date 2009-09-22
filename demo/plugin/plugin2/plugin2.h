#ifndef U_PLIUGIN_2_H
#define U_PLIUGIN_2_H

#include "uplugin.h"
/*
 * =====================================================================================
 *        Class:  UPlugin
 *  Description:  The wapper for wondows plugin
 * =====================================================================================
 */
class EXPORT UPlugin : public IPlugin
{
public:
    /* ====================  LIFECYCLE     ======================================= */
    UPlugin();                             /* constructor */
    ~UPlugin();
    /* ====================  OPERATORS     ======================================= */

    /* ====================  OPERATIONS    ======================================= */
    virtual string getName() const;
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
    string m_name;
    IPluginHost m_iph;
}; /* -----  end of class  UPlugin  ----- */

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
 
EXPORT IPlugin * retrieveIPP();

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif // U_PLIUGIN_2_H

