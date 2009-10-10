#include <windows.h>
#include <tchar.h>

//#define INITGUID

#include <objbase.h>
#include <unknwn.h>

#include "./objmodel/appauto.h"
#include "./objmodel/appguid.h"
#include "./objmodel/textauto.h"
#include "./objmodel/textguid.h"

#include "uwinapp.h"
#include "ubasewindow.h"
#include "colors.h"
#include "ugdi.h"

#include "ole/uole.h"

using huys::UOle::BString;
using huys::UOle::CString;

class USmartActiveDocument: public huys::UOle::UDispObject
{
public:
    USmartActiveDocument (huys::UOle::USmartObjFace<IApplication, &IID_IApplication> & app)
    {
        HRESULT hr = app->get_ActiveDocument (&_iDisp);
        if (FAILED (hr))
            throw huys::UOle::HEx (hr, "get_ActiveDocument failed");
    }
};

class SSelection: public huys::UOle::UDispObject
{
public:
    SSelection (huys::UOle::USmartObjFace<ITextDocument, &IID_ITextDocument> & doc)
    {
        HRESULT hr = doc->get_Selection (&_iDisp);
        if (FAILED (hr))
            throw huys::UOle::HEx (hr, "get_Selection failed");
    }
};


class UMyWindow : public UBaseWindow
{
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("DevStudio Test 0.0.1"));
   }

   ~UMyWindow()
   {}

   BOOL onCreate()
   {
       return UBaseWindow::onCreate();
   }


    void PeekApp (HDC hdc)
    {
    int y = 2;
    const char *lpText = NULL;

    try
    {
        // {FB7FDAE2-89B8-11CF-9BE8-00A0C90A632C}
        CLSID idMsDev;
        HRESULT hr = ::CLSIDFromProgID (L"MSDEV.APPLICATION", &idMsDev);
        if (FAILED (hr))
            throw huys::UOle::HEx (hr, "Couldn't convert prog id to class id");
        huys::UOle::USmartObject obj (idMsDev, true);
        huys::UOle::USmartObjFace<IApplication, &IID_IApplication> app (obj);

        VARIANT_BOOL b = VARIANT_TRUE;
        app->put_Visible (b);

        USmartActiveDocument docObj(app); // a dispatch object among others

        if (!docObj)
        {
            lpText = "No active document";
            TextOut(hdc, 10, 10, lpText, strlen(lpText) );
        }
        else
        {
            // Use docObj as a dispatch interface
            DISPID pid = docObj.getDispId (L"Name");
            VARIANT varResult ;
            ::VariantInit(&varResult) ;
            docObj.getProperty (pid, varResult);
            BString bName (varResult);
            CString cName (bName);
            //canvas.Text (20, y, "Name:");
            //canvas.Text (200, y, cName);
            lpText = "Name:";
            TextOut(hdc, 20, y, lpText, strlen(lpText));
            lpText = cName;
            TextOut(hdc, 200, y, lpText, cName.Len());

            y += 20;

            // Get vtable interface for this document
            huys::UOle::USmartObjFace<IGenericDocument, &IID_IGenericDocument> doc (docObj);

            BString bPath;
            doc->get_FullName (bPath.GetPointer ());
            CString path (bPath);
            //canvas.Text (20, y, "Active Document:");
            //canvas.Text (200, y, path);
            lpText = "Active Document:";
            TextOut(hdc, 20, y, lpText, strlen(lpText));
            lpText = path;
            TextOut(hdc, 200, y, lpText, path.Len());


            y += 20;

            BString bType;
            doc->get_Type (bType.GetPointer ());
            CString type (bType);
            //canvas.Text (20, y, "Type:");
            //canvas.Text (200, y, type);
            lpText = "Type:";
            TextOut(hdc, 20, y, lpText, strlen(lpText));
            lpText = type;
            TextOut(hdc, 200, y, lpText, type.Len());


            y += 20;

            if (type.IsEqual ("Text"))
            {
                huys::UOle::USmartObjFace<ITextDocument, &IID_ITextDocument> text (docObj);
                SSelection selObj (text);
                huys::UOle::USmartObjFace<ITextSelection, &IID_ITextSelection> sel (selObj);
                long line;
                sel->get_CurrentLine (&line);
                //canvas.Text (20, y, "CurrentLine:");
                lpText = "CurrentLine:";
                TextOut(hdc, 20, y, lpText, strlen(lpText));
                char buf [10];
                wsprintf (buf, "%ld", line);
                //canvas.Text (200, y, buf);
                TextOut(hdc, 20, y, buf, strlen(buf));

            }
        }
    }
    catch (huys::UOle::HEx & hex)
    {
        //hex.Display (20, 100, canvas);
        lpText = "huys::UOle::HEx";
        TextOut(hdc, 20, 100, lpText, strlen(lpText));
    }
    catch (char const * str)
    {
        //canvas.Text (10, 10, str);
        TextOut(hdc, 10, 10, str, strlen(str));
    }
    catch (...)
    {
        //char msg [] = "Unknown exception";
        //canvas.Text (10, 10, msg);
        lpText = "Unknown exception";
        TextOut(hdc, 10, 10, lpText, strlen(lpText));
    }
}
    //
    virtual void onDraw(HDC hdc)
    {
        PeekApp(hdc);
    }
    //
    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc = {0};
        ::GetClientRect(*this, &rc);
        huys::URectangle urc(rc);
        urc.setFilledColor(huys::aliceblue);
        urc.setFilledStyle(BS_SOLID);
        urc.Draw(hdc);
        return TRUE;
    }

private:
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);

    return app.run();
}
