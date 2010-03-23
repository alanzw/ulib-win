#include "adpbho.h"
//#include "ExtieFactory.h"
#include "Exdisp.h"
#include "Exdispid.h"
#include "mshtml.h"
#include "mshtmdid.h"
#include "olectl.h"

#include <stdio.h>

ULONG STDMETHODCALLTYPE adpbho::AddRef() {
	char buf[50];
	m++;
	sprintf(buf, "adpbho Add Ref: %d", m);
	MB1(buf);
  return m;
}

ULONG STDMETHODCALLTYPE adpbho::Release() {
  if (--m != 0 ) {
    return m;
  }

  MB1("deleting adpbho");
  delete this;
  return 0;
}

HRESULT STDMETHODCALLTYPE adpbho::QueryInterface(
      REFIID riid, //Identifier of the requested interface
      void ** ppvObject //Address of output variable that receives the 
                        //interface pointer requested in iid
      ) {
  MB1("adpbho: QueryInterface");
  
  /*
adpbho: neither IID_IUnknown nor clsid_extie, {34A715A0-6587-11D0-924A-0020AFC7AC4D}
DIID_DWebBrowserEvents2

adpbho: neither IID_IUnknown nor clsid_extie, {49C3DE7C-D329-11D0-AB73-00C04FC33E80}
*/

  if ( riid == IID_IUnknown ) {
    *ppvObject = static_cast<adpbho*> (this);
  } 
  else if (riid == IID_IObjectWithSite) {
    // ::MessageBox(0, "Querying for IID_IObjectWithSite", 0, 0);
    *ppvObject = static_cast<IObjectWithSite*>(this);
    //::MessageBox(0,"XXXXXXXXXXXXXXXXXXXX", 0, 0);
    //char buf[50];
    //sprintf(buf, "%x", ppvObject 
    //static_cast<IObjectWithSite*>(*ppvObject)->SetSite(0);
  }
  else if (riid == IID_IDispatch) {
    // ::MessageBox(0, "Querying for IID_IDispatch", 0, 0);
    *ppvObject = static_cast<IDispatch*>(this);
    
    //unsigned int* x=new unsigned(4);
    //static_cast<IDispatch*>(this)->GetTypeInfoCount(x);
    //::MessageBox(0,"its an GotType", 0, 0);
  }
  else {
    // Information on StringFromCLSID can be found here:
    // http://msdn.microsoft.com/library/default.asp?url=/library/en-us/com/cmf_a2c_05r8.asp
    wchar_t* buf;
    StringFromCLSID(riid, &buf);
    wchar_t buf1[150];
    wsprintfW(buf1, L"adpbho: neither IID_IUnknown nor clsid_extie, %s", buf);
	  ::CoTaskMemFree(buf);
    // ::MessageBoxW(0, buf1, 0, 0);
    
    *ppvObject = 0;
    return E_NOINTERFACE;
  }

	AddRef();
  return S_OK;
}

      
HRESULT STDMETHODCALLTYPE adpbho::GetSite(
	REFIID riid,
    void** ppvSite)
{
  MB1("GetSite");
  return E_NOINTERFACE;
}

// Provides the site\'s IUnknown pointer to the object. The object should hold onto this pointer,
// calling IUnknown::AddRef in doing so. If the object already has a site, it should call that 
// existing site\'s IUnknown::Release, save the new site pointer, and call the new site\'s IUnknown::AddRef.



HRESULT STDMETHODCALLTYPE adpbho::#B(SetSite)(IUnknown* iu) {
  MB1("SetSite");
  // ::MessageBox(0, "SetSite", 0, 0);

  if (! (iu->QueryInterface(IID_IWebBrowser2, (void**) &webBrowser_) == S_OK )) {
    MB1("QueryInterface failed on SetSite for IID_IWebBrowser2");
  }

  if (! (iu->QueryInterface(IID_IConnectionPointContainer, (void**) &connectionPointContainer) == S_OK )) {
    MB1("QueryInterface failed on SetSite for IID_IConnectionPointContainer");
    // ::MessageBox(0, "xxx", 0, 0);
  }

  // make sure we\'re getting browser events
  IConnectionPoint* spCP;
  HRESULT hr = connectionPointContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &spCP);
  if (FAILED(hr)) {
     MB1("Failed hr")
  }
  hr = spCP->Advise(static_cast<IDispatch*>(this), &cookie_);

 

  VARIANT b;
	b.vt=VT_BOOL;
	b.boolVal=0x0000;
  webBrowser_->put_Resizable(0x0000);

  return S_OK;
}

HRESULT STDMETHODCALLTYPE adpbho::GetTypeInfoCount(unsigned int FAR* pctinfo) {
  //::MessageBox(0, "GetTypeInfoCount", 0, 0);
  *pctinfo = 1;
  return NOERROR;
}

