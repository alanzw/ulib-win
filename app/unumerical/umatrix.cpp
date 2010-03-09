#include <iostream>
#include <cmath>

using namespace std;

#include "umatrix.h"

void Matrix::setMatrix(double* rmatr)
{
    for (int i=0;i<index*index;i++)
    {
        *(MatrixA+i)=rmatr[i];                //¾ØÕó³ÉÔ±¸³³õÖµ
    }
}

Matrix::Matrix(int dims)
{
    index=dims;
    MatrixA=new double[index*index];
}

Matrix::~Matrix()
{
    delete[] MatrixA;
}


void Matrix::printM()
{
    cout<<"The Matrix is:"<<endl;
    for (int i=0;i<index;i++)
    {
        for (int j=0;j<index;j++) {
            cout<<*(MatrixA+i*index+j)<<" ";
        }
        cout<<endl;
    }
}

Linequ::Linequ(int dims):Matrix(dims)
{
    sums=new double[dims];
    solu=new double[dims];
}

Linequ::~Linequ()
{
    delete[] sums;
    delete[] solu;
}

void Linequ::setLinequ(double* a, double* b)
{
    setMatrix(a);
    for (int i=0;i<index;i++)
    {
        sums[i]=b[i];
    }
}

void Linequ::printL()
{
    cout<<"The Line eqution is: AX=B"<<endl;
    for (int i=0;i<index;i++)
    {
        for (int j=0;j<index;j++) {
            cout<<*(MatrixA+i*index+j)<<" ";
        }
        cout<<"\t"<<sums[i]<<endl;
    }
}

void Linequ::showX()
{
    cout<<"The Result is:"<<endl;
    for (int i=0;i<index;i++)
    {
        cout<<"X["<<i<<"]="<<solu[i]<<endl;
    }
}


int Linequ::Solve()
{
    int *js,l,k,i,j,is,p,q;
    double d,t;
    js=new int[index];
    l=1;

    for (k=0;k<=index-2;k++)
    {
        d=0.0;
        for (i=k;i<index-1;i++)
        {
            for (j=k;j<=index-1;j++)
            {
                t=fabs(MatrixA[i*index+j]);
                if (t>d)
                {
                    d=t;
                    js[k]=j;
                    is=i;
                }
            }
        }

        if (d==0.0)
        {
            l=0;
        }
        else
        {
            if (js[k]!=k)
            {
                for (i=0;i<=index-1;i++)
                {
                    p=i*index+k;
                    q=i*index+js[k];
                    //t=MatrixA[p];
                    //MatrixA[p]=MatrixA[q];
                    //MatrixA[q]=t;
                    swap(MatrixA[p],MatrixA[q]);
                }

                if (is!=k)
                {
                    for (j=k;j<=index-1;j++)
                    {
                        p=k*index+j;
                        q=is*index+j;
                        //t=MatrixA[p];
                        //MatrixA[p]=MatrixA[q];
                        //MatrixA[q]=t;
                        swap(MatrixA[p],MatrixA[q]);
                    }
                    //t=sums[k];
                    //sums[k]=sums[is];
                    //sums[is]=t;
                    swap(sums[k],sums[is]);
                }
            }
        }

        if (l==0)
        {
            delete[] js;
            cout<<"fail"<<endl;
            return(0);
        }

        d=MatrixA[k*index+k];
        for (j=k+1;j<=index-1;j++)
        {
            p=k*index+j;
            MatrixA[p]=MatrixA[p]/d;
        }

        sums[k]=sums[k]/d;
        for (i=k+1;i<=index-1;i++)
        {
            for (j=k+1;j<=index-1;j++)
            {
                p=i*index+j;
                MatrixA[p]=MatrixA[p]-MatrixA[i*index+k]*MatrixA[k*index+j];
            }
            sums[i]=sums[i]-MatrixA[i*index+k]*sums[k];
        }

    }

    d=MatrixA[(index-1)*index+index-1];
    if (fabs(d)+1.0==1.0)
    {
        delete[] js;
        cout<<"fail"<<endl;
        return(0);
    }

    solu[index-1]=sums[index-1]/d;
    for (i=index-2;i>=0;i--)
    {
        t=0.0;
        for (j=i+1;j<=index-1;j++)
        {
            t=t+MatrixA[i*index+j]*solu[j];
        }

        solu[i]=sums[i]-t;
    }

    js[index-1]=index-1;
    for (k=index-1;k>=0;k--)
    {
        if (js[k]!=k)
        {
            //t=solu[k];
            //solu[k]=solu[js[k]];
            //solu[js[k]]=t;
            swap(solu[k],solu[js[k]]);
        }
    }
    delete[] js;
    return(1);
}

void linequ_driver()
{
    double a[]=
    {
        0.2368,0.2471,0.2568,1.2671,
        0.1968,0.2071,1.2168,0.2271,
        0.1581,1.1675,0.1768,0.1871,
        1.1161,0.1254,0.1397,0.1490
    };

    double b[4]=
    { 1.8471,1.7471,1.6471,1.5471 };

    Linequ equ1(4);
    equ1.setLinequ(a,b);
    equ1.printL();
    if (equ1.Solve())
    {
        equ1.showX();
    }
    else
    {
        cout<<"fail"<<endl;
    }
}
