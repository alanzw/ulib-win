#ifndef U_EVAL_H
#define U_EVAL_H

#include <exception>
#include <iostream>

#include "umsg.h"

#include "adt/ustring.h"
#include "adt/ustack.h"
#include "adt/utable.h"

typedef huys::ADT::UStringAnsi TString;
typedef huys::ADT::UStack<int> UStackInt;
typedef huys::ADT::UStack<TString> UStackStr;
typedef huys::ADT::UTable<TString, int> UTableSI;

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
private:
    void getOperands(int &left, int &right);
    int compute(int left, int right, char op) const;
    bool isOperator(char ch) const;
};

class Infix2Postfix
{
public:
    Infix2Postfix()
    {}

    Infix2Postfix(const TString & infixExp)
    :infix(infixExp)
    {}

    void setInfixExp(const TString& infixExp)
    {
        infix=infixExp;
    }

    TString postfixExp();

    ~Infix2Postfix(){};

private:
    TString infix;
    TString postfix;
    UStackStr stk;//stack used to store operator
    UTableSI oper_prio;//Used to store the priority of operator
    void set_priority();//set operator's priority
};

#endif // U_EVAL_H
