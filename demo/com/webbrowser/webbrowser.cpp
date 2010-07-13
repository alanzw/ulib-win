#include <windows.h>
#include <tchar.h>
#include <exdisp.h>
#include <mshtml.h>

#ifdef _MSC_VER
EXTERN_C const CLSID CLSID_InternetExplorer =
        __uuidof (InternetExplorer);

EXTERN_C const   IID   IID_IWebBrowser2 =
        __uuidof (IWebBrowser2);
#else
EXTERN_C const IID IID_IWebBrowser2;
EXTERN_C const IID CLSID_InternetExplorer;
EXTERN_C const IID IID_IHTMLDocument2;
#endif // _MSC_VER

//#import <shdocvw.dll>

#include "shdocvw.tlh"

#ifndef ASSERT
#include <assert.h>
#define ASSERT assert
#endif

#include "adt/ustring.h"

typedef huys::ADT::UStringAnsi TString;

class UIExplorer
{
public:
    UIExplorer()
        :m_pWebBrowser(0)
    {

    }

    ~UIExplorer();

    bool createNewInstance()
    {
        if (m_pWebBrowser != NULL)
        {
            m_pWebBrowser->Release ();
            m_pWebBrowser = NULL;
        }

        HRESULT hr;
        IWebBrowser2* pWebBrowser = NULL;
        hr = CoCreateInstance (CLSID_InternetExplorer, NULL, CLSCTX_SERVER, IID_IWebBrowser2, (LPVOID*)&pWebBrowser);

        if (SUCCEEDED (hr) && (pWebBrowser != NULL))
        {
            m_pWebBrowser = pWebBrowser;
            m_pWebBrowser->put_Visible (VARIANT_TRUE);
            return true;
        }
        else
        {
            if (pWebBrowser)
                pWebBrowser->Release ();
            return false;
        }
        return false;
    }

    bool findUsingTitle(LPCTSTR sTitleToSearch)
    {
        if (m_pWebBrowser != NULL)
        {
            m_pWebBrowser->Release ();
            m_pWebBrowser = NULL;
        }

        HRESULT hr;
        SHDocVw::IShellWindowsPtr spSHWinds;
        hr = spSHWinds.CreateInstance
            (__uuidof(SHDocVw::ShellWindows));

        if (FAILED (hr))
            return false;

        ASSERT (spSHWinds != NULL);

        long nCount = spSHWinds->GetCount ();

        IDispatchPtr spDisp;

        for (long i = 0; i < nCount; i++)
        {
            _variant_t va (i, VT_I4);
            spDisp = spSHWinds->Item (va);

            IWebBrowser2 * pWebBrowser = NULL;
            hr = spDisp.QueryInterface (IID_IWebBrowser2, &
                pWebBrowser);

            if (pWebBrowser != NULL)
            {
                HRESULT hr;
                IDispatch* pHtmlDocDispatch = NULL;
                IHTMLDocument2 * pHtmlDoc = NULL;

                // Retrieve the document object.
                hr = pWebBrowser->get_Document
                    (&pHtmlDocDispatch);

                if (SUCCEEDED (hr) && (pHtmlDocDispatch != NULL))
                {
                    // Query for IPersistStreamInit.
                    hr = pHtmlDocDispatch->QueryInterface
                        (IID_IHTMLDocument2,
                        (void**)&pHtmlDoc);
                    if (SUCCEEDED (hr) && (pHtmlDoc != NULL))
                    {
                        TString sTitle;

                        HWND hWnd = NULL;
                        pWebBrowser->get_HWND ((long*)(&hWnd));
                        if (::IsWindow (hWnd))
                        {
                            int nLen = ::GetWindowTextLength (hWnd);
                            //::GetWindowText (hWnd,
                            //    sTitle.GetBufferSetLength(nLen),
                            //   nLen + 1);
                            //sTitle.ReleaseBuffer ();
                        }

                        // If I cannot get the window title
                        // (should never happen, though)
                        // So, let's just use the title of the document
                        //if (sTitle.IsEmpty ())
                        {
                            BSTR bstrTitle;
                            hr = pHtmlDoc->get_title (&bstrTitle);
                            if (!FAILED (hr))
                            {
                                //sTitle = bstrTitle;
                                SysFreeString (bstrTitle); 
                            }
                        }

                        //if (StringHelper::WildcardCompareNoCase(sTitleToSearch, sTitle))
                        {
                            m_pWebBrowser = pWebBrowser;
                            pHtmlDoc->Release ();
                            pHtmlDocDispatch->Release ();
                            // Exit the method safely!
                            return true;
                        }
                        pHtmlDoc->Release();
                    }
                    pHtmlDocDispatch->Release ();
                }
                pWebBrowser->Release ();
            }
        }

        return false;

    }

