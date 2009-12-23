#include <windows.h>
#include <cstdio>

#include "uxmlparser.h"

int main()
{
    const char* demoStart =
        "<?xml version=\"1.0\"  standalone='no' >\n"
        "<!-- Our to do list data -->"
        "<ToDo>\n"
        "<!-- Do I need a secure PDA? -->\n"
        "<Item priority=\"1\" distance='close'> Go to the <bold>Toy store!</bold></Item>"
        "<Item priority=\"2\" distance='none'> Do bills   </Item>"
        "<Item priority=\"2\" distance='far &amp; back'> Look for Evil Dinosaurs! </Item>"
        "</ToDo>";


    huys::XML::UXMLDocument doc("hello.xml");

    printf("XML Doc File Name: %s\n", doc.docname());

    getchar();
    return 0;
}
