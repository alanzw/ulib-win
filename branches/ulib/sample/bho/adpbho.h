#pragma once

#include <ocidl.h>
#include <exdisp.h>

class BHO class adpbho : public IObjectWithSite, public IDispatch  {
  int m;
  public:

  adpbho(unsigned long cookie) : m(1), webBrowser_(0), connectionPointContainer(0), cookie_(cookie), document_(0) {};

 ~adpbho() {
    webBrowser_->Release(); connectionPointContainer->Release(); 

    if (document_) document_->Release();
  };

  ULONG STDMETHODCALLTYPE AddRef();

  ULONG STDMETHODCALLTYPE Release();

  HRESULT STDMETHODCALLTYPE QueryInterface(
      REFIID iid, //Identifier of the requested interface
      void ** ppvObject //Address of output variable that receives the //interface pointer requested in iid
  );
      
  HRESULT STDMETHODCALLTYPE GetSite(
      REFIID riid,
      void** ppvSite
  );

  HRESULT STDMETHODCALLTYPE SetSite(IUnknown*);

  // --------- methods from IDispatch
  
  HRESULT STDMETHODCALLTYPE GetTypeInfoCount(unsigned int FAR* pctinfo);
  HRESULT STDMETHODCALLTYPE GetTypeInfo( 
        unsigned int  iTInfo,         
          LCID  lcid,                   
            ITypeInfo FAR* FAR*  ppTInfo  
      );

  HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
        REFIID  riid,                  
        OLECHAR FAR* FAR*  rgszNames,  
        unsigned int  cNames,          
        LCID   lcid,                   
        DISPID FAR*  rgDispId          
      );

  HRESULT STDMETHODCALLTYPE adpbho::Invoke( 
     DISPID             dispIdMember,      
     REFIID             riid,              
     LCID               lcid,                
     WORD               wFlags,              
     DISPPARAMS FAR*    pDispParams,  
     VARIANT FAR*       pVarResult,  
     EXCEPINFO FAR*     pExcepInfo,  
     unsigned int FAR*  puArgErr  
    ); 




  private:
  IWebBrowser2*              webBrowser_;
  IConnectionPointContainer* connectionPointContainer;
  unsigned long              cookie_;
  IDispatch*                 document_;
};
