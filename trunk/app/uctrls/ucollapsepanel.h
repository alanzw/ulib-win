#include "ucollapse.h"
#include "upanel.h"

class UCollapsePanel : public UPanel
{
public:
    UCollapsePanel(HWND hParent, UINT nID, HINSTANCE hInst)
    : UPanel(hParent, nID, hInst)
    {
        setStyles(SWP_NOREPOSITION|WS_CLIPSIBLINGS);
    }

    ~UCollapsePanel()
    {
        CHECK_PTR(pGBox);
    }

    virtual BOOL create()
    {
        //this->subclassProc();
        BOOL bRet = UPanel::create();
        setMsgWnd(getHParent());
        subclassProc();


        pGBox = new UCollapseGroupBox(m_hSelf, 21111, m_hInstance);
        //pGBox->setPos(8, 8, 150, 150);
        pGBox->create();
        pGBox->setTitle(_T("G1"));

        pGBoxSec = new UCollapseGroupBox(m_hSelf, 21112, m_hInstance);
        //pGBoxSec->setPos(158, 8, 150, 150);
        pGBoxSec->create();
        pGBoxSec->setTitle(_T("G2"));

        this->addControl(pGBox);
        this->addControl(pGBoxSec);

        updateChild();

        return bRet;
    }
private:
    UCollapseGroupBox *pGBox;
    UCollapseGroupBox *pGBoxSec;

    virtual void updateChild()
    {
        //
        pGBox->updateWindowSize();
        pGBoxSec->updateWindowSize();
    }
};
