#ifndef U_HYPERLINK_H
#define U_HYPERLINK_H

#include "ustatic.h"
#include "colors.h"

class UHyperLink : public UStatic
{
public:
    UHyperLink();
    UHyperLink(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UHyperLink();

    virtual BOOL create();

    virtual BOOL onCtrlColor(WPARAM, LPARAM);
private:
    huys::Color m_clrLink;
    huys::Color m_clrVisited;
    huys::Color m_clrHover;
    huys::Color m_clrBackground;
};

#endif // U_HYPERLINK_H