    void navigate(LPCTSTR lpszURL,
        DWORD dwFlags /* = 0 */,
        LPCTSTR lpszTargetFrameName /* = NULL */ ,
        LPCTSTR lpszHeaders /* = NULL */,
        LPVOID lpvPostData /* = NULL */,
        DWORD dwPostDataLen /* = 0 */)
    {
        TString strURL (lpszURL);
        //BSTR bstrURL = strURL.AllocSysString ();

        COleSafeArray vPostData;
        if (lpvPostData != NULL)
        {
            if (dwPostDataLen == 0)
                dwPostDataLen = lstrlen ((LPCTSTR) lpvPostData);

            vPostData.CreateOneDim (VT_UI1, dwPostDataLen, lpvPostData);
        }

        m_pWebBrowser->Navigate (bstrURL, COleVariant
            ((long) dwFlags, VT_I4),
            COleVariant (lpszTargetFrameName,
            VT_BSTR), vPostData,
            COleVariant
            (lpszHeaders, VT_BSTR));

        SysFreeString (bstrURL);
    }

    bool waitTillLoaded (int nTimeout)
    {
        READYSTATE result;
        DWORD nFirstTick = GetTickCount ();

        do
        {
            m_pWebBrowser->get_ReadyState (&result);

            if (result != READYSTATE_COMPLETE)
                Sleep (250);

            if (nTimeout > 0)
            {
                if ((GetTickCount () - nFirstTick) > nTimeout)
                    break;
            }
        } while (result != READYSTATE_COMPLETE);

        if (result == READYSTATE_COMPLETE)
            return true;
        else
            return false;
    }

