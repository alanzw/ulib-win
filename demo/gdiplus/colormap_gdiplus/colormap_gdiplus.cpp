// #define UNICODE

#include "resource.h"

#include <windows.h>
#include <tchar.h>

#ifdef __GNUC__
#include <gdiplus/gdiPlus.h>
#else
#include <gdiPlus.h>
#endif

#include "ubasewindow.h"
#include "uwinapp.h"
#include "udc.h"
#include "ugdiplus.h"

using namespace Gdiplus;

struct Color_HSL {
    double h;
    double s; 
    double l;
};

Color HSL_to_RGB(Color_HSL hsl) 
{ 
    double r=0,g=0,b=0; 
    double temp1,temp2; 
    if(hsl.l==0) 
    { 
        r=g=b=0; 
    } 
    else 
    { 
        if(hsl.s==0) 
        { 
            r=g=b=hsl.l; 
        } 
        else 
        { 
            temp2 = ((hsl.l<=0.5) ? hsl.l*(1.0+hsl.s) : hsl.l+hsl.s-(hsl.l*hsl.s)); 
            temp1 = 2.0*hsl.l-temp2;         

            double t3[3] = {hsl.h+1.0/3.0,hsl.h,hsl.h-1.0/3.0}; 
            double clr[3] = {0,0,0}; 

            for(int i=0;i<3;i++) 
            { 
                if(t3[i]<0) 
                {
                    t3[i]+=1.0; 
                }
                
                if(t3[i]>1) 
                {
                    t3[i]-=1.0; 
                }
                
                if (6.0*t3[i] < 1.0) 
                {
                    clr[i]=temp1+(temp2-temp1)*t3[i]*6.0; 
                }
                else if (2.0*t3[i] < 1.0) 
                {
                    clr[i]=temp2; 
                }
                else if (3.0*t3[i] < 2.0) 
                {
                    clr[i]=(temp1+(temp2-temp1)*((2.0/3.0)-t3[i])*6.0); 
                }
                else 
                {
                    clr[i]=temp1; 
                }
            } 

            r=clr[0]; 

            g=clr[1]; 

            b=clr[2]; 

        } 

    } 
    return Color((int)(255*r),(int)(255*g),(int)(255*b));
}

Color_HSL RGB_to_HSL (Color c) 
{ 
	Color_HSL hsl;

	int Max, Min, Diff, Sum;
	// Of our RGB values, assign the highest value to Max, and the Smallest to Min
	if ( c.GetR() > c.GetG() )
	{
		Max = c.GetR();
		Min = c.GetG();
	}
	else
	{
		Max = c.GetG();
		Min = c.GetR();
	}
	
	if ( c.GetB() > Max )
	{
		Max = c.GetB();
	}
	else if ( c.GetB() < Min )
	{
		Min = c.GetB();
	}
	Diff = Max - Min;
	Sum = Max + Min;
	// Luminance - a.k.a. Brightness - Adobe photoshop uses the logic that the
	// site VBspeed regards (regarded) as too primitive = superior decides the 
	// level of brightness.
	hsl.l = (double)Max/255;
	// Saturation
	if ( Max == 0 ) hsl.s = 0; // Protecting from the impossible operation of division by zero.
	else hsl.s = (double)Diff/Max; // The logic of Adobe Photoshops is this simple.
	// Hue  R is situated at the angel of 360 eller noll degrees; 
	//   G vid 120 degrees
	//   B vid 240 degrees
	double q;
	if ( Diff == 0 ) q = 0; // Protecting from the impossible operation of division by zero.
	else q = (double)60/Diff;

	if ( Max == c.GetR() )
	{
		if ( c.GetG() < c.GetB() ) 
		{
			hsl.h = (double)(360 + q * (c.GetG() - c.GetB()))/360;
		}
		else
		{
			hsl.h = (double)(q * (c.GetG() - c.GetB()))/360;
		}
	}
	else if ( Max == c.GetG() )
	{
		hsl.h = (double)(120 + q * (c.GetB() - c.GetR()))/360;
	}
	else if ( Max == c.GetB() )
	{
		hsl.h = (double)(240 + q * (c.GetR() - c.GetG()))/360;
	}
	else
	{
		hsl.h = 0.0;
	}

	return hsl; 
}

Color SetBrightness(Color c, double brightness) 
{ 
	Color_HSL hsl = RGB_to_HSL(c); 
	hsl.l=brightness; 
	return HSL_to_RGB(hsl); 
}



class GDIPlusWindow : public UBaseWindow
{
public:
    GDIPlusWindow()
    : UBaseWindow(NULL, NULL)
    {
        setTitle(_T("GDIPlus Window"));
    }

    virtual BOOL onCreate()
    {
        this->setIconBig(IDI_APP);
        
        return UBaseWindow::onCreate();
    }

    virtual void onDraw(HDC hdc)
    {
        Graphics graphics(hdc);
        
        //USmartDC dc(hdc);
        huys::URectL rect;
        this->getClientRect(rect);
        
        
        huys::URectL rect1(200, 220, 620, 270);

        //
        //LinearGradientBrush lgb(Point(0, 0), Point(rect.width(), 0), Color::Blue, Color::Green);
        //
        //graphics.FillRectangle(&lgb, 0, 0, rect.right(), rect.bottom());
        
        //
        //SolidBrush sb(Color::Red);
        //graphics.FillEllipse(&sb, 20, 20, 100, 50);

        //
        //graphics.FillRectangle(&sb, 100, 100, 200, 200);

		COLORREF rainbow[] = {
			Color::Red,
			Color::Orange,
			Color::Yellow,
			Color::Green,
			Color::Blue,
			Color::Indigo,
			Color::Violet };
	    
		for (int i=0; i<7; ++i)
		{ 
			Color c;
			c.SetFromCOLORREF(rainbow[i]);
			SolidBrush br(SetBrightness(c, 0.7)); 

			graphics.FillRectangle( &br, 
									(float)(rect1.width()*i/7 + rect1.left()),
									(float)(rect1.top()),
									(float)(rect1.width()/7),
									(float)(rect1.height())); 

		}  

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
    UGDIPlusHelper _gh;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new GDIPlusWindow);
    app.init(hInstance);

    return app.run();
}

