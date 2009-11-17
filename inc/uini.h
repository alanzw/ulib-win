// Sample:
// *******
//   include "global.inc"
//   [SectionName]
//   key=value ; comment here
//
//

#ifndef U_INI_H
#define U_INI_H

#include "ulib.h"

#ifdef HU_DEBUG
  #define UINI_LOG  printf
#else
  //#define UINI_LOG(...)
	#define UINI_LOG
#endif // HU_DEBUG

class ULIB_API UIni
{
public:
    enum ParseError {
        PE_NONE = 0,
        PE_ERROR,
        PE_EMPTY
    };

    enum ParseState {
        PS_NONE = 0,
        PS_PCOMMENT,
        PS_LINECONT
    };

    enum EntryType {
        ET_INCLUDE = 0,
        ET_SECNAME,
        ET_KEYVALUE,
        ET_LCOMMENT,
        ET_PCOMMENT
    };

    enum QueryError {
        QE_FOUND = 0,
        QE_NOFOUND,
        QE_UNKOWN
    };

    class UIni_Exception
    {
    public:
        UIni_Exception(const char *sError) {}
        void logWhat()
        {
            LOG_STRING("Failed with %s.", _sError);
        }
        const char * what() const
        {
            return _sError;
        }
    private:
        const char *_sError;
    };

public:
    UIni();
    ~UIni();

    bool parseFile(const char *sFilename);
    bool parseIncFile(const char *sFilename);
    bool parse();

    //
    QueryError query(const char *name, const char *&value);

    ParseState parseLine(const char *sLine);

    void printAll();
    void deleteAll();

//private:
    //
    bool open(const char *sFilename);
    //
    bool readFile(const char *sFilename);

    bool read();

    bool writeFile(const char *sFilename, bool bStripInc = false);

    bool write();

    bool close();
private:
    class BaseEntry
    {
    public:
        BaseEntry(): _prev(0), _next(0) {}
        virtual ~BaseEntry() {};

        bool isFirst() const
        { return 0==_prev; }

        bool isLast() const
        { return 0==_next; }


        void setPrev(BaseEntry *prev)
        { _prev = prev; }

        void setNext(BaseEntry *next)
        { _next = next; }

        BaseEntry *next() const
        { return _next; }

        BaseEntry *prev() const
        { return _prev; }

        virtual bool equals(const char *sText)
        { return false; }

        virtual void get(const char *sp, const char *sp_end) = 0;
        virtual void print() = 0;
        virtual EntryType type() = 0;
        virtual bool write(FILE *f) = 0;
    private:
        BaseEntry *_prev;
        BaseEntry *_next;
    };

    class KeyValueEntry : public BaseEntry
    {
    public:
        KeyValueEntry() : BaseEntry()
        {
            ::ZeroMemory(_key, sizeof(_key));
            ::ZeroMemory(_value, sizeof(_value));
        }

        ~KeyValueEntry()
        {}

        int toInt()
        {
            if (strcmp("0", _value) == 0)
            {
                return 0;
            }

            try
            {
                int ret = atoi(_value);

                if (INT_MAX==ret || INT_MIN==ret)
                {
                    throw UIni_Exception("exceeding int limit");
                }
            }
            catch (UIni_Exception &e)
            {
                e.logWhat();
            }
        }

        double toDouble()
        {
            return atof(_value);
        }

        /*
        void setKey(const char *key)
        { _key = key; }

        void setValue(const char *value)
        { _value = value; }

        void setSecname(const char *secname)
        { _secname = secname; }
        */
        const char *key() const
        { return _key; }

        const char *value() const
        { return _value; }

        bool isEnable() const
        {
            return (0 == strcmp("enable", _value) ||
                0 == strcmp("yes", _value));
        }

        /*
        const char *secname() const
        { return _secname; }
        */
        void get(const char *sp, const char *sp_end)
        {
            const char *p = strchr(sp, '=') - 1;
            const char *px = strchr(sp, ' ') - 1;
            if (px < p)
            {
                p = px;
            }
            int nSize = p - sp + 1;
            ::CopyMemory(_key, sp, nSize);
            ++p;
            while (' ' == p[0] || '=' == p[0])
            {
                ++p;
            }
            nSize = sp_end - p + 1;
            ::CopyMemory(_value, p, nSize);

            UINI_LOG("%s --> %s\n", _key, _value);

        }

