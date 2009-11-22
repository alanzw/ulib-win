#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace B32
{

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

void test()
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

}

}; // namespace B32


namespace B8
{

class BitSet
{
public:
    BitSet(unsigned short i = 0)
    {
        bp = (char *)calloc((i >> 3) + 3, sizeof(char));
        *reinterpret_cast<unsigned short*>(bp) = i;
    }
    ~BitSet()
    {
        free(bp);
    }
    bool resize(unsigned short nsize)
    {
        char *nbp = (char *)calloc((nsize >> 3) + 3, sizeof(char));
        if (nbp == NULL) return false;
        size_t copysize = *bp < nsize ? *bp : nsize;
        memcpy(nbp, bp, ((copysize >> 3) + 3) * sizeof(char));
        *reinterpret_cast<unsigned short*>(nbp) = nsize;
        free(bp);
        bp = nbp;
        return true;
    }
    void set(unsigned short i)
    {
        bp[2 + (i >> 3)] |= (1 << (i & 0x07));
    }
    void clr(unsigned short i)
    {
        bp[2 + (i >> 3)] &= ~(1 << (i & 0x07));
    }
    bool test(unsigned short i) const
    {
        return (bp[2 + (i >> 3)] & (1 << (i & 0x07))) > 0;
    }
    unsigned short size()
    {
        return *bp;
    }
private:
    char *bp;
    BitSet(const BitSet&);
};

void test()
{
    BitSet b(10);
    b.set(3);
    b.set(8);
    for (size_t i=0; i < b.size();++i)
        printf("%d -> %d\n", i, b.test(i));
    printf("\n");

    b.resize(5);
    for (size_t i=0; i < b.size();++i)
        printf("%d -> %d\n", i, b.test(i));
    printf("\n");

    b.resize(9);
    for (size_t i=0; i < b.size();++i)
        printf("%d -> %d\n", i, b.test(i));
    printf("\n");
    for (size_t i=0; i < b.size();++i)
        b.set(i);
    for (size_t i=0; i < b.size();++i)
        printf("%d -> %d\n", i, b.test(i));
    printf("\n");
    for (size_t i=0; i < b.size();++i)
        b.clr(i);
    for (size_t i=0; i < b.size();++i)
        printf("%d -> %d\n", i, b.test(i));
    printf("\n");
}

}; // namespace B8

int main()
{
    B8::test();
    return 0;
}
