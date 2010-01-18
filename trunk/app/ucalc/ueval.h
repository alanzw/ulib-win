#ifndef U_EVAL_H
#define U_EVAL_H

#include <exception>
#include <iostream>
//#include <map>

#include "umsg.h"

#include "adt/ustring.h"
#include "adt/ustack.h"
#include "adt/utable.h"

typedef huys::ADT::UStringAnsi TString;
typedef huys::ADT::UStack<int> UStackInt;
typedef huys::ADT::UStack<TString> UStackStr;
typedef huys::ADT::UTable<TString, int> UTableSI;
//typedef std::map<TString, int> UTableSI;

template <>
struct huys::ADT::hash<TString>
{
    static int hash_value(const TString &key)
    {
        unsigned int hash_value = 0;
        /*
        Compute the hash value from the first four bytes of the object's
        resident memory.  Then get the hash index for the array by taking
        the modulo of the size of that array (<number_buckets>).
        */
        memcpy( &hash_value, key.c_str(), sizeof(unsigned char) * key.length() % sizeof(unsigned int) );
        return hash_value;
    }
};

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
