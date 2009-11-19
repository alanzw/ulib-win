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
        return 0;
    }
protected:
private:
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

class UXMLParser
{
public:
    UXMLParser();
    ~UXMLParser();

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
protected:
private:
    UXMLString _buffer;
private:
    void extractTags()
    {

    }

    bool isCommnent()
    {
        return false;
    }

    bool isElement()
    {
        return false;
    }

    bool parse(const char *sText)
    {
        return true;
    }

    bool parse(const UXMLString *xstr)
    {
        return true;
    }
};

class UXMLDocument
{
public:
	UXMLDocument();
	~UXMLDocument();

private:
	UXMLParser _parser;

private:
	bool read();
	bool parse();
	bool writeNew();
	bool writeBack();
};

}; // namespace XML

}; // namespace huys

#endif // U_XML_PARSER_H
