#ifndef U_SQLITE3_MAN_H
#define U_SQLITE3_MAN_H

class USqlite3Man : public UDBMan
{
public:
    USqlite3Man(LPCTSTR lpFileName)
        : UDBMan()
    {
        lstrcpy(m_lpFilename, lpFileName);
    }

    ~USqlite3Man()
    {
        if (DBST_CLOSED != this->state())
        {
            this->close();
        }

    }

    virtual BOOL connect()
    {
        // Check if the filename is valid

        //
        int rc = sqlite3_open(m_lpFilename, &m_db);

        if (rc)
        {
            this->close();
            setState(DBST_ERROR);
            return FALSE;
        }

        setState(DBST_CONNECTED);
        return TRUE;
    }

    virtual BOOL query(LPCSTR sqlText)
    {
        int rc = sqlite3_exec(m_db, sqlText, 0, 0, 0);
        if( rc!=SQLITE_OK ){
            fprintf(stderr, "SQL error: %s!");
            setState(DBST_ERROR);
        }

        return TRUE;
    }

    virtual BOOL close()
    {
        sqlite3_close(m_db);
        setState(DBST_CLOSED);
        return TRUE;
    }
private:
    sqlite3 *m_db;
    TCHAR m_lpFilename[MAX_PATH];
};

#endif // U_SQLITE3_MAN_H
