#include "rpn_calculator.hpp"
#include <cctype>
#include <cmath>
#include <stdexcept>

double RPNCalculator::applyOperation(char op, double left_arg, double right_arg) {
    switch(op) {
        case '+': return left_arg + right_arg;
        case '-': return left_arg - right_arg;
        case '*': return left_arg * right_arg;
        case '/':
            if (right_arg == 0)
                throw std::invalid_argument("Опит за деление на 0");
            return left_arg / right_arg;
        case '^': return pow(left_arg, right_arg);
        default: throw std::invalid_argument("Невалидна операция");
    }
}

double RPNCalculator::calculateRPN(std::string const& rpn) {
    for(char c : rpn) {
        if (std::isdigit(c)) {
            // c е цифра
            results.push(digitValue(c));
        } else {
            // c е операция
            double right_arg = results.pop();
            double left_arg  = results.pop();
            results.push(applyOperation(c, left_arg, right_arg));
        }
    }
    return results.pop();
}

std::string RPNCalculator::convertToRPN(std::string const& expr) {
    if(!this->validateBraces(expr)){
        throw std::invalid_argument("Невалидна формулировка на израза.");
    }
    std::string rpn;
    for(char c : expr)
        if (std::isdigit(c))
            // c е цифра, записваме я в резултата
            rpn += c;
        else if (c == ')') {
            // c е затваряща скоба, изваждаме всички операции до отваряща скоба
            while (ops.peek() != '(')
                rpn += ops.pop();
            // изваждане на отварящата скоба от стека
            ops.pop();
        } else if (c == '(')
            // c е отваряща скоба, добавяме я в стека
            ops.push(c);
        else {
            // c е операция, записваме я в стека с операции,
            // но преди това тя избутва всички останали операции с по-висок или равен приоритет
            while (!ops.empty() && ops.peek() != '(' && priority(ops.peek()) >= priority(c))
                rpn += ops.pop();
            ops.push(c);
        }
    while (!ops.empty())
        rpn += ops.pop();
    return rpn;
}

int RPNCalculator::priority(char op) {
    switch(op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;

        default: throw std::invalid_argument("Невалидна операция");
    }  
}

bool RPNCalculator::isSymbol(char op){
    switch (op)
    {
    case '*':
    case '-':
    case '+':
    case '/':
    case '^':
        return true;
    default:
        return false;
    }
}

bool RPNCalculator::validateBraces(std::string const& rpn){
    for(char i: rpn){
        if('(' == i){
            this->braces.push('(');
        }else if(')' == i){
            if(this->braces.empty()){
                return false;
            }else{
                this->braces.pop();
            }
        }
    }
    for (size_t i = 0; i < rpn.length()-1; i++)
    {
        if(isdigit(rpn[i]) && isdigit(rpn[i+1])){
            return false;
        }else if(isSymbol(rpn[i]) && isSymbol(rpn[i+1])){
            return false;
        }
    }
    
    return this->braces.empty();
}