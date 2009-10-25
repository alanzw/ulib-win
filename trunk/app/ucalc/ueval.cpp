#include <windows.h>
#include <tchar.h>

#include "ueval.h"

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
