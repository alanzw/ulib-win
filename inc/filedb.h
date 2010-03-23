#ifndef FILE_DB_H
#define FILE_DB_H

typedef struct data_test {
    char name[10];
    char description[20];
} MyData;

typedef struct record {
    MyData data;
    record *prior;
    record *next;
} MyRecord;

class CFileDB
{
public:
    CFileDB(CString filename = "test.bin");
    ~CFileDB();
    void read();
    void add(const MyData aData);
    void remove(char name[]);
    void change(const MyData aData);
	void output(CString &szOut);
    void save();
private:
    MyRecord *m_pHead;
    MyRecord *m_pEnd;
	MyRecord *m_pRecord;
	CString m_filename;
};

#endif // FILE_DB_H
