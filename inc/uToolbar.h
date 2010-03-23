#ifndef HU_TOOLBAR_H
#define HU_TOOLBAR_H

#include "uLedCtrl.h"

class UToolBar : public CToolBar
{
public:
    UToolBar();
    virtual ~UToolBar();
    ULedCtrl m_subCtrl;
protected:
private:

};

#endif // HU_TOOLBAR_H
