/*
 * =====================================================================================
 *
 *       Filename:  test_sysadm.cpp
 *
 *    Description:  A little kit for system admin
 *
 *        Version:  1.0
 *        Created:  2009-8-9 2:17:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <shlobj.h>
#include "exdisp.h"

#include "udlgapp.h"

#include "usysadm.h"

UDLGAPP_T(UDialogSysAdm, IDD_SYSADM);


#ifndef _MSC_VER
#define   INITGUID
#include <initguid.h>
//13709620-C279-11CE-A49E-444553540000 
DEFINE_GUID(CLSID_Shell,0x13709620,0xc279,0x11ce,0xa4,0x9e,0x44,0x45,0x53,0x54,0x00,0x00);
#endif