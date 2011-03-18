#include <windows.h>
#include <tchar.h>

#ifdef __GNUC__
#include <gdiplus/GdiPlus.h>
#else
#include <gdiPlus.h>
#endif

using namespace Gdiplus;

#include "ugdiplus.h"

class UGDIPlus_Stuff
{
public:
    void startGDIPlus()
    {
        // Initialize GDI+.
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    }

    void stopGDIPlus()
    {
        GdiplusShutdown(gdiplusToken);
    }

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;  
};

UGDIPlusHelper::UGDIPlusHelper()
: ps(0)
{
    ps = new UGDIPlus_Stuff();
    ps->startGDIPlus();
}

UGDIPlusHelper::~UGDIPlusHelper()
{
    ps->stopGDIPlus();
    delete ps;
    ps = 0;
}


/*****************************************************
返回值为-1表示失败，其他为成功
******************************************************/
int GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
	int nRet = -1;
	ImageCodecInfo * pCodecInfo = NULL;
	UINT nNum = 0,nSize = 0;
	GetImageEncodersSize(&nNum,&nSize);
	if (nSize<0)
	{
		return nRet;
	}
	pCodecInfo = new ImageCodecInfo[nSize];
	if (pCodecInfo==NULL)
	{
		return nRet;
	}
	GetImageEncoders(nNum,nSize,pCodecInfo);
	for (UINT i=0; i<nNum; i++)
	{
		if (wcscmp(pCodecInfo[i].MimeType,format)==0)
		{
			*pClsid = pCodecInfo[i].Clsid;
			nRet = i;

			delete[] pCodecInfo;
			return nRet;
		}
		else
		{
			continue;
		}
	}
	delete[] pCodecInfo;
	return nRet;
}



/*********************************
format: 
bmp转为jpg，format为image/jpeg,
jpg转为bmp,format为image/bmp

**********************************/
BOOL ConvertPic(const WCHAR *format, LPCWSTR strDst, LPCWSTR strSrc)
{
	BOOL bConvert = false;
	CLSID clsid;
	int nRet = 0;
	nRet = GetEncoderClsid(format,&clsid);  //得到CLSID
	//USES_CONVERSION;
	if (nRet>=0)
	{
		Image image(strSrc);
		image.Save(strDst,&clsid,NULL);
		bConvert = true;
	}
	return bConvert;
}

BOOL CombinePic(const WCHAR *format, LPCWSTR strDst,
				LPCWSTR strPic1, LPCWSTR strPic2)
{
	BOOL bCombine = false;
	int nRet = 0;
	CLSID clsid;
	nRet = GetEncoderClsid(format,&clsid);
	if (nRet>=0)
	{
		//USES_CONVERSION;
		Bitmap bmp1(strPic1);
		Bitmap bmp2(strPic2);

		int nWidth = 0, nHeight = 0;
		nWidth = bmp1.GetWidth();   //假设两图片大小同
		nHeight = bmp1.GetHeight();
		Bitmap bmpCombine(2*nWidth,nHeight);  //高不变，宽*2，水平合并
		Graphics * pG = NULL;
		pG = Graphics::FromImage(&bmpCombine);
		if (pG!=NULL)
		{
			pG->DrawImage(&bmp1,0,0);
			pG->DrawImage(&bmp2,nWidth,0);
			bmpCombine.Save(strDst,&clsid,NULL);
		}
	}
	return bCombine;
}
