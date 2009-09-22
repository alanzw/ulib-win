#ifndef U_WGET_H
#define U_WGET_H

//+ wget: World Wide Web and get
//
//
//+

class WgetInvoker
{
public:
    WgetInvoker();
    ~WgetInvoker();

    bool invoke();

    bool isExeFileOk() const;

    //
    void setRecursive(bool bRecursive = true);
    void setProxyUsed(bool bProxyOn = true);
    void setContinued(bool bContinued = true);
protected:
private:
    bool m_bRecursive;
    bool m_bProxyUsed;
    bool m_bContinued;
    bool m_bQuiet;
    bool m_bLogEnable;
private:

};


#endif // U_WGET_H
