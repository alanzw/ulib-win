#ifndef U_EVAL_H
#define U_EVAL_H

#include <exception>
#include <iostream>

#include "umsg.h"

#include "adt/ustring.h"
#include "adt/ustack.h"

typedef huys::ADT::UStringAnsi TString;
typedef huys::ADT::UStack<int> UStackInt;

class EvalException
{
public:
    EvalException(const char *sError)
    : m_sError(sError)
    {}

    void what()
    {
        showMsg(m_sError);
    }
private:
    const char *m_sError;
};

class PostfixEval
{
public:
    PostfixEval();

    int evaluate();

    TString getPostfixExp() const;

    void setPostfixExp(const TString &exp);
private:
    TString m_sPostfixExp;
    UStackInt m_operandStack;

    void getOperands(int &left, int &right);
    int compute(int left, int right, char op) const;
    bool isOperator() const;
};

#endif // U_EVAL_H