        void print()
        {
            printf("%s --> %s\n", _key, _value);
        }
        virtual EntryType type()
        {
            return ET_KEYVALUE;
        }

        virtual bool write(FILE *f)
        {
            fprintf(f, "%s=%s\n", _key, _value);
            return true;
        }
    private:
        char _key[MAX_PATH];
        char _value[MAX_PATH];
        //const char *_secname;
    };

    class IncludeEntry : public BaseEntry
    {
    public:
        IncludeEntry() : BaseEntry()
        {
            ::ZeroMemory(_sFilename, sizeof(_sFilename));
        }

        ~IncludeEntry() {}

        void get(const char *sp, const char *sp_end)
        {
            int nSize = sp_end-sp + 1;
            ::CopyMemory(_sFilename, sp, nSize);
            *(_sFilename+nSize) = '\0';

            UINI_LOG("include --> %s\n", _sFilename);
        }

        const char *getIncFilename() const
        {
            return _sFilename;
        }

        void print()
        {
            printf("include --> %s\n" , _sFilename);
        }

        virtual EntryType type()
        {
            return ET_INCLUDE;
        }

        virtual bool write(FILE *f)
        {
            fprintf(f, ";include %s\n", _sFilename);
            return true;
        }
    private:
        char _sFilename[MAX_PATH];
    };

    class CommentEntry : public BaseEntry
    {
    public:
        CommentEntry() : BaseEntry(),
            _sText(0), _size(MAX_PATH)
        {
            _sText = new char[_size];
        }

        ~CommentEntry()
        {
            delete [] _sText;
        }

        void get(const char *sp, const char *sp_end)
        {
            int nSize = sp_end - sp  + 1;

            if (nSize > _size)
            {
                _size = nSize + 2;
                delete [] _sText;
                _sText = new char[_size];
            }
            ::CopyMemory(_sText, sp, nSize);
            *(_sText+nSize) = '\0';

             UINI_LOG("comment --> %s\n", _sText);
        }

        virtual bool equals(const char *sText)
        {
            return (0 == strcmp(sText, _sText));
        }

        void print()
        {
            printf("comment --> %s\n" , _sText);
        }

        virtual EntryType type()
        {
            return ET_LCOMMENT;
        }

        const char *text() const
        {
            return _sText;
        }

        virtual bool write(FILE *f)
        {
            fprintf(f, ";%s\n", _sText);
            return true;
        }

    private:
        char *_sText;
        int _size;
    };

    class SecnameEntry : public BaseEntry
    {
    public:
        SecnameEntry() : BaseEntry()
        {
            ::ZeroMemory(_sSecname, sizeof(_sSecname));
        }

        ~SecnameEntry() {}

        void get(const char *sp, const char *sp_end)
        {
            int nSize = sp_end-sp + 1;
            ::CopyMemory(_sSecname, sp, nSize);
            *(_sSecname+nSize) = '\0';

            UINI_LOG("section --> %s\n", _sSecname);
        }

        void print()
        {
            printf("section --> %s\n" , _sSecname);
        }

        virtual EntryType type()
        {
            return ET_SECNAME;
        }

        virtual bool write(FILE *f)
        {
            fprintf(f, "[%s]\n", _sSecname);
            return true;
        }
    private:
        char _sSecname[MAX_PATH];
    };

    BaseEntry *_first;
    BaseEntry *_last;

public:
    BaseEntry *get(const char *name);

private:
    const char *_sFilename;

    ParseState _parse_state;

    FILE *m_pFile;
private:
    int separateLine(const char *sLine);

    BaseEntry *addEntry(EntryType ei, const char *sp, const char *sp_end);
    BaseEntry *appendEntry(BaseEntry *pEntry);

    enum FileState {
        FS_OK,
        FS_ERROR,
        FS_EOF
    };

    FileState getline(char *lpBuf, int nMax);
};

#endif // U_INI_H

