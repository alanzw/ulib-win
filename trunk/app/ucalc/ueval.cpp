#include <windows.h>
#include <tchar.h>

#include "ueval.h"

PostfixEval::PostfixEval()
{
}

int PostfixEval::compute(int left, int right, char op) const
{
    int value;

    switch (op)
    {
    case '+':
        value = left + right;
        break;
    case '-':
        value = left - right;
        break;
    case '*':
        value = left * right;
        break;
    case '/':
        if (0 == right) throw EvalException("PostfixEval: divide by 0");
        value = left / right;
        break;
    case '%':
        if (0 == right) throw EvalException("PostfixEval: divide by 0");
        value = left%right;
        break;
    case '^':
        if (0 == left && 0 == right) throw EvalException("PostfixEval: 0^0 undefined");
        value = 1;
        while (right > 0)
        {
            value *=left;
            --right;
        }
        break;
    default:
        throw EvalException("PostfixEval: unknown operand");
    }
    return value;
}

void PostfixEval::getOperands(int &left, int &right)
{
    //
    if (m_operandStack.empty())
        throw EvalException("PostfixEval: Too many operators");

    right = m_operandStack.top();
    m_operandStack.pop();

    //
    if (m_operandStack.empty())
        throw EvalException("PostfixEval: Too many operators");

    left = m_operandStack.top();
    m_operandStack.pop();
}

TString PostfixEval::getPostfixExp() const
{
    return m_sPostfixExp;
}

void PostfixEval::setPostfixExp(const TString &exp)
{
    m_sPostfixExp = exp;
}

int PostfixEval::evaluate()
{
    int i, left, right, expValue;
    char ch;
    for(i=0; i<m_sPostfixExp.length(); i++)
    {
        ch = m_sPostfixExp.at(i);
        if(isdigit(ch))
        {
            m_operandStack.push(ch-'0');
        }
        else if(isOperator(ch))
        {
            getOperands(left,right);
            m_operandStack.push(compute(left,right,ch));
        }
    }
    expValue=m_operandStack.top();
    m_operandStack.pop();
    return expValue;
}

bool PostfixEval::isOperator(char ch) const
{
    return ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='%'||ch=='^';
}

void Infix2Postfix::set_priority()
{
    oper_prio.add("#", 1);
    oper_prio.add("(", 2);
    oper_prio.add("+", 3);
    oper_prio.add("-", 3);
    oper_prio.add("*", 4);
    oper_prio.add("/", 4);
    oper_prio.add("%", 4);
    oper_prio.add("^", 5);
    oper_prio.add(")", 6);
}

TString Infix2Postfix::postfixExp()
{
    postfix="";
    set_priority();
    stk.push("#");
    int i=0;
    TString input, topstk;
    for(;i<infix.size();)
    {
        topstk=stk.top();
        input=infix.substr(i, i);
        if(!oper_prio.inTable(input))
        {
            postfix+=input;
        }
        else
        {
            if(oper_prio[input]>oper_prio[topstk])
            {
                if(input.compare(")"))
                {
                    while(!topstk.compare("("))
                    {
                        postfix+=topstk;
                        stk.pop();
                        topstk=stk.top();
                    }
                    stk.pop();
                }
                else
                    stk.push(input);
            }
            else
            {
                 if(!input.compare("("))
                {
                    postfix+=topstk;
                    stk.pop();
                    continue;
                }
                stk.push(input);
            }
        }
        ++i;
    }
    topstk=stk.top();
    while(!topstk.compare("#"))
    {
        postfix+=topstk;
        stk.pop();
        topstk=stk.top();
    }
    return postfix;
}