    bool findAnchor (bool bClick, bool bFocus,
        bool bName, bool bOuterText,
        bool bTooltip, bool bURL,
        LPCTSTR sName,
        LPCTSTR sOuterText,
        LPCTSTR sTooltip,
        LPCTSTR sURL)
    {
        ASSERT (m_pWebBrowser != NULL);
        if (m_pWebBrowser == NULL)
            return false;

        HRESULT hr;
        IDispatch* pHtmlDocDispatch = NULL;
        IHTMLDocument2 * pHtmlDoc = NULL;
        bool bSearch = true;

        // Retrieve the document object.
        hr = m_pWebBrowser->get_Document (&pHtmlDocDispatch);
        if (SUCCEEDED (hr) && (pHtmlDocDispatch != NULL))
        {
            hr = pHtmlDocDispatch->QueryInterface (IID_IHTMLDocument2,
                (void**)&pHtmlDoc);
            if (SUCCEEDED (hr) && (pHtmlDoc != NULL))
            {
                IHTMLElementCollection* pColl = NULL;
                hr = pHtmlDoc->get_all (&pColl);

                if (SUCCEEDED (hr) && (pColl != NULL))
                {
                    // Obtained the Anchor Collection...
                    long nLength = 0;
                    pColl->get_length (&nLength);

                    for (int i = 0; i < nLength && bSearch; i++)
                    {
                        COleVariant vIdx ((long)i, VT_I4);

                        IDispatch* pElemDispatch = NULL;
                        IHTMLElement * pElem = NULL;

                        hr = pColl->item (vIdx, vIdx, &pElemDispatch);

                        if (SUCCEEDED (hr) && (pElemDispatch != NULL))
                        {
                            hr = pElemDispatch->QueryInterface
                                (IID_IHTMLElement, (void**)&pElem);

                            if (SUCCEEDED (hr) && (pElem != NULL))
                            {
                                BSTR bstrTagName;
                                CString sTempTagName;
                                if (!FAILED (pElem->get_tagName
                                    (&bstrTagName)))
                                {
                                    sTempTagName = bstrTagName;
                                    SysFreeString (bstrTagName);
                                }

                                if (sTempTagName == _T ("a") ||
                                    sTempTagName == _T ("A"))
                                {
                                    IHTMLAnchorElement * pAnchor = NULL;
                                    hr = pElemDispatch->
                                        QueryInterface(
                                        IID_IHTMLAnchorElement,
                                        (void**)&pAnchor);

                                    if (SUCCEEDED (hr) &&
                                        (pAnchor != NULL))
                                    {
                                        BSTR bstrName, bstrOuterText,
                                            bstrURL,
                                            bstrTooltip;
                                        CString sTempName, sTempOuter,
                                            sTempURL, sTempTooltip;

                                        if (!FAILED
                                            (pElem->get_outerText
                                            (&bstrOuterText)))
                                        {
                                            sTempOuter = bstrOuterText;
                                            SysFreeString
                                                (bstrOuterText);
                                        }
                                        if (!FAILED (pElem->get_title
                                            (&bstrTooltip)))
                                        {
                                            sTempTooltip = bstrTooltip;
                                            SysFreeString (bstrTooltip);
                                        }
                                        if (!FAILED (pAnchor->get_name
                                            (&bstrName)))
                                        {
                                            sTempName = bstrName;
                                            SysFreeString (bstrName);
                                        }
                                        if (!FAILED (pAnchor->get_href
                                            (&bstrURL)))
                                        {
                                            sTempURL = bstrURL;
                                            SysFreeString (bstrURL);
                                        }

                                        // Do the comparison here!
                                        bool bMatches = true;
                                        if (bMatches && bName)
                                        {
                                            if (!StringHelper::
                                                WildcardCompareNoCase
                                                (sName, sTempName))
                                                bMatches = false;
                                        }
                                        if (bMatches && bOuterText)
                                        {
                                            if (!StringHelper::
                                                WildcardCompareNoCase
                                                (sOuterText,
                                                sTempOuter))
                                                bMatches = false;
                                        }
                                        if (bMatches && bURL)
                                        {
                                            if (!StringHelper::
                                                WildcardCompareNoCase
                                                (sURL, sTempURL))
                                                bMatches = false;
                                        }
                                        if (bMatches && bTooltip)
                                        {
                                            if (!StringHelper::
                                                WildcardCompareNoCase
                                                (sTooltip,
                                                sTempTooltip))
                                                bMatches = false;
                                        }

                                        if (bMatches)
                                        {
                                            // No need to search more!
                                            bSearch = false;

                                            if (bFocus)
                                                pAnchor->focus ();
                                            if (bClick)
                                                pElem->click ();
                                        }
                                        pAnchor->Release ();
                                    }
                                }
                                pElem->Release ();
                            }
                            pElemDispatch->Release ();
                        }
                    }
                    pColl->Release ();
                }
                pHtmlDoc->Release();
            }
            pHtmlDocDispatch->Release ();
        }

        if (bSearch == false)
            return true;

        return false;
    }

    bool findInput  (bool bClick, bool bSelect,
        bool bChangeValue,
        bool bSetCheck, bool bType,
        bool bName, bool bValue,
        LPCTSTR sTypeToLook,
        LPCTSTR sNameToLook,
        LPCTSTR sValueToLook,
        bool bNewCheckValue,
        LPCTSTR sNewValue)
    {
        ASSERT (m_pWebBrowser != NULL);
        if (m_pWebBrowser == NULL)
            return false;

        HRESULT hr;
        IDispatch* pHtmlDocDispatch = NULL;
        IHTMLDocument2 * pHtmlDoc = NULL;
        bool bSearch = true;

        // Retrieve the document object.
        hr = m_pWebBrowser->get_Document (&pHtmlDocDispatch);
        if (SUCCEEDED (hr) && (pHtmlDocDispatch != NULL))
        {
            hr = pHtmlDocDispatch->QueryInterface (IID_IHTMLDocument2,
                (void**)&pHtmlDoc);
            if (SUCCEEDED (hr) && (pHtmlDoc != NULL))
            {
                IHTMLElementCollection* pColl = NULL;
                hr = pHtmlDoc->get_all (&pColl);

                if (SUCCEEDED (hr) && (pColl != NULL))
                {
                    // Obtained the Anchor Collection...
                    long nLength = 0;
                    pColl->get_length (&nLength);

                    for (int i = 0; i < nLength && bSearch; i++)
                    {
                        COleVariant vIdx ((long)i, VT_I4);

                        IDispatch* pElemDispatch = NULL;
                        IHTMLElement * pElem = NULL;

                        hr = pColl->item (vIdx, vIdx, &pElemDispatch);

                        if (SUCCEEDED (hr) && (pElemDispatch != NULL))
                        {
                            hr = pElemDispatch->QueryInterface
                                (IID_IHTMLElement, (void**)&pElem);

                            if (SUCCEEDED (hr) && (pElem != NULL))
                            {
                                BSTR bstrTagName;
                                CString sTempTagName;
                                if (!FAILED (pElem->get_tagName
                                    (&bstrTagName)))
                                {
                                    sTempTagName = bstrTagName;
                                    sTempTagName.MakeLower ();
                                    //AfxMessageBox (sTempTagName);
                                    SysFreeString (bstrTagName);
                                }
                                if (sTempTagName == _T ("input"))
                                {
                                    IHTMLInputElement * pInputElem
                                        = NULL;
                                    hr = pElemDispatch->QueryInterface
                                        (IID_IHTMLInputElement,
                                        (void**)&pInputElem);

                                    if (SUCCEEDED (hr) &&
                                        (pInputElem != NULL))
                                    {
                                        BSTR bstrType, bstrName,
                                            bstrValue; CString sTempType,
                                            sTempName, sTempValue;

                                        if (!FAILED
                                            (pInputElem->get_type
                                            (&bstrType)))
                                        {
                                            sTempType = bstrType;
                                            SysFreeString (bstrType);
                                        }
                                        if (!FAILED
                                            (pInputElem->get_name
                                            (&bstrName)))
                                        {
                                            sTempName = bstrName;
                                            SysFreeString (bstrName);
                                        }
                                        if (!FAILED
                                            (pInputElem->get_value
                                            (&bstrValue)))
                                        {
                                            sTempValue = bstrValue;
                                            SysFreeString (bstrValue);
                                        }
                                        // Do the comparison here!
                                        bool bMatches = true;
                                        if (bMatches && bType)
                                        {
                                            if (!StringHelper::
                                                WildcardCompareNoCase
                                                (sTypeToLook,
                                                sTempType))
                                                bMatches = false;
                                        }
                                        if (bMatches && bName)
                                        {
                                            if (!StringHelper::
                                                WildcardCompareNoCase
                                                (sNameToLook,
                                                sTempName))
                                                bMatches = false;
                                        }
                                        if (bMatches && bValue)
                                        {
                                            if (!StringHelper::
                                                WildcardCompareNoCase
                                                (sValueToLook,
                                                sTempValue))
                                                bMatches = false;
                                        }

                                        if (bMatches)
                                        {
                                            // No need to search more!
                                            bSearch = false;

                                            if (bSetCheck)
                                            {
                                                if (bNewCheckValue)
                                                    pInputElem->
                                                    put_checked
                                                    (VARIANT_TRUE);
                                                else
                                                    pInputElem->
                                                    put_checked
                                                    (VARIANT_FALSE);
                                            }
                                            if (bChangeValue)
                                            {
                                                CString sTemp
                                                    (sNewValue);
                                                BSTR bstrNewValue =
                                                    sTemp.AllocSysString ();
                                                pInputElem->
                                                    put_value
                                                    (bstrNewValue);
                                                SysFreeString
                                                    (bstrNewValue);
                                            }
                                            if (bSelect)
                                                pInputElem->select ();

                                            if (bClick)
                                                pElem->click ();
                                        }
                                        pInputElem->Release ();
                                    }
                                }
                                pElem->Release ();
                            }
                            pElemDispatch->Release ();
                        }
                    }
                    pColl->Release ();
                }
                pHtmlDoc->Release();
            }
            pHtmlDocDispatch->Release ();
        }

        if (bSearch == false)
            return true;

        return false;
    }

private:
    IWebBrowser2* m_pWebBrowser;
};

int main()
{
    if (SUCCEEDED(OleInitialize(NULL)))
    {
        IWebBrowser2* pBrowser2;

        CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER,
            IID_IWebBrowser2, (void**)&pBrowser2);
        if (pBrowser2)
        {
            VARIANT vEmpty;
            VariantInit(&vEmpty);

            BSTR bstrURL = SysAllocString(L"http://www.baidu.com");

            HRESULT hr = pBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vEmpty);
            if (SUCCEEDED(hr))
            {
                pBrowser2->put_Visible(VARIANT_TRUE);
            }
            else
            {
                pBrowser2->Quit();
            }

            SysFreeString(bstrURL);
            pBrowser2->Release();
        }

        OleUninitialize();
    }

    return 0;
}
