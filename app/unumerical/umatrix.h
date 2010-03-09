#ifndef U_MATRIX_H
#define U_MATRIX_H

class Matrix
{
public:
    Matrix(int dims=2);
    ~Matrix();
    void setMatrix(double* rmatr);
    void printM();
protected:
    int index;
    double* MatrixA;
private:

};

class Linequ: public Matrix
{
public:
    Linequ(int dims=2);
    ~Linequ();
    void setLinequ(double* a, double* b);
    void printL();
    int Solve();
    void showX();
    double *getSolution() const
    {
        return solu;
    }
protected:
private:
    double *sums;
    double *solu;
};

#endif // U_MATRIX_H
