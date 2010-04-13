#ifndef U_PROPERTY_SHEET_H
#define U_PROPERTY_SHEET_H

#include "ucontrol.h"

#include "adt/uvector.h"

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

    void addPage();
private:
    PROPSHEETHEADER m_psh;
    PROPSHEETPAGE  m_ppsp;

public:
    class UPropPage
    {
    public:
        UPropPage()
        {

        }
    };
};

#endif // U_PROPERTY_SHEET_H
