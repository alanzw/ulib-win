/*
 * =====================================================================================
 *
 *       Filename:  chat.cpp
 *
 *    Description:  chat socket example.
 *
 *        Version:  1.0
 *        Created:  2009-8-2 21:47:58
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
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <tchar.h>

#include "ubasewindow.h"
#include "uwinapp.h"
#include "usocket.h"

#include "utime.h"

#include "adt/ustring.h"

typedef huys::ADT::UStringAnsi UCString;

#include "aui/aui_label.h"
#include "aui/aui_entry.h"

class UCMessage
{
public:
    UCString user_name;
    UCString content;
    UTime time;
};

struct IMessageSender
{
    bool connect(int ip, int port);
    bool sendMessage(UCMessage msg);
    void signout();
};

typedef void (*EventHandler)();

struct IMessageReceiver
{
    EventHandler MessageReceived;
    EventHandler ClientLost;
    EventHandler ClientConnected;
    void StartListen();
    void StopListen();
};


class UMyWindow : public UBaseWindow
{
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("Chat Socket 0.0.1"));
   }
   
      BOOL onCreate()
   {
       this->setIconBig(IDI_APP);
       
        huys::URectL rect;
        rect.set(110, 20, 200, 20);
       
       _title = new AUI::UTransLabel(this, _T("Let's chat!"));
       _title->setRect(rect);
       _title->create();

       return UBaseWindow::onCreate();
   }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }
private:
    AUI::UTransLabelP _title;
    AUI::UTransLabelP _send;
    AUI::UTransLabelP _log;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

   
   