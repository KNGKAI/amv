#ifndef IOPERAND_HPP
#define IOPERAND_HPP

#include <string>
#include "EOperandType.hpp"

class IOperand
{
    public:
        virtual int             GetPrecision() const = 0;
        virtual EOperandType    GetType() const = 0;

        virtual IOperand const  *operator+(const IOperand &rhs) const = 0;
        virtual IOperand const  *operator-(const IOperand &rhs) const = 0;
        virtual IOperand const  *operator*(const IOperand &rhs) const = 0;
        virtual IOperand const  *operator/(const IOperand &rhs) const = 0;
        virtual IOperand const  *operator%(const IOperand &rhs) const = 0;    

        virtual std::string const   &ToString() const = 0;

        virtual ~IOperand() {}
};

#endif