#include <windows.h>
#include <tchar.h>

#include "ufiledb.h"

#include <fstream>
#include <cassert>
#include <cstring>
using namespace std;

UFileDB::UFileDB( const char *sFilename /*= "default.bin"*/ )
: m_pHead(0), m_pEnd(0), m_pRecord(0)
{
    assert(lstrlen(sFilename) < MAX_PATH);
    lstrcpy(m_sFilename, sFilename);
}

UFileDB::~UFileDB()
{}

bool UFileDB::read()
{
    m_pHead = 0;
    m_pEnd = 0;

    //
    fstream file;
    file.open(m_sFilename, ios::in|ios::binary);
    //
    file.seekg(0, ios::end);
    if (0 == file.tellg())
    {
        return false;
    }

    //
    long filesize = file.tellg();
    //
    long recordsize = sizeof(DBData);
    //
    long recordnum = filesize/recordsize;

    //
    file.seekg(0, ios::beg);

    //
    for (int i=0; i<recordnum; i++)
    {
        m_pRecord = new DBRecord;
        file.read((char *)&m_pRecord->data, sizeof(DBData));

        //
        if (m_pHead == 0)
        {
            m_pRecord->index = 1;
            m_pHead = m_pRecord;
            m_pEnd = m_pRecord;
            m_pHead->prior = 0;
            m_pHead->next = 0;
        }
        else
        {
            m_pRecord->index = m_pEnd->index + 1;
            m_pEnd->next = m_pRecord;
            m_pRecord->prior = m_pEnd;
            m_pEnd = m_pRecord;
            m_pEnd->next = 0;
        }
    }

    file.close();
    return true;
}

bool UFileDB::add( const DBData &aData )
{
    m_pRecord = new DBRecord;
    //
    if(!m_pRecord)
    {
        return false;
    }

    lstrcpy(m_pRecord->data.name, aData.name);
    lstrcpy(m_pRecord->data.description, aData.description);

    if (m_pHead == 0) // Empty File
    {
        m_pRecord->index = 1;
        m_pHead = m_pEnd = m_pRecord;
        m_pRecord->prior = m_pRecord->next = 0;
    }
    else // Append to the tail
    {
        m_pRecord->index = m_pEnd->index + 1;
        m_pRecord->prior = m_pEnd;
        m_pEnd->next = m_pRecord;
        m_pEnd = m_pRecord;
        m_pEnd->next=0;
    }
    return true;
}

bool UFileDB::change( const DBData &aData )
{
    m_pRecord = m_pHead;

    while (0 != m_pRecord)
    {
        if(!lstrcmp(m_pRecord->data.name, aData.name))
        {
            lstrcpy(m_pRecord->data.description, aData.description);
            return true;
        }
        m_pRecord = m_pRecord->next;
    }
    return false;
}

bool UFileDB::save()
{
    fstream file;
    file.open(m_sFilename, ios::out|ios::binary);
    for(m_pRecord=m_pHead; m_pRecord!=0; m_pRecord = m_pRecord->next)
    {
        file.write((char*)&m_pRecord->data, sizeof(DBData));
    }
    file.close();

    return true;
}

bool UFileDB::saveAs( const char *sNewFilename )
{
    fstream file;
    file.open(sNewFilename, ios::out|ios::binary);
    for(m_pRecord=m_pHead; m_pRecord!=0; m_pRecord = m_pRecord->next)
    {
        file.write((char*)&m_pRecord->data, sizeof(DBData));
    }
    file.close();

    return true;
}


