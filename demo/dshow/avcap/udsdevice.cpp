#include "udsdevice.h"

UDSDevice::UDSDevice()
: m_nType(DT_Unknown),
  m_sFriendlyName(""),
  m_sDislayName("")
{}

UDSDevice::~UDSDevice()
{}

UDSDevice & UDSDevice::operator = (const CDSDevice & device)
: m_nType(device.m_nType),
  m_sFriendlyName(device.m_sFriendlyName),
  m_sDisplayName(m_sDisplayName)
{

}