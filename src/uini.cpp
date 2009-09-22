#include <windows.h>
#include <tchar.h>
#include <cctype>

#include <cstdio>
#include <climits>
#include <cassert>

#include "uini.h"

#define MAX_TEXT_LEN MAX_PATH

typedef int (UIni::*PFCMD)(const char *sParam);

struct NameAndCmd
{
    char name[MAX_TEXT_LEN];
    PFCMD pfCmd;
};

//static NameAndCmd *nameCmds [] =
//{
//
//};

const char *LineCommentPre[] =
{
    "//", /*  C++     */
    "#",  /*  SHELL */
    ";",  /*  VIM   */
    0
};


const char *ParaCommentStart = "/*";
const char *ParaCommentEnd   = "*/";

const char cLineContine = '\\';

const char cEqual = '=';

UIni::UIni()
: _first(0), _last(0), m_pFile(0)
{
}

UIni::~UIni()
{
}

int UIni::separateLine(const char *sLine)
{
    const char *sp = sLine;
    const char *sp_end = 0;

    // a empty line
    if ('\0' == sp[0])
    {
        return -2;
    }

    // Skip spaces
    //while (' ' == sp[0] )
    //{
    //    ++sp;
    //}

    // parse to the end
    while ('\0' != sp[0])
    {
        // Skip spaces
        while (' ' == sp[0] )
        {
            ++sp;
        }

        // Entry : include 
        if ( strncmp("include ", sp, 8) ==0 )
        {
            sp += 8;

            // Skip spaces
            while (' ' == sp[0] )
            {
                ++sp;
            }

            // line end reached
            if ('\0' == sp[0])
            {
                return -4;
            }

            //
            if (NULL == strchr(sp, '#'))
            {
                sp_end = strchr(sp, '\0') - 1;
            }
            else
            {
                sp_end = strchr(sp, '#') - 1;
            }
            UINI_LOG("***%s", sp);
            UINI_LOG("--> Add include\n");
            this->addEntry(ET_INCLUDE, sp, sp_end);
            //
            sp = sp_end + 1;
            continue;

        }

        if ( '[' == sp[0] )
        {
            ++sp;
            sp_end = strchr(sp, ']') - 1;
            if (0 == sp_end)
            {
                return -1;
            }
            UINI_LOG("--> Add section\n");
            this->addEntry(ET_SECNAME, sp, sp_end);
            //
            sp = sp_end + 2;
            continue;
        }

        // line comment
        if (';' == sp[0] || '#' == sp[0] )
        {
            ++sp;
            sp_end = strchr(sp, '\0') - 1;

            UINI_LOG("--> Add line comment\n");
            this->addEntry(ET_LCOMMENT, sp, sp_end);
            //
            sp = sp_end + 1;
            continue;
        }

         if (('/' == sp[0] && '/' == sp[1]))
         {
            sp += 2;
            sp_end = strchr(sp, '\0') - 1;

            UINI_LOG("--> Add line comment\n");
            this->addEntry(ET_LCOMMENT, sp, sp_end);
            //
            sp = sp_end + 1;
            continue;
         }
        // paragraph comment
        if ( '/' == sp[0] && '*' == sp[1] )
        {
            _parse_state = PS_PCOMMENT;
            this->addEntry(ET_PCOMMENT, sp, sp_end);
            //
            sp = sp_end + 1;
            continue;
        }

        if ( '*' == sp[0] && '/'== sp[1] )
        {
            _parse_state = PS_NONE;
        }

        if (isalpha(sp[0]))
        {
            sp_end = strchr(sp, '\0') - 1;

            UINI_LOG("***%s", sp);
            UINI_LOG("--> Add key & value\n");
            this->addEntry(ET_KEYVALUE, sp, sp_end);
            sp = sp_end + 1;
            continue;
        }
        //
    }
    return -3;
}

UIni::BaseEntry * UIni::addEntry(UIni::EntryType ei, const char *sp, const char *sp_end)
{
    UIni::BaseEntry *pEntry = 0;

    switch (ei)
    {
    case ET_INCLUDE:
        {
            //
            pEntry = new UIni::IncludeEntry;
            break;
        }
    case ET_SECNAME:
        {
             pEntry = new UIni::SecnameEntry;
            break;
        }
    case ET_LCOMMENT:
        {
             pEntry = new UIni::CommentEntry;
            break;
        }
    case ET_KEYVALUE:
        {
             pEntry = new UIni::KeyValueEntry;
            break;
        }
    default:
        return NULL; // DO nothing.
    }

    pEntry->get(sp, sp_end);

    this->appendEntry(pEntry);

    return pEntry;
}

UIni::BaseEntry * UIni::appendEntry(UIni::BaseEntry *pEntry)
{
    if (0 == _first)
    {
        pEntry->setPrev(0);
        pEntry->setNext(0);
        _first = pEntry;
        _last = 0;
    }
    else
    {
        if (0 == _last)
        {
            pEntry->setPrev(_first);
            pEntry->setNext(0);
            _last = pEntry;
            _first->setNext(pEntry);
        }
        else
        {
            pEntry->setPrev(_last);
            pEntry->setNext(0);
            _last->setNext(pEntry);
            _last = pEntry;
        }
    }
    return pEntry;
}

UIni::FileState UIni::getline( char *lpBuf, int nMax)
{
    assert( 0 != m_pFile );

    fgets(lpBuf, nMax, m_pFile);

    if (ferror(m_pFile))
    {
        return UIni::FS_ERROR;
    }

    if (feof(m_pFile))
    {
        return UIni::FS_EOF;
    }

    int n = strlen(lpBuf);
    lpBuf[n-1] = '\0';

    return UIni::FS_OK;
}

bool UIni::open( const char *sFilename )
{
    m_pFile = fopen(sFilename, "r");
 
    if (0 == m_pFile)
    {
        throw UIni_Exception("fopen");
        return false;
    }
    return true;
}

UIni::ParseState UIni::parseLine( const char *sLine )
{
    static int iCount = 0;
    ++iCount;
    printf("%d\n", iCount);
    separateLine(sLine);
    return UIni::PS_NONE;
}

void UIni::printAll()
{
    UIni::BaseEntry *pEntry = 0;

    for (pEntry = _first; pEntry != 0; pEntry = pEntry->next())
    {
        pEntry->print();
    }

}

bool UIni::parseFile( const char *sFilename )
{
  if (!this->open(sFilename))
  {
      return false;
  }
    char buf[512] = {0};
    while ( UIni::FS_OK == this->getline(buf, 512))
    {
        parseLine(buf);
    }
    this->close();
    return true;
}

bool UIni::close()
{
    if (0 != m_pFile)
    {
        fclose(m_pFile);
        return true;
    }
    return false;
}

void UIni::deleteAll()
{
    UIni::BaseEntry *pEntry = _first;
    UIni::BaseEntry *pEntryNext = 0;
    
    while (0 != pEntry)
    {
        pEntryNext = pEntry->next();
        delete pEntry;
        pEntry = pEntryNext;
    }
   
    _first = 0;
    _last = 0;
}

UIni::QueryError UIni::query( const char *name, char * &value )
{
    return UIni::QE_UNKOWN;
}

UIni::BaseEntry * UIni::get( const char *name )
{
    return 0;
}


