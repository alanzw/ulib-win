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
    __asm {
        nop
    }

    return a1[0];
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

void set_fpu (unsigned int mode)
{
    __asm {
        fldcw [mode]
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
