#ifndef U_HELPER_H
#define U_HELPER_H


#ifndef HAS_SSIZE_T
typedef signed int ssize_t
#define HAS_SSIZE_T
#endif // HAS_SSIZE_T 

/*  Function prototypes  */

void    Error_Quit(const char * msg);
int     Trim      (char * buffer);
int     StrUpper  (char * buffer);
void    CleanURL  (char * buffer);
ssize_t Readline  (int sockd, void *vptr, size_t maxlen);
ssize_t Writeline (int sockd, const void *vptr, size_t n);


/*  Global macros/variables  */

#define LISTENQ          (1024)


#endif  /*  U_HELPER_H  */
