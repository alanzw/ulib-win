#include <windows.h>
#include <cstdio>
#include <cctype>

#include "uxmlparser.h"

int main()
{
    const char* demoStart =
        "<?xml version=\"1.0\"  standalone='no' >\n"
        "<!-- Our to do list data -->\n"
        "<ToDo>\n"
        "<!-- Do I need a secure PDA? -->\n"
        "<Item priority=\"1\" distance='close'> Go to the <bold>Toy store!</bold></Item>"
        "<Item priority=\"2\" distance='none'> Do bills   </Item>"
        "<Item priority=\"2\" distance='far &amp; back'> Look for Evil Dinosaurs! </Item>"
        "</ToDo>";


    huys::XML::UXMLDocument doc("hello.xml");

    printf("XML Doc File Name: %s\n", doc.docname());

    huys::XML::UXMLParser parser;

    parser.parse("<?xml version=\"1.0\" standalone='no' >");

    parser.parse("<!--This is A Comment!-->");

    parser.parse("<ToDO>Hello</ToDo>");

    parser.parse("<![CDATA[");

    parser.parse("<!def");

    getchar();
    return 0;
}
