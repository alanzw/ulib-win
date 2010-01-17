/*
 * =====================================================================================
 *
 *       Filename:  uxmlpaser.h
 *
 *    Description:  A simpe Parser for XML
 *
 *        Version:  1.0
 *        Created:  2009-6-21 22:29:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

/*
 *  XML stands for eXtensible Markup Language.
 *  XML is designed to transport and store data.
 *  XML is important to know, and very easy to learn.
 *
 */

#ifndef U_XML_PARSER_H
#define U_XML_PARSER_H

#include "adt/ustring.h"

namespace huys
{

namespace XML
{


/*!
   Disable copy and assign.
*/
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &);            \
    void operator=(const TypeName &)

#define DISALLOW_EVIL_CONSTRUCTOR(TypeName) \
    DISALLOW_COPY_AND_ASSIGN(TypeName)

/*! The enum type of XML format.
 *  The documentation block cannot be put after the enum!
 */
enum XML_Form {
    XF_WELL = 0x00,  /**< for well format XML docs */
    XF_BAD  = 0x01   /**< for bad format XML docs */
};

typedef huys::ADT::UStringAnsi UXMLString;


/*
 *  A -- B -- C -- D -- ... -- Z
 *       |
 *       B1
 *       |
 *       B2
 */

class UXMLNode
{
public:
    UXMLNode()
    : _parent(0), _firstChild(0), _lastChild(0)
    {}

    ~UXMLNode()
    {}

    bool isRoot() const
    {
        return (0 == _parent);
    }

    bool hasChild() const
    {
        return (0 != _firstChild);
    }

    const UXMLNode *parent() const
    {
        return _parent;
    }

    const UXMLNode *firstChild() const
    {
        return _firstChild;
    }

    const UXMLNode *lastChild() const
    {
        return _lastChild;
    }

    void setParent(UXMLNode * parent)
    {
        _parent = parent;
    }

    void setFirstChild(UXMLNode *node)
    {
        _firstChild = node;
    }

    void setLastChild(UXMLNode *node)
    {
        _lastChild = node;
    }
protected:
private:
    UXMLNode *_parent;
    UXMLNode *_firstChild;
    UXMLNode *_lastChild;
};

class UXMLAtrribute
{
public:
    UXMLAtrribute()
    : _prev(0), _next(0)
    {}

    UXMLAtrribute(const char *name, const char *value)
    : _name(name), _value(value),
      _prev(0), _next(0)
    {}

    ~UXMLAtrribute()
    {}

    const char* name() const
    {
        return _name.c_str();
    }

    const char* value() const
    {
        return _value.c_str();
    }

    void setName(const char *name)
    {
        _name = name;
    }

    void setValue(const char *value)
    {
        _value = value;
    }
protected:
private:
    DISALLOW_EVIL_CONSTRUCTOR(UXMLAtrribute);
private:
    UXMLString _name;
    UXMLString _value;

    UXMLAtrribute *_prev;
    UXMLAtrribute *_next;
};

class UXMLElement : public UXMLNode
{
public:
    UXMLElement();
    ~UXMLElement();

    const char* atrribute(const char* name)
    {
        return _atrributes->value.c_str();
    }
protected:
private:
    UXMLString _name;

    UXMLAtrribute *_atrributes;
    int _nAttributeNum;
};

class UXMLText
{
public:
    UXMLText();
    ~UXMLText();
protected:
private:
    UXMLString _text;
};

class UXMLComment : public UXMLNode
{
public:
    UXMLComment() {}
    ~UXMLComment() {}
private:
};

class UXMLHeader : public UXMLNode
{
public:
    UXMLHeader() {}
    ~UXMLHeader() {}
private:
    UXMLString _version;
    UXMLString _encoding;

};

enum XMLNodeType
{
    XML_NT_HEADER = 0,
    XML_NT_COMMNET,
    XML_NT_DTD_HEADER,
    XML_NT_CDATA_HEADER,
    XML_NT_ELEMENT,
    XML_NT_UNKNOWN
};

class UXMLParser
{
public:
    UXMLParser()
    {}

    ~UXMLParser()
    {}

/**
 * Well Formed XML Documents
 *   - XML documents must have a root element
 *   - XML elements must have a closing tag
 *   - XML tags are case sensitive
 *   - XML elements must be properly nested
 *   - XML attribute values must be quoted
 */

    bool checkForm() const
    {
        return true;
    }

    XMLNodeType parse(const char *sText)
    {
        _buffer = sText;

        if (isCommnent())
        {
            printf("%s : Commnet!\n", sText);
            return XML_NT_COMMNET;
        }
        else if (isHeader())
        {
            printf("%s : Header!\n", sText);
            return XML_NT_HEADER;
        }
        else if (isCDataHeader())
        {
            printf("%s : CDataHeader!\n", sText);
            return XML_NT_CDATA_HEADER;
        }
        else if (isDTDHeader())
        {
            printf("%s : DTDHeader!\n", sText);
            return XML_NT_DTD_HEADER;
        }
        else if (isElement())
        {
            printf("%s : Element!\n", sText);
            return XML_NT_ELEMENT;
        }
        else
        {
            printf("%s : Unknown!\n", sText);
            return XML_NT_UNKNOWN;
        }
    }

    bool parse(const UXMLString *xstr)
    {
        return true;
    }

protected:
private:
    UXMLString _buffer;
private:
    void extractTags()
    {

    }

    bool isCommnent()
    {
        if ( _buffer.substr(0, 3) == "<!--")
        {
            return true;
        }
        return false;
    }

    bool isHeader()
    {
        if (_buffer.substr(0,4) == "<?xml")
        {
            return true;
        }
        return false;
    }

    bool isDTDHeader()
    {
        if (_buffer.substr(0,1) == "<!")
        {
            return true;
        }
        return false;
    }

    bool isCDataHeader()
    {
        if (_buffer.substr(0,8) == "<![CDATA[")
        {
            return true;
        }
        return false;
    }

    bool isElement()
    {
        if ( _buffer.at(1) < 127 )
            return isalpha( _buffer.at(1) );
        else
            return true;
    }
};

class UXMLDocument
{
public:
    UXMLDocument(const char *sFilename)
    : m_sFilename(sFilename)
    {}

    UXMLDocument(const UXMLDocument &copy)
    {

    }

    UXMLDocument & operator=(const UXMLDocument &copy)
    {

    }

    virtual ~UXMLDocument()
    {}

    const char *docname() const
    {
        return m_sFilename.c_str();
    }
private:
    UXMLParser _parser;
    UXMLString _buf;
private:
    bool read()
    {
        return true;
    }

    bool parse()
    {
        return true;
    }

    bool writeNew()
    {
        return true;
    }

    bool writeBack()
    {
        return true;
    }
    
    
private:
    UXMLString m_sFilename;
};

}; // namespace XML

}; // namespace huys

#endif // U_XML_PARSER_H
