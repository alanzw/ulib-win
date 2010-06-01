#include <stdio.h>
//#import "c:\program files\common files\system\ado\msado15.dll" rename ("EOF","adoEOF") no_namespace
#include <windows.h>
#include <objbase.h>
#include <comutil.h>


struct InitOle
{
    InitOle()  { ::CoInitialize(NULL); }
    ~InitOle() { ::CoUninitialize();   }
} InitOle_tag;

//------------------ utility fns to simplify access to recordset fields
_bstr_t RsItem( _RecordsetPtr p, BSTR fldName )
{  // by field name
    return( p->Fields->Item[_variant_t(fldName)]->Value );
}
_bstr_t RsItem( _RecordsetPtr p, long nIdx )
{ // by field # (0 is first)
    return( p->Fields->Item[_variant_t(nIdx)]->Value );
}
//-------------------------------- The Program ----------------
int main()
{
    _RecordsetPtr spRs;
    HRESULT hr;
    _bstr_t sConn= "driver={sql server};SERVER=local);Database=pubs;UID=sa; PWD=;";
    _bstr_t sSQL= "SELECT au_fname+' '+au_lname, phone FROM authors";

    try
    {
        hr= spRs.CreateInstance( __uuidof(Recordset) );
        if FAILED(hr) printf("CreateInstance failed\n");

        hr= spRs->Open( sSQL, sConn, adOpenForwardOnly, adLockReadOnly, adCmdText );
        if FAILED(hr) printf("Open failed\n");

        printf("NAME                      PHONE\n");
        while( !(spRs->adoEOF) ) {
            printf("%-25.25s %s\n",
                    (char*) RsItem( spRs, 0L ),
                    (char*) RsItem( spRs, L"phone")  // or 1L
            );
            spRs->MoveNext();
        }
        spRs->Close();
    } catch( _com_error &e) {
        printf("Error:%s\n",(char*)e.Description());
    }
    return 0;
}

