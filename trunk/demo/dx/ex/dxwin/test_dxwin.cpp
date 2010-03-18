#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <cassert>

#include <d3d9.h>

#include "uwinapp.h"
#include "ubasewindow.h"

struct CUSTOMVERTEX {
    FLOAT X;
    FLOAT Y;
    FLOAT Z;
    FLOAT RHW;
    DWORD COLOR;
};
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class UDXWindow : public UBaseWindow
{
public:
    UDXWindow(HINSTANCE hInst = ::GetModuleHandle(NULL))
    : UBaseWindow(NULL, hInst)
    {
        setTitle(_T("Direct3D Window"));
    }

    ~UDXWindow()
    {
        releaseD3D();
    }

    BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setStyles(CS_OWNDC);
        return FALSE;
    }

    BOOL onCreate()
    {
        //
        assert(initD3D());

        //m_uico.loadIconEx(::GetModuleHandle(NULL), IDI_APP);
        //this->setIconBig(m_uico);

        return UBaseWindow::onCreate();
    }

    void onDraw(HDC hdc)
    {
        renderD3DScene();
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
    bool initD3D()
    {
        if((pD3D = Direct3DCreate9(D3D_SDK_VERSION))==NULL){
            return false;
        }
        D3DDISPLAYMODE d3ddm;
        pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.BackBufferFormat = d3ddm.Format;
        d3dpp.Windowed = true;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.BackBufferWidth = 1024;
        d3dpp.BackBufferHeight = 768;

        if(FAILED(pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
            *this, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice))){
            return false;
        }

        pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
        pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
        pDevice->SetRenderState( D3DRS_ZENABLE, TRUE );


        // create the vertices using the CUSTOMVERTEX struct
        CUSTOMVERTEX vertices[] =
        {
            { 400.0f, 62.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
            { 650.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
            { 150.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
        };

        // create a vertex buffer interface called v_buffer
        pDevice->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX),
            0,
            CUSTOMFVF,
            D3DPOOL_MANAGED,
            &pVB,
            NULL);

        VOID* pVoid;    // a void pointer

        // lock v_buffer and load the vertices into it
        pVB->Lock(0, 0, (void**)&pVoid, 0);
        memcpy(pVoid, vertices, sizeof(vertices));
        pVB->Unlock();


        return true;
    }

    void renderD3DScene()
    {
        pDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0, 0, 0),1,0);
        pDevice->BeginScene();

        // select which vertex format we are using
        pDevice->SetFVF(CUSTOMFVF);

        // select the vertex buffer to display
        pDevice->SetStreamSource(0, pVB, 0, sizeof(CUSTOMVERTEX));

        // copy the vertex buffer to the back buffer
        pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

        pDevice->EndScene();
        pDevice->Present(NULL,NULL,NULL,NULL);
    }

    void releaseD3D()
    {
        if( pDevice )
        {
            pDevice->Release();
        }

        if( pD3D )
        {
            pD3D->Release();
        }

        if (pVB)
        {
            pVB->Release();
        }

    }

private:
    LPDIRECT3D9 pD3D;
    LPDIRECT3DDEVICE9 pDevice;
    LPDIRECT3DVERTEXBUFFER9 pVB;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    //UDXWinapp app;
    UWinApp app;
    app.setMainWindow(new UDXWindow);
    app.init(hInstance);

    return app.run();
}
