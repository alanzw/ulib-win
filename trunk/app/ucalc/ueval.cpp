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

    right = m_operandStack.pop();

    //
    if (m_operandStack.empty())
        throw EvalException("PostfixEval: Too many operators");

    left = m_operandStack.pop();
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
    //unsigned int i;
    int left, right, expValue;
    //char ch;
    TString tmp;
    TString::size_type pos = 0;
    TString::size_type newPos = 0;
    //for(i=0; i<m_sPostfixExp.length(); i++)
    while (pos < m_sPostfixExp.length())
    {
        //ch = m_sPostfixExp.at(i);
        newPos = m_sPostfixExp.find(' ', pos);
        tmp = m_sPostfixExp.substr(pos, newPos-1);
        pos =  newPos < m_sPostfixExp.length() ? newPos+1 : m_sPostfixExp.length()-1;
        if(isdigit(tmp.at(0)))
        {
            //m_operandStack.push(ch-'0');
            m_operandStack.push(atoi(tmp));
        }
        else if(isOperator(tmp.at(0)))
        {
            getOperands(left, right);
            m_operandStack.push(compute(left,right,tmp.at(0)));
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
    oper_prio["#"] = 1;
    oper_prio["("] = 2;
    oper_prio["+"] = 3;
    oper_prio["-"] = 3;
    oper_prio["*"] = 4;
    oper_prio["/"] = 4;
    oper_prio["%"] = 4;
    oper_prio["^"] = 5;
    oper_prio[")"] = 6;
    opers = "#(+-*/%^)";
}

TString Infix2Postfix::postfixExp()
{
    postfix="";

    set_priority();

    stk.push("#");

    TString input;
    TString topstk;

    TString::size_type pos = 0;
    TString::size_type posEnd = 0;

    //for(unsigned int i=0; i<infix.size(); ++i)
    while (posEnd < infix.size())
    {
        topstk = stk.top();

        pos = posEnd; // Update pos value

        posEnd = infix.findCharSet(opers, opers.size(), pos);

        if (posEnd == -1)
        {
            posEnd = infix.size();
        }

        if (posEnd == pos)
        {
            ++posEnd;
        }

        input = infix.substr(pos, posEnd-1);
        

        if (!oper_prio[input])
        {
            postfix += input;
            postfix += " ";
            //stk.push(input);
        }
        else
        {
            if( oper_prio[input]>oper_prio[topstk] )
            {
                if(input.compare(")"))
                {
                    while(!topstk.compare("("))
                    {
                        postfix += topstk;
                        postfix += " ";
                        stk.pop();
                        topstk=stk.top();
                    }
                    stk.pop();
                }
                else
                {
                    stk.push(input);
                }
            }
            else
            {
                 if(!input.compare("("))
                {
                    postfix +=topstk;
                    postfix += " ";
                    stk.pop();
                    //continue;
                }
                stk.push(input);
            }
        }
    }
    topstk=stk.top();
    while(!topstk.compare("#"))
    {
        postfix+=topstk;
        postfix+=" ";
        stk.pop();
        topstk=stk.top();
    }
    return postfix;
}