// ------------------------------------------------------------
// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/automat/htm_hh2/chap5_8anj.asp
HRESULT STDMETHODCALLTYPE adpbho::GetTypeInfo( 
        unsigned int  iTInfo,         
          LCID  lcid,                   
            ITypeInfo FAR* FAR*  ppTInfo  
      ) {

  //::MessageBox(0, "GetTypeInfo", 0, 0);
  return NOERROR;
}

// ------------------------------------------------------------
// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/automat/htm_hh2/chap5_32cz.asp

HRESULT STDMETHODCALLTYPE adpbho::GetIDsOfNames( 
        REFIID  riid,                  
          OLECHAR FAR* FAR*  rgszNames,  
            unsigned int  cNames,          
              LCID   lcid,                   
                DISPID FAR*  rgDispId          
      ) {

  //::MessageBox(0, "GetTypeInfo", 0, 0);

  return NOERROR;

}

// ------------------------------------------------------------
// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/automat/htm_hh2/chap5_61id.asp

HRESULT STDMETHODCALLTYPE adpbho::Invoke( 
      DISPID             dispIdMember,      
      REFIID             riid,              
      LCID               lcid,                
      WORD               wFlags,              
      DISPPARAMS FAR*    pDispParams,  
      VARIANT FAR*       pVarResult,  
      EXCEPINFO FAR*     pExcepInfo,  
      unsigned int FAR*  puArgErr  
    ) {

  //
  // See exdispid.h for a list of DISPIDs
  switch (dispIdMember) {

    case DISPID_BEFORENAVIGATE     :

    break;

    case DISPID_NAVIGATECOMPLETE   :

    break;

    case DISPID_STATUSTEXTCHANGE   :

    break;

    case DISPID_QUIT               :
    break;

    case DISPID_DOWNLOADCOMPLETE   :
    break;

    case DISPID_COMMANDSTATECHANGE :
    break;

    case DISPID_DOWNLOADBEGIN      :
    break;

    case DISPID_NEWWINDOW          :
    break;

    case DISPID_PROGRESSCHANGE     :
    break;

    case DISPID_WINDOWMOVE         :
    break;

    case DISPID_WINDOWRESIZE       :
    break;

    case DISPID_WINDOWACTIVATE     :
    break;

    case DISPID_PROPERTYCHANGE     :
    break;

    case DISPID_TITLECHANGE        :
    break;

    case DISPID_FRAMEBEFORENAVIGATE   :

  break;

    case DISPID_FRAMENAVIGATECOMPLETE :

    break;

    case DISPID_FRAMENEWWINDOW        :
    break;

    case DISPID_BEFORENAVIGATE2      :
    break;

    case DISPID_NEWWINDOW2           :
    break;

    case DISPID_NAVIGATECOMPLETE2    :
    break;

    case DISPID_ONQUIT               :
    break;

    case DISPID_ONVISIBLE            :
    break;

    case DISPID_ONTOOLBAR            :
    break;

    case DISPID_ONMENUBAR            :
    break;

    case DISPID_ONSTATUSBAR          :
    break;

    case DISPID_ONFULLSCREEN         :
    break;

    // on Msdn page
    // http://support.microsoft.com/default.aspx?scid=kb;EN-US;q246247
    // it says:
    // 1. Sink the WebBrowser control\'s events (the DWebBrowserEvents2 interface)
    //    and capture the DocumentComplete event. Because the document is not guaranteed to 
    //    be fully loaded until this event fires, all document sinking must be done from here. 
    case DISPID_DOCUMENTCOMPLETE: {
		CHR(webBrowser_->get_Document(&document_)); 
    	if (! (document_->QueryInterface(IID_IConnectionPointContainer, (void**) &connectionPointContainer) == S_OK )) {
			::MessageBox(0, "xxx", 0, 0);
		}


	   IConnectionPoint* cpHTMLDocument;
       // make sure we\'re getting browser events
       HRESULT hr = connectionPointContainer->FindConnectionPoint(DIID_HTMLDocumentEvents2, &cpHTMLDocument);
       if (FAILED(hr)) {
         ::MessageBox(0, "DIID_HTMLDocumentEvents failed", 0, 0);
         MB1("Failed hr")
	   }
	   //::MessageBox(0, "goin", 0, 0);
     hr = cpHTMLDocument->Advise(static_cast<IDispatch*>(this), &cookie_);
       if (FAILED(hr)) {
         ::MessageBox(0, "Advise failed", 0, 0);
         MB1("Failed hr")
       }
      
	}
    break;

    case DISPID_ONTHEATERMODE        :
    break;

    case DISPID_ONADDRESSBAR         :
    break;

    //case DISPID_SELECTIONCHANGED:
    //break;

    // case DISPID_WINDOWREGISTERED     :
    // break;

    // case DISPID_WINDOWREVOKED        :
    // break;

    case DISPID_RESETFIRSTBOOTMODE:
    break;

    case DISPID_RESETSAFEMODE:
    break;

    case DISPID_REFRESHOFFLINEDESKTOP:
    break;

    case DISPID_ADDFAVORITE:
    break;

    case DISPID_ADDCHANNEL           :
    break;

    case DISPID_ADDDESKTOPCOMPONENT  :
    break;

    case DISPID_ISSUBSCRIBED         :
    break;

    case DISPID_SHELLUIHELPERLAST    :
    break;

    case 270:
    break;


    // The DISPIDs found from:
    // http://msdn.microsoft.com/workshop/browser/mshtml/reference/events/htmldocumentevents2/htmldocumentevents2.asp?frame=true
    /*
     * DISPID_HTMLDOCUMENTEVENTS_ONHELP             
     * DISPID_HTMLDOCUMENTEVENTS_ONCLICK            
     * DISPID_HTMLDOCUMENTEVENTS_ONDBLCLICK         
     * DISPID_HTMLDOCUMENTEVENTS_ONKEYDOWN          
     * DISPID_HTMLDOCUMENTEVENTS_ONKEYUP            
     * DISPID_HTMLDOCUMENTEVENTS_ONKEYPRESS         
     * DISPID_HTMLDOCUMENTEVENTS_ONMOUSEDOWN        
     * DISPID_HTMLDOCUMENTEVENTS_ONMOUSEMOVE        
     * DISPID_HTMLDOCUMENTEVENTS_ONMOUSEUP          
     * DISPID_HTMLDOCUMENTEVENTS_ONMOUSEOUT         
     * DISPID_HTMLDOCUMENTEVENTS_ONMOUSEOVER        
     * DISPID_HTMLDOCUMENTEVENTS_ONREADYSTATECHANGE 
     * DISPID_HTMLDOCUMENTEVENTS_ONBEFOREUPDATE     
     * DISPID_HTMLDOCUMENTEVENTS_ONAFTERUPDATE      
     * DISPID_HTMLDOCUMENTEVENTS_ONROWEXIT          
     * DISPID_HTMLDOCUMENTEVENTS_ONROWENTER         
     * DISPID_HTMLDOCUMENTEVENTS_ONDRAGSTART        
     * DISPID_HTMLDOCUMENTEVENTS_ONSELECTSTART      
     * DISPID_HTMLDOCUMENTEVENTS_ONERRORUPDATE      
     */

    case DISPID_HTMLDOCUMENTEVENTS_ONCLICK: {
       ::MessageBox(0, "click", 0, 0);
      
    }
    break;

    // onkeydown is documented at:
    // http://msdn.microsoft.com/workshop/browser/mshtml/reference/events/htmldocumentevents2/onkeydown.asp
    case DISPID_HTMLDOCUMENTEVENTS_ONKEYDOWN: {
      
      // DISPPARAMS are described at:
      // http://msdn.microsoft.com/library/default.asp?url=/library/en-us/automat/htm_hh2/chap6_12gj.asp
      if (pDispParams->cArgs != 1) {
        ::MessageBox(0, "key down cArgs != 1", 0, 0);
      }
      VARIANTARG v = (pDispParams -> rgvarg)[0];
      if (v.vt != VT_DISPATCH) {
        ::MessageBox(0, "vt.v != VT_DISPATCH", 0, 0);
      }
      IDispatch* i = v.pdispVal;

      DISPID dispid_keyCode;

      wchar_t* name_keyCode =  L"keyCode";
      if (S_OK != i->GetIDsOfNames(IID_NULL, &name_keyCode, 1, LOCALE_SYSTEM_DEFAULT, &dispid_keyCode)) {
        ::MessageBox(0, "GetIDsOfNames", 0, 0);
      }
      else {
        char buf[50];
        sprintf(buf, "dispid_keyCode: %d", dispid_keyCode);
        ::MessageBox(0, buf, 0, 0);
      }

      VARIANT result;
      ::VariantInit(&result);

      DISPPARAMS nullDispParams;
      nullDispParams.rgvarg            = 0;
      nullDispParams.rgdispidNamedArgs = 0;
      nullDispParams.cArgs             = 0;
      nullDispParams.cNamedArgs        = 0;
      // ::MessageBox(0, "befor", 0, 0);
      i->Invoke(dispid_keyCode, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &nullDispParams, &result, 0, 0);
      // ::MessageBox(0, "nach", 0, 0);
      

      if (result.vt == VT_I4) {
        // char buf[50];
        // sprintf(buf, "char: %d", result.lVal);
        // ::MessageBox(0, buf, 0, 0);
      }
      else {
        MB1(0, "result.vt", 0, 0);
      }

    }
    break;

    default: {
      char buf[50];
      sprintf(buf, "dispID: %d", dispIdMember);
      //::MessageBox(0, buf, 0, 0);
    }

  }

  return NOERROR;
}
