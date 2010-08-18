/*
 * =====================================================================================
 *
 *       Filename:  ado_cxx.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2010-8-15 20:22:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include <windows.h>

#import "C:\Program files\Common Files\System\Ado\msado15.dll" rename("EOF", "ADOEOF")

std::string outputashex(unsigned long l)
{
    char buffer[1024];
    ::itoa(l, buffer, 16);
    return buffer;
} ;

int main()
{
    HRESULT hr;
    CoInitialize(NULL);

    try
    {
        ADODB::_ConnectionPtr connection;
        hr = connection.CreateInstance(__uuidof(ADODB::Connection));
        if (FAILED(hr))
        {
            throw _com_error(hr);
        }

        ADODB::_RecordsetPtr recordset;
        hr = recordset.CreateInstance(__uuidof(ADODB::Recordset));
        if (FAILED(hr))
        {
            throw _com_error(hr);
        }

        connection->CursorLocation = ADODB::adUseClient;
        connection->Open(L"Provider=sqloledb;Data Source=fifa;"
            L"Initial Catalog=test;User Id=testsa;Password=testsa;", L"",
            L"", ADODB::adConnectUnspecified);

        recordset->Open("CREATE TABLE mytable (value NVARCHAR(255))",
            connection.GetInterfacePtr(), ADODB::adOpenForwardOnly,
            ADODB::adLockReadOnly, ADODB::adCmdText);

        recordset->Open("INSERT INTO mytable VALUES ('Hello')",
            connection.GetInterfacePtr(), ADODB::adOpenForwardOnly,
            ADODB::adLockReadOnly, ADODB::adCmdText);
        recordset->Open("INSERT INTO mytable VALUES ('Goodbye')",
            connection.GetInterfacePtr(), ADODB::adOpenForwardOnly,
            ADODB::adLockReadOnly, ADODB::adCmdText);

        recordset->Open("SELECT * from mytable",
            connection.GetInterfacePtr(),
            ADODB::adOpenForwardOnly, ADODB::adLockReadOnly,
            ADODB::adCmdText);

        while(!recordset->ADOEOF)
        {
            _variant_t var;
            var = recordset->Fields->GetItem(L"value")->GetValue();
            std::cout << static_cast<char *>(_bstr_t(var.bstrVal))
                << std::endl;
            recordset->MoveNext();
        };

        recordset->Close();
        recordset->Open("DROP TABLE mytable", connection.GetInterfacePtr(),
            ADODB::adOpenForwardOnly, ADODB::adLockReadOnly,
            ADODB::adCmdText);
    }
    catch(_com_error &e)
    {
        std::cerr << ::outputashex(hr) << ":"
            << static_cast<char *>(e.Description()) << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled Exception" << std::endl;
    };


    std::cout << "Press any key to continue...";
    std::cin.ignore();

    return 0;
}

