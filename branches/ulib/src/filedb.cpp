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
		AfxMessageBox("�ڴ�������!");
		return;
	}

	strcpy(m_pRecord->data.name, aData.name);
	strcpy(m_pRecord->data.description, aData.description);

	if (m_pHead == 0) // ���ĵ�
	{
		m_pHead = m_pEnd = m_pRecord;
		m_pRecord->prior = m_pRecord->next = 0;
	}
	else // ��ӵ�����β
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
		//����ҵ�Ҫɾ���ļ�¼
		if(!strcmp(m_pRecord->data.name,name))
		{
			//���Ҫɾ�����������ĵ�һ��
			if(m_pRecord->prior)
			{
				//���Ҳ�����������һ��
				if(m_pRecord->next)
				{
					//���������ɾ����¼
					m_pRecord->next->prior = m_pRecord->prior;
					m_pRecord->prior->next = m_pRecord->next;
				}

				//��������ĵ�һ�������������һ��
				else
				{
					//���������ɾ��β��
					m_pRecord->prior->next = 0;
					m_pEnd = m_pRecord->prior;
				}
			}

			//���Ҫɾ������������ĵ�һ��
			else
			{
				//���Ҳ�����������һ��
				if(m_pRecord->next)
				{
					//���������ɾ������
					m_pRecord->next->prior = 0;
					m_pHead = m_pRecord->next;
				}
				//�����������β���������ֻ��һ��
				else
				{
					m_pHead = m_pEnd = 0;
				}
			}
			return;
		}
		m_pRecord = m_pRecord->next;
	}
	AfxMessageBox("û���ҵ���ؼ�¼!");
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
