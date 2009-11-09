#ifndef U_DSDEVICE_H
#define U_DSDEVICE_H

#include <list>
#include <string>

using std::string;

#include "global.h"

class UDSDevice
{
private:
    Device_Type m_nType;
    string m_sFriendlyName;
    string m_sDisplayName;
public:
    UDSDevice();
    ~UDSDevice();
    UDSDevice & operator = (const UDSDevice &);
    //
    void setDeviceType(Device_Type iType);
    Device_Type getDeviceType() const;
    //
    BOOL isDVDevice();
    //
    void setDeviceFriendlyName(const char * sName);
    string getDeviceFriendlyName();
    //
    void setDeviceDisplayName(const char * sName);
    string getDeviceDisplayName();
};

typedef list<UDSDevice &> DEVICE_LIST;

#endif // U_DSDEVICE_H
