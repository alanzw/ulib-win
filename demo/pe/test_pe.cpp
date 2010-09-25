#include <windows.h>
#include <stdio.h>



int main(int argc, char **argv)
{
    printf("Sizeof IMAGE_DOS_HEADER: %d\n",  sizeof(IMAGE_DOS_HEADER));

    printf("Press any Key to exit...\n");
    getchar();
    return 0;
}

