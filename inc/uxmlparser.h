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

#include <cstdio>

#include "adt/ustring.h"
#include "adt/uvector.h"

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


enum XMLNodeType
{
    XML_NT_HEADER = 0,
    XML_NT_COMMNET,
    XML_NT_DTD_HEADER,
    XML_NT_CDATA_HEADER,
    XML_NT_ELEMENT,
    XML_NT_UNKNOWN
};

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
    : _parent(0), _firstChild(0), _lastChild(0),
      _type(XML_NT_UNKNOWN)
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
    XMLNodeType _type;
private:
    UXMLNode *_parent;
    UXMLNode *_firstChild;
    UXMLNode *_lastChild;
};

class UXMLAttribute
{
public:
    UXMLAttribute()
    {}

    UXMLAttribute(const char *name, const char *value)
    : _name(name), _value(value)
    {}

    UXMLAttribute(const UXMLAttribute &attr)
    : _name(attr._name), _value(attr._value)
    {}

    ~UXMLAttribute()
    {}

    UXMLAttribute & operator=(const UXMLAttribute &attr)
    {
         _name = attr._name;
         _value = attr._value;
    }

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

    bool operator==(const UXMLAttribute &attr) const
    {
        return _name == attr._name;
    }

    bool operator==(const char * name) const
    {
        return _name == name;
    }
private:
    UXMLString _name;
    UXMLString _value;
};

typedef huys::ADT::UVector<UXMLAttribute> UXMLAttributes;

class UXMLText
{
public:
    UXMLText(const char *sText)
    : _text(sText)
    {

    }
    ~UXMLText()
    {

    }
protected:
private:
    UXMLString _text;
};

class UXMLElement : public UXMLNode
{
public:
    UXMLElement()
    : _text(0)
    {
        _type = XML_NT_ELEMENT;
    }

    ~UXMLElement()
    {
        if (_text)
        {
            delete _text;
        }
    }

    bool setAttribute(const char *name, const char * value)
    {
        if (0 == _attributes.size())
        {
            _attributes.push_back(UXMLAttribute(name, value));
            return true;
        }

        UXMLAttributes::iterator it = _attributes.begin();
        for (; it != _attributes.end(); ++it)
        {
            if (*it == name)
            {
                (*it).setValue(value);
                return true;
            }
        }

        _attributes.push_back(UXMLAttribute(name, value));

        return false;
    }

    void setText(const char *sText)
    {
        if (_text)
        {
            delete _text;
        }
        _text = new UXMLText(sText);
    }

    bool hasAttribute() const
    {
        return 0 != _attributes.size();
    }

    bool hasText() const
    {
        return 0 != _text;
    }
protected:
private:
    UXMLString _name;
    UXMLText *_text;
    UXMLAttributes _attributes;
};

class UXMLComment : public UXMLNode
{
public:
    UXMLComment()
    {
        _type = XML_NT_COMMNET;
    }

    ~UXMLComment() {}
private:
    UXMLString _text;
};

class UXMLHeader : public UXMLNode
{
public:
    UXMLHeader()
    {
        _type = XML_NT_HEADER;
    }
    ~UXMLHeader() {}
private:
    UXMLString _version;
    UXMLString _encoding;
};

class UXMLUnknown : public UXMLNode
{
public:
    UXMLUnknown()
    {
        _type = XML_NT_UNKNOWN;
    }
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
            printf("=================\n");
            printf("Tag: %s\n", extractTags().c_str());
            printf("=================\n");
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
    UXMLString extractTags()
    {
        UXMLString::size_type start = _buffer.find('<') + 1;
        if (' ' == _buffer.at(start))
        {
            ++start;
        }
        UXMLString::size_type end = _buffer.find('>', start) - 1;
        if (-1 != _buffer.find(' ', start, end))
        {
            end = _buffer.find(' ', start, end) - 1;
            
            extractAtrributes(end+2, _buffer.find('>', start) - 1);
        }
        return _buffer.substr(start, end);
    }
    
    void extractAtrributes(UXMLString::size_type s, UXMLString::size_type e)
    {
        if (e <= s)
        {
            printf("==== No attributes there!!!\n====");
            return;
        }
        
        UXMLString name;
        UXMLString value;
        
        UXMLString::size_type ps, pe;

        printf("-------------------------------------------------\n");
        printf("name   value \n");
        printf("==========================\n");
        
        while (-1 != _buffer.find('=', s, e) )
        {
            ps = _buffer.find('=', s, e) -1;
            pe = _buffer.find(' ', ps+2, e);
            
            if (-1 == pe)
            {
                pe = e;
            }
            else
            {
                --pe;
            }
            
            name = _buffer.substr(s, ps);
            value = _buffer.substr(ps+2, pe);

            printf("%s    %s\n", name.c_str(), value.c_str());
            
            s = pe+2;
        }
        
        printf("-------------------------------------------------\n");
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
    : m_sFilename(sFilename),
      _root(0)
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

    bool loadfile()
    {
        return loadfile(m_sFilename);
    }

    bool loadfile(const char * fname)
    {
        FILE *pfile = fopen(fname, "rb");
        if (!pfile)
        {
            return false;
        }

        size_t s = 0;
        fseek(pfile, 0, SEEK_END);
        s = ftell(pfile);
        //rewind(pfile);
        fseek(pfile, 0, SEEK_SET);
        _buffer.reserve(s+1);

        fread((char *)_buffer, s, 1, pfile);

        fclose(pfile);

        _buffer.setLength(s);

        //printf(_buffer.c_str());
        parse();
        
        return true;
    }

    bool savefile(const char *fname)
    {
        return true;
    }

private:
    UXMLParser _parser;
    UXMLString _buffer;
    UXMLNode * _root;
private:
    bool read()
    {
        return true;
    }

    bool parse()
    {
        UXMLString temp;
        UXMLString::size_type ps, pe;
        
        ps = _buffer.find('<');
        pe = _buffer.find('>');
        
        while ( -1 != ps && -1 != pe)
        {
            if ( '/' != _buffer.at(ps+1) )
            {
                temp = _buffer.substr(ps, pe);
        
                _parser.parse(temp.c_str());
            }
            ps = _buffer.find('<', ps+1);
            pe = _buffer.find('>', pe+1);
        
        }
    
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
