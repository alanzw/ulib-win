#if _MSC_VER > 1000
#include "stdafx.h"

#include <fstream>

using namespace std;

#include "filedb.h"

CFileDB::CFileDB(CString filename)
{
	m_pHead = 0;
	m_pEnd = 0;
	m_filename = filename;
}

CFileDB::~CFileDB()
{
    ;
}

void CFileDB::read()
{
	m_pHead = 0;
	m_pEnd = 0;
	fstream file;
	file.open(m_filename, ios::in|ios::binary);
	//
	file.seekg(0, ios::end);
	if (0 == file.tellg())
	{
		AfxMessageBox("Error! Empty File.");
		return;
	}

	//
	long filesize = file.tellg();
	//
	long recordsize = sizeof(MyRecord);
	//
	long recordnum = filesize/recordsize;

	//
	file.seekg(0, ios::beg);

	//
	for (int i=0; i<recordnum; i++)
	{
		m_pRecord = new MyRecord;
		file.read((char *)m_pRecord, sizeof(MyRecord));

		//
		if (m_pHead == 0)
		{
			m_pHead = m_pRecord;
			m_pEnd = m_pRecord;
			m_pHead->prior = 0;
			m_pHead->next = 0;
		}
		else
		{
			m_pEnd->next = m_pRecord;
			m_pRecord->prior = m_pEnd;
			m_pEnd = m_pRecord;
			m_pEnd->next = 0;
		}
	}

	file.close();
}


void CFileDB::add(MyData aData)
{
	//
	m_pRecord = new MyRecord;

	//
	if(!m_pRecord)
	{
		AfxMessageBox("内存分配错误!");
		return;
	}

	strcpy(m_pRecord->data.name, aData.name);
	strcpy(m_pRecord->data.description, aData.description);

	if (m_pHead == 0) // 空文档
	{
		m_pHead = m_pEnd = m_pRecord;
		m_pRecord->prior = m_pRecord->next = 0;
	}
	else // 添加到链表尾
	{
		m_pRecord->prior = m_pEnd;
		m_pEnd->next = m_pRecord;
		m_pEnd = m_pRecord;
		m_pEnd->next=0;
	}

}


void CFileDB::remove(char name[])
{
	m_pRecord = m_pHead;
	while (m_pRecord)
	{
		//如果找到要删除的记录
		if(!strcmp(m_pRecord->data.name,name))
		{
			//如果要删除的项不是链表的第一项
			if(m_pRecord->prior)
			{
				//并且不是链表的最后一项
				if(m_pRecord->next)
				{
					//链表操作，删除记录
					m_pRecord->next->prior = m_pRecord->prior;
					m_pRecord->prior->next = m_pRecord->next;
				}

				//不是链表的第一项，但是链表的最后一项
				else
				{
					//链表操作，删除尾项
					m_pRecord->prior->next = 0;
					m_pEnd = m_pRecord->prior;
				}
			}

			//如果要删除的项是链表的第一项
			else
			{
				//并且不是链表的最后一项
				if(m_pRecord->next)
				{
					//链表操作，删除首项
					m_pRecord->next->prior = 0;
					m_pHead = m_pRecord->next;
				}
				//既是首项，又是尾项，即链表中只有一项
				else
				{
					m_pHead = m_pEnd = 0;
				}
			}
			return;
		}
		m_pRecord = m_pRecord->next;
	}
	AfxMessageBox("没有找到相关记录!");
}

void CFileDB::change(MyData aData)
{
	return;
}

void CFileDB::output(CString &szOut)
{
	CString tempstr1,tempstr2;
	for( m_pRecord = m_pHead; m_pRecord!=0; m_pRecord = m_pRecord->next)
	{
		tempstr1=m_pRecord->data.name;
		tempstr2=m_pRecord->data.description;
		szOut += tempstr1 + "---" + tempstr2 + "---" + "\r\n";
	}
}

void CFileDB::save()
{
    fstream file;
    file.open(m_filename, ios::out|ios::binary);
    for(m_pRecord=m_pHead; m_pRecord!=0; m_pRecord = m_pRecord->next)
        file.write((char*)m_pRecord,sizeof(MyRecord));
	file.close();
}
