#ifndef U_GUIEVENT_H
#define U_GUIEVENT_H


class UGUIEvent
{
public:
    enum EvtID {
        CloseEvt = WM_CLOSE
    };
public:
    EvtID id;
    
    explicit UGUIEvent(EvtID x)
    : id(x)
    {}
    
    explicit UGUIEvent(UINT x)
    : id(static_cast<EvtID>(x))
    {}
    
    bool operator==(const UGUIEvent & e)
    {
        return this->id == e.id;
    }
    
    const UGUIEvent & operator=(const UGUIEvent &e)
    {
        this->id = e.id;
        return *this;
    }
};

class EventHandler
{
public:
    EventHandler(void (*pf)())
    : _pf(pf)
    {}
    
    void handle(const UGUIEvent &e)
    {
        _pf();
    }
    
    const EventHandler & operator=(const EventHandler &eh)
    {
        this->_pf = eh._pf;
        return *this;
    }
    
    void (*_pf)();
};

#endif // U_GUIEVENT_H
