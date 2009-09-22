//#define UNICODE
//#define _UNICODE

#include <windows.h>
#include <objbase.h>
#include <comcat.h>
#include <stdio.h>
#include <tchar.h>

//=====================================================================================
/*
|| ::DESCRIPTION::
|| This function will convert a WCHAR string to a CHAR string.
||
|| Param 1 :: Pointer to a buffer that will contain the converted string. Ensure this
|| buffer is large enough; if not, buffer overrun errors will occur.
|| Param 2 :: Constant pointer to a source WCHAR string to be converted to CHAR
*/
void wtoc(CHAR* Dest, const WCHAR* Source)
{
    int i = 0;

    while(Source[i] != '\0')
    {
        Dest[i] = (CHAR)Source[i];
        ++i;
    }
    Dest[i] = '\0';
}

//=====================================================================================
/*
|| ::DESCRIPTION::
|| This function will convert a CHAR string to a WCHAR string.
||
|| Param 1 :: Pointer to a buffer that will contain the converted string. Ensure this
|| buffer is large enough; if not, buffer overrun errors will occur.
|| Param 2 :: Constant pointer to a source CHAR string to be converted to WCHAR
*/
void ctow(WCHAR* Dest, const CHAR* Source)
{
    int i = 0;

    while(Source[i] != '\0')
    {
        Dest[i] = (WCHAR)Source[i];
        ++i;
    }
}


int main()
{
    //Initialise com libraries
    ::CoInitialize (NULL);

    //The Component Category Manager implemented by System
    //implements this interface
    ICatInformation *pCatInfo=NULL;

    //Create an instance of standard Component Category Manager
    HRESULT hr = ::CoCreateInstance( CLSID_StdComponentCategoriesMgr,
                                     NULL,
                                     CLSCTX_INPROC_SERVER,
                                     IID_ICatInformation,
                                     (void **)&pCatInfo );

    //Increase ref count on interface
    pCatInfo->AddRef ();

    //IEnumGUID interface provides enumerator for
    //enumerating through the collection of com objects
    IEnumGUID *pEnumGUID=NULL;

    //We are intersted in finding out only controls
    //so put CATID_Control in the array
    CATID pcatidImpl[1];
    CATID pcatidReqd[1];
    pcatidImpl[0]=CATID_Control;

    //Now enumerate the classes i.e. com objects of this type.
    pCatInfo->EnumClassesOfCategories (1,pcatidImpl,0,pcatidReqd ,&pEnumGUID);

    //Enumerate as long as you get S_OK
    CLSID clsid;
    while( (hr= pEnumGUID->Next( 1, &clsid, NULL ))==S_OK )
    {
        BSTR bstrClassName;


        //Get the information of class
        //This is what MSDN says about the parameters
        /*-----------------------------------------------
        USERCLASSTYPE_FULL     The full type name of the class.
        USERCLASSTYPE_SHORT    A short name (maximum of 15 characters) that is used for popup menus and the Links dialog box.
        USERCLASSTYPE_APPNAME  The name of the application servicing the class and is used in the Result text in dialog boxes.
        -----------------------------------------------*/
        OleRegGetUserType (clsid,USERCLASSTYPE_FULL,&bstrClassName);
        //CString strControlName(bstrClassName);
        //Add string in our listbox
        //m_list1.AddString (strControlName);
        WCHAR wBuff[256];
        char buff[256];
        swprintf(wBuff, L"%s", bstrClassName);
        //_tprintf(_T("%s\n"), buff);
        wtoc(buff, wBuff);
        printf("%s\n", buff);
    }

    //we are done so now release the interface ptr
    pCatInfo->Release ();

    ::CoUninitialize ();

    return 0;

}
