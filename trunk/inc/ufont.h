#ifndef U_FONT_H
#define U_FONT_H

#include "uobject.h"

class ULIB_API UFont : public UGDIObject
{
public:
    enum StockFont {
        SystemFixedFont   = SYSTEM_FIXED_FONT,
        SystemFont        = SYSTEM_FONT,
        OemFixedFont      = OEM_FIXED_FONT,
        DeviceDefaultFont = DEVICE_DEFAULT_FONT,
        AnsiVarFont       = ANSI_VAR_FONT,
        AnsiFixedFont     = ANSI_FIXED_FONT
    };


    static int addFontResource(LPCTSTR lpszFilename);
    static int addFontResourcePrivate(LPCTSTR lpszFilename);
    static int addFontResourceNotEnum(LPCTSTR lpszFilename);

    UFont();
    ~UFont();
    
    operator HFONT()
    {
        return (HFONT)m_hObj;
    }
    
    void create();
    
    void setFontHeightRatio(int n);
    
    void setFontFaceName(const TCHAR * lfFaceName);

    bool useLogFont(const LOGFONT &lf);

    bool useStockFont(StockFont sf);

    int draw(HDC hdc, LPCTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat);
    int drawEx(HDC hdc, LPTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat, LPDRAWTEXTPARAMS lpDTParams);

    BOOL textOut(HDC hdc, int x, int y, LPCTSTR lpString, int cbString);
    BOOL extTextOut(HDC hdc, int x, int y, LPCTSTR lpString, int cbString, UINT fuOptions, const LPRECT lpRect, const INT *lpDx);
protected:
    LOGFONT m_lf;
    HFONT createFont();
};

#endif // U_FONT_H

