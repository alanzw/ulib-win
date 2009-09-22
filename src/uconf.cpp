#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ios_base;
using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::getline;

#include "uconf.h"

UConfig::UConfig(const char *filename/*= "default.cfg"*/)
: m_cfg_file(filename),
  m_file_buffer(""),
  bFileReady(false)
{
}

UConfig::~UConfig()
{}

int UConfig::setFile(const char *filename)
{
    m_cfg_file = filename;
    return 0;
}

UConfig::Option_t UConfig::getOption(const string &optName)
{
    string optionValue = getOptionString(optName);

    return checkOptionValue(optionValue);
}

int UConfig::writeNewFile(const char *filename)
{
    ofstream ofs(filename, ios_base::out);

    if (!ofs)
    {
        // Error
        cerr << "Error: cannot write to file " << filename << endl;
        return 1;
    }

    ofs << m_file_buffer << endl;

    ofs.close();

    return 0;
}

int UConfig::read()
{
    if (bFileReady)
        return 0;

#ifdef HU_DEBUG
    cout << "--> Reading file: " << m_cfg_file << endl;
#endif // HU_DEBUG

    ifstream ifs(m_cfg_file.c_str(), ios_base::in);

    if (!ifs)
    {
        // Error
        cerr << "Error: cannot open file " << m_cfg_file << endl;
        return 1;
    }

    string tmp;
    while( getline(ifs,tmp) )
    {
        if (tmp[0] == '#' || tmp[0] == '\0' )
        {
            continue; // ignore
        }
        else
        {
            m_file_buffer += tmp;
            m_file_buffer.append("\n");
        }
    }

    bFileReady = true;

    ifs.close();

    return 0;
}

int UConfig::writeBack()
{
    return this->writeNewFile(m_cfg_file.c_str());
}

string UConfig::getOptionString(const string &optName)
{
    if (!bFileReady)
    {
        this->read();
    }

#ifdef HU_DEBUG
    cout << "--> Search For: " << optName << endl;
#endif // HU_DEBUG

    string optionValue = "";
    string searchStr = optName;
    searchStr.append("=");
    //string::size_type location_start = m_file_buffer.find(searchStr, 0);
    location_start = m_file_buffer.find(searchStr, 0);
    if (location_start == string::npos)
    {
        cout << "Cannot find the option: " << optName << endl;
        //optionValue.clear();
        optionValue = "";
        return optionValue;
    }

    //string::size_type location_end;
    location_end = m_file_buffer.find_first_of('\n', location_start);
    string::size_type str_length = location_end - location_start;

    string optStr;
    optStr = m_file_buffer.substr(location_start, str_length);

    #ifdef HU_DEBUG
    cout << "Option_String: " << optStr << endl;
    #endif // HU_DEBUG


    if (!optStr.compare(searchStr))
    {
        cout << "Option is not set!" << endl;
        optionValue = "\n";
        return optionValue;
    }

    value_start = optStr.find('=')+1;
    value_len = location_end - value_start;

    optionValue = optStr.substr(value_start);

    #ifdef HU_DEBUG
    cout << "Option_Value: " << optionValue << endl;
    #endif // HU_DEBUG

    return optionValue;
}



int UConfig::getOption(const string &optName, int &value)
{
    string optionValue = getOptionString(optName);

    if (checkOptionValue(optionValue) == NUMVALUE)
    {
        istringstream iss(optionValue);
        iss >> value;
    }
    else
    {
        value = 0;
    }


    return value;
}

double UConfig::getOption(const string &optName, double &value)
{
    string optionValue = getOptionString(optName);

    if (checkOptionValue(optionValue) == NUMVALUE )
    {
        istringstream iss(optionValue);
        iss >> value;
    }
    else
    {
        value = 0.0;
    }

    return value;
}

const char *UConfig::getOption(const string &optName, string &value)
{
    string optionValue = getOptionString(optName);

    if (checkOptionValue(optionValue) == STRVALUE )
    {
        value = optionValue;
    }
    else
    {
        value = "";
    }

    return value.c_str();
}

UConfig::Option_t UConfig::checkOptionValue(const string &optValue)
{
    if (optValue.empty())
    {
#ifdef HU_DEBUG
        cout << "UConfig::Option_type: unknown!" << endl;
#endif // HU_DEBUG
        return UConfig::UNKNOWN;
    }

    if (!optValue.compare("\n"))
    {
#ifdef HU_DEBUG
        cout << "UConfig::Option_type: NoValue!" << endl;
#endif // HU_DEBUG
        return UConfig::NOVALUE;
    }

    if (!this->upper(optValue).compare("DISABLE"))
    {
        #ifdef HU_DEBUG
        cout << "UConfig::Option_type: disable!" << endl;
        #endif // HU_DEBUG

        return UConfig::DISABLE;
    }

    if (!this->upper(optValue).compare("ENABLE"))
    {
        #ifdef HU_DEBUG
        cout << "UConfig::Option_type: enable!" << endl;
        #endif // HU_DEBUG
        return UConfig::ENABLE;

    }

    if (optValue.find_first_of("1234567890.")==0)
    {
        #ifdef HU_DEBUG
        cout << "UConfig::Option_type: A numerical value!" << endl;
        #endif // HU_DEBUG
        return UConfig::NUMVALUE;
    }

#ifdef HU_DEBUG
    cout << "UConfig::Option_type: A string value!" << endl;
#endif // HU_DEBUG
    return UConfig::STRVALUE;

}

//
string UConfig::upper(const string &str)
{
    string tmp = str;
    for (unsigned int i=0; i<str.length(); i++)
    {
        tmp[i] = toupper(tmp[i]);
    }
    return tmp;
}

bool UConfig::changeOptionValue( const string &optName, string &newValue )
{
    string oldValue = getOptionString(optName);

    if (oldValue.compare("") == 0 || oldValue.compare(newValue) == 0)
    {
        return false;
    }

    cout << "debug" << endl;
    m_file_buffer.replace(value_start, value_len, newValue);

    return true;
}

bool UConfig::changeOptionValue( const char *optName, const char *newValue )
{
    string oldValue = getOptionString(optName);

    if (oldValue.compare("") == 0 || oldValue.compare(newValue) == 0)
    {
        return false;
    }

    m_file_buffer.replace(location_start+value_start, value_len, newValue);

    return true;
}

const char * UConfig::getCfgFilename() const
{
    return m_cfg_file.c_str();
}

const char * UConfig::getFileBuf() const
{
    return m_file_buffer.c_str();
}

