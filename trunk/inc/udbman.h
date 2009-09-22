#ifndef U_DBMAN_H
#define U_DBMAN_H

#include "ulib.h"

class ULIB_API UDBMan
{
public:
    enum DBState {
        DBST_CLOSED    = 0x00,
        DBST_CONNECTED = 0x01,
        DBST_ERROR     = 0x02
    };
public:
    UDBMan();

    virtual ~UDBMan() = 0;

    virtual BOOL connect() = 0;

    virtual BOOL query(LPCSTR sqlText) = 0;

    virtual BOOL close() = 0;

    DBState state()
    {
        return m_dbst;
    }

    void setState(DBState dbst)
    {
        m_dbst = dbst;
    }

protected:
private:
    DBState m_dbst;
};

#endif // U_DBMAN_H
