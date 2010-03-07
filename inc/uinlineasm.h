#ifndef U_INLINE_ASSEMBLY_H
#define U_INLINE_ASSEMBLY_H

namespace huys
{

#ifdef _MSC_VER

inline void swap(int *a, int *b)
{
    __asm {
        mov eax, [a]
        mov ebx, [b]
        mov ecx, [ebx]
        xchg ecx, [eax]
        mov [ebx], ecx
    };
}

inline int times5(int x)
{
   __asm {
       mov eax, [x]
       lea eax, [eax + eax * 4]
       mov [x], eax
   };

   return x;
}

inline double sine(double x)
{
    __asm {
        finit                           ; initialize FPU
        fld qword ptr x                 ; load x onto FPU stack
        fsin                            ; calculate sine
        lea eax, x                      ; get address of x
        fst qword ptr [eax]             ; store sine in x on the stack
    };
    return x;
}

inline int intify( float val )
{
  int rc;
    __asm {
        cvttss2si eax, val
        mov rc, eax
    }
    return rc;
}

float scalarProduct( float* a1, float* a2, int n )
{
//#error Not implemented yet!
#pragma pack(push, 16)
    float ans[4];
#pragma pack(pop)

    register int i;
    if( n >= 8 )
    {
        __asm {
            xorps xmm0, xmm0
        }
        for( i = 0; i < ( n >> 3 ); ++i )
        {
            __asm {
                mov eax, a1
                mov ebx, a2
                movups  xmm1, [eax]
                movups  xmm2, [eax+16]
                movups  xmm3, [ebx]
                movups  xmm4, [ebx+16]
                add eax, 32
                add ebx, 32
                mulps xmm1, xmm3
                mulps xmm2, xmm4
                addps xmm1, xmm2
                addps xmm0, xmm1
            }
        }

        //float *p = &ans[0];
        __asm {
            mov edi, ans
            movaps xmmword ptr [edi], xmm0
        }
        n -= i << 3;
        ans[0] += ans[1] + ans[2] + ans[3];
    }
    else
    {
        ans[0] = 0.0;
        for( i = 0; i < n; ++i )
            ans[0] += a1[i] * a2[i];
    }
    return ans[0];
}

#if defined(_X86_)

inline unsigned long rdtsc(void)
{
    unsigned long int x;
    __asm {
        rdtsc
        mov x, eax
    }
    return x;
}

#elif defined(_AMD64_)

typedef unsigned long long int unsigned long long;

inline unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm {
        rdtsc
        mov lo, eax
        mov hi, edx
    }
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#endif

float addf(float x, float y)
{
    __asm {
        fld dword ptr x
        fld dword ptr y
        fadd
        lea eax, x
        fst dword ptr [eax]
    }
    return x;
}

int sum(int *piarray, int sf)
{
     int isum = 0;
     __asm {
         mov ECX, sf
         dec ECX
         mov ESI, DWORD PTR piarray
         finit
         fild DWORD PTR [ESI]
      next:
         add ESI, 4
         fiadd DWORD PTR [ESI]
         loop next
         fistp DWORD PTR isum
         fwait
    }
    return isum;
}

void set_fpu (unsigned int mode)
{
    __asm {
        fldcw [mode]
    }
}

__inline __declspec(naked) unsigned int getKernel32()
{
    __asm {
        push esi
        push ecx
        mov esi,fs:0
        lodsd
retry:
        cmp [eax],0xffffffff
        je exit//如果到达最后一个节点(它的pfnHandler指向UnhandledExceptionFilter)
        mov eax,[eax]//否则往后遍历,一直到最后一个节点
        jmp retry
exit:
        mov eax, [eax+4]
FindMZ:
        and eax,0xffff0000//根据PE执行文件以64k对界的特征加快查找速度
        cmp word ptr [eax],'ZM'//根据PE可执行文件特征查找KERNEL32.DLL的基址
        jne MoveUp//如果当前地址不符全MZ头部特征,则向上查找
        mov ecx,[eax+0x3c]
        add ecx,eax
        cmp word ptr [ecx],'EP'//根据PE可执行文件特征查找KERNEL32.DLL的基址
        je Found//如果符合MZ及PE头部特征,则认为已经找到,并通过Eax返回给调用者
MoveUp:
        dec eax//准备指向下一个界起始地址
        jmp FindMZ
Found:
        pop ecx
        pop esi
        ret
    }
}

#endif // _MSC_VER

#ifdef __GNUC__

inline void swap(int *a, int *b)
{

    __asm__ __volatile__ (
        "xchg %0, %1"
        : "=r"(*a), "=b"(*b)
        : "r"(*a), "b"(*b)
        : "memory"
    );
}

inline int times5(int x)
{
    __asm__ __volatile__ ( "leal (%1,%1,4), %0"
                           : "=r" (x)
                           : "0" (x)
                         );

    return x;
}

inline double sine(double x)
{
    double *px = &x;
    __asm__ __volatile__ ( "finit\n\t"
                           "fldl (%0)\n\t"
                           "fsin\n\t"
                           "fstl (%1)"
                            : "=r" (px)
                            : "r" (px)
                            : "memory"
                         );

    return x;
}

inline int intify( float val )
{
  int rc;
    __asm__ __volatile__( "cvttss2si %1, %0"
                          : "=r" (rc)
                          : "m" (val) );
    return rc;
}

inline float scalarProduct( float* a1, float* a2, int n )
{
  float ans[4] __attribute__ ((aligned(16)));
  register int i;
  if( n >= 8 )
  {
    __asm__ __volatile__(
        "xorps      %%xmm0, %%xmm0"
        : /* outputs */
        : /* inputs */
        : /* clobbered */ /*"xmm0"*/
        );
    for( i = 0; i < ( n >> 3 ); ++i )
    {
      __asm__ __volatile__(
            "movups     (%0), %%xmm1\n\t"
            "movups     16(%0), %%xmm2\n\t"
            "movups     (%1), %%xmm3\n\t"
            "movups     16(%1), %%xmm4\n\t"
            "add        $32,%0\n\t"
            "add        $32,%1\n\t"
            "mulps      %%xmm3, %%xmm1\n\t"
            "mulps      %%xmm4, %%xmm2\n\t"
            "addps      %%xmm2, %%xmm1\n\t"
            "addps      %%xmm1, %%xmm0"
            : /* outputs */ "+r" ( a1 ), "+r" ( a2 )
            : /* inputs */
            : /* clobbered */ /*"xmm0", "xmm1", "xmm2", "xmm3", "xmm4"*/
            );
    }
    __asm__ __volatile__ (
        "movaps     %%xmm0, %0"
        : /* outputs */ "=m" ( ans )
        : /* inputs */
        : /* clobbered */ /*"xmm0",*/ "memory" );
    n -= i << 3;
    ans[0] += ans[1] + ans[2] + ans[3];
  }
  else
    ans[0] = 0.0;
  for( i = 0; i < n; ++i )
    ans[0] += a1[i] * a2[i];
  return( ans[0] );
}

#if defined(__i386__)

inline  unsigned long long rdtsc(void)
{
    unsigned long long int x;
     __asm__ __volatile__ ( ".byte 0x0f, 0x31"
                           : "=A" (x) );
     return x;
}

#elif defined(__x86_64__)

typedef unsigned long long int unsigned long long;

inline unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ( "rdtsc"
                          : "=a"(lo), "=d"(hi) );
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#endif

inline float addf(float x, float y)
{
    float *px = &x;
    __asm__ __volatile__ (
        "faddp\n\t"
        "fsts (%0)"
        : "=r"(px)
        : "f"(x), "f"(y)
        : "memory"
    );
    return x;
}

int sum(int *piarray, int sf)
{
     int isum = 0;
     int *p = &isum;
     __asm__ __volatile__(
         "movl %2, %%ecx\n\t"
         "dec %%ecx\n\t"
         "movl %1, %%esi\n\t"
         "finit\n\t"
         "fildl (%%esi)\n\t"
           "next:\n\t"
         "add $4, %%esi\n\t"
         "fiaddl (%%esi)\n\t"
         "loop next\n\t"
         "fistpl (%0)\n\t"
         "fwait"
         : "=r"(p)
         : "m"(piarray), "m"(sf)
         : "memory"
    );
    return isum;
}


inline void set_fpu (unsigned int mode)
{
    __asm__ __volatile__ (
        "fldcw %0"
        :
        : "m" (*&mode));
}


#endif //

}; // namespace huys

#endif // U_INLINE_ASSEMBLY_H
