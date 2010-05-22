#include <windows.h>
#include <tchar.h>

#include <shlwapi.h>

#include <objidl.h>

#include "com/ucom.h"

#ifndef _MSC_VER
#define __out
#define __in
#define __out_opt
#define __in_opt
#define __out_ecount_part(x,y)
#define __inout
#define __in_ecount(x)
const GUID CLSID_IXmlReader = {0x7279FC81, 0x709D, 0x4095, 0xB6, 0x3D, 0x69, 0xFE, 0x4B, 0x0D, 0x90, 0x30};
#define __uuidof(IXmlReader) CLSID_IXmlReader
#endif

#include "xmllite.h"

int _tmain(int argc, _TCHAR* argv[])
{
    HRESULT hr;
    huys::UCOM::UComPtr<IStream> pFileStream;
    huys::UCOM::UComPtr<IXmlReader> pReader;
    XmlNodeType nodeType;
    const WCHAR* pName;
    const WCHAR* pValue;


    //Open XML document
    if (FAILED(hr = SHCreateStreamOnFileW(L"config.xml", STGM_READ, &pFileStream)))
    {
        printf("Error opening XML document, error %08.8lx", hr);
        return -1;
    }

    //if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL)))
    if (FAILED(hr = CreateXmlReader(IID_IXmlReader, (void**)&pReader, NULL)))
    {
        printf("Error creating XmlReader, error %08.8lx", hr);
        return -1;
    }

    if (FAILED(hr = pReader->SetInput(pFileStream)))
    {
        printf("Error setting input for XmlReader, error %08.8lx", hr);
        return -1;
    }

    while (S_OK == (hr = pReader->Read(&nodeType)))
    {
        switch (nodeType)
        {
        case XmlNodeType_Element:
            if (FAILED(hr = pReader->GetQualifiedName(&pName, NULL)))
            {
                printf("Error reading element name, error %08.8lx", hr);
                return -1;
            }
            if (lstrcmpW(pName, L"key") == 0)
            {
                if (SUCCEEDED(hr =
                            pReader->MoveToAttributeByName(L"name", NULL)))
                {
                    if (FAILED(hr = pReader->GetValue(&pValue, NULL)))
                    {
                        printf("Error reading attribute value, error %08.8lx", hr);
                        return -1;
                    }
                    if (lstrcmpW(pValue, L"foo") == 0)
                    {
                        //That's an element we are looking for
                        if (FAILED(hr =
                                    pReader->MoveToAttributeByName(L"value", NULL)))
                        {
                            printf("Error reading attribute \"value\", error %08.8lx", hr);
                            return -1;
                        }
                        if (FAILED(hr = pReader->GetValue(&pValue, NULL)))
                        {
                            printf("Error reading attribute value, error %08.8lx", hr);
                            return -1;
                        }
                        printf("Key \"foo\"'s value is \"%s\"", pValue);
                    }
                }
            }
            break;
        }
    }

    getchar();

    return 0;
}
