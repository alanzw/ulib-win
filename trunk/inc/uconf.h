/*
* =====================================================================================
*
*       Filename:  uconf.h
*
*    Description:  Configure File Operator
*
*        Version:  1.0
*        Created:  05/21/2008 01:58:23 AM
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  huys (hu), huys03@gmail.com
*        Company:  HUYS .Inc
*
* =====================================================================================
*/

#ifndef U_CONF_H
#define U_CONF_H

#include "ulib.h"


///
/// @class UConfig
/// @brief Configuration File Parser
///
/// <b>Format:</b><br />
///  # This is a comment line. <br />
///  Option=Value
///
class ULIB_API UConfig
{
public:
    typedef enum tagOptionType {
        DISABLE = 0,
        ENABLE,
        UNKNOWN,
        NOVALUE,
        NUMVALUE,
        STRVALUE
    } Option_t;
public:
    UConfig(const char *filename = "default.cfg");
    ~UConfig();
    ///
    int setFile(const char *filename);
    ///
    Option_t getOption(const string &optName);
    ///
    double getOption(const string &optName, double &value);
    ///
    int getOption(const string &optName, int &value);
    ///
    const char *getOption(const string &optName, string &value);
    ///
    int writeNewFile(const char *filename);
    ///
    int read();
    ///
    int writeBack();
    ///
    bool changeOptionValue(const string &optName, string &newValue);
    ///
    bool changeOptionValue(const char *optName, const char *newValue);
    ///
    const char *getCfgFilename() const;
    ///
    const char *getFileBuf() const;
private: 
    /**
     * @brief  Get value string with the option name
     * @param  const string &optName
     * @return string
     */
    string getOptionString(const string &optName);
    //
    string upper(const string &str);
    //
    Option_t checkOptionValue(const string &optValue);
    //
    string m_cfg_file;
    string m_file_buffer;
    //
    bool bFileReady;
    //
    string::size_type location_start;
    string::size_type location_end;
    //
    string::size_type value_start;
    string::size_type value_len;
};

#endif // U_CONF_H

