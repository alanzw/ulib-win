#include <cstdio>
#include <cstdlib>
#include <cstring>

class BitSet
{
public:
    BitSet(const size_t i)
    {
        bp = (unsigned *)calloc((i>>5)+3,sizeof(unsigned));
        *bp = i;//size
        *(bp+1) = i;//capacity
    };
    
    ~BitSet()
    {
        free(bp);
    };
    
    bool resize(const size_t nsize)
    {
        if (nsize <= this->capacity())
        {
            if (nsize < this->size())
            {
                bp[2+(nsize>>5)] &= ~(0xffffffff <<(nsize &0x1f));
                unsigned *s = bp+3+(nsize>>5);
                unsigned *d = bp+3+(size()>>5);
                while (s!=d) *s++ = 0;
            };
            *bp = nsize;
        }
        else
        {
            unsigned *nbp =(unsigned *)calloc((nsize>>5)+3,sizeof(unsigned));
            if (nbp==NULL) return false;
            memcpy(nbp,bp,((size()>>5)+3)*sizeof(unsigned));
            *nbp = nsize;
            *(nbp+1) = nsize;
            free(bp);
            bp = nbp;
        }
        return true;
    };
    void set(const int i)
    {
        bp[2+(i>>5)] |= (1 << (i & 0x1f));
    };
    void clr(const int i)
    {
        bp[2+(i>>5)] &= ~(1 << (i & 0x1f));
    };
    bool test(int i) const
    {
        return bp[2+(i>>5)] & (1 << (i&0x1f));
    } ;
    size_t size() const
    {
        return *bp;
    }
    size_t capacity() const
    {
        return *(bp+1);
    }


private :
    unsigned *bp;
    BitSet(const BitSet&);
    BitSet & operator=(const BitSet&);
};

int main()
{
    BitSet bb(9);
    bb.set(3);
    bb.set(8);
    for (size_t i = 0; i < bb.size(); ++i)
        printf("%d -> %d\n", i, bb.test(i));
    printf("\n");

    bb.resize(5);
    for (size_t i = 0; i < bb.size(); ++i)
        printf("%d -> %d\n", i, bb.test(i));
    printf("\n");


    bb.resize(11);
    for (size_t i = 0; i < bb.size(); ++i)
        printf("%d -> %d\n", i, bb.test(i));
    return 0;
}