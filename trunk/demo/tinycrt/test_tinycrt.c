// TEST.C
int __cdecl printf(const char * format, ...);

int __cdecl main(int argc, char **argv, char **envp)
{
    printf("%d %s", 11, "Hello TinyCRT!\n");
    printf("Hello TinyCRT!\n");
    return 0;
}

int __cdecl __main()
{
    return 0;
}

#ifdef _MSC_VER

//void __chkesp();
//{
//}

#endif
