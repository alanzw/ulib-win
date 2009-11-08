#ifndef U_PROPERTY_SHEET_H
#define U_PROPERTY_SHEET_H

#include "ucontrol.h"

class ULIB_API UPropSheet : public UControl
{
    enum {
        SHEET_NUM_LIMIT = MAXPROPPAGES
    };
public:
    UPropSheet(HWND hParent, HINSTANCE hInst);
    UPropSheet() {}
    ~UPropSheet();

    BOOL create(PROPSHEETPAGE *ppsp, int nPages, LPSTR lpCaption);
private:
    PROPSHEETHEADER m_psh;
    PROPSHEETPAGE *m_ppsp;
};

#endif // U_PROPERTY_SHEET_H
