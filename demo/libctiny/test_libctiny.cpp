// Small test program to exercise TINYCRT.  Does nothing useful
//
#include <windows.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

extern "C" int __cdecl printf(const char * format, ...);

int main( int argc, char *argv[] )
{
    int i;

    for ( i = 0; i < argc; i++ )
    {
        printf( "argc: %u \'%s\'\n", i, argv[i] );
    }

    //char * p = new char[10];

    //lstrcpy( p, "Hello" );

    //delete p;

    printf( "%s\n", strlwr( "MyLowerCaseString" ) );

    printf ( "strcmpi: %u\n", strcmpi( "Abc", "abc" ) );

    strrchr( "foo", 'o' );

    return 0;
}

// Declare a simple C++ class with a constructor
class TestClass
{
public:
    TestClass(void)
    {
        printf( "In TestClass constructor\n" );
    }
    ~TestClass(void)
    {
        printf( "In TestClass destructor\n" );
    }
};

// Create a global instance of the class
TestClass g_TestClassInstance;
