#ifndef OPERAND
#define OPERAND

#include "IOperand.hpp"
#include "Factory.hpp"
#include <cmath>

class Operand : public IOperand
{
    private:
        IOperand const  *Arithmetic(const IOperand *lhs, const IOperand *rhs, int operation) const;

        std::string     _value;
        EOperandType    _type;
        Factory const   *_factory;
    public:
        Operand();
        Operand(const Factory *factory, EOperandType type, std::string value);
        Operand(const Operand &src);
        ~Operand();

        int                 GetPrecision() const;
        EOperandType        GetType() const;

        IOperand const      *operator=(const IOperand &rhs);
        IOperand const      *operator+(const IOperand &rhs) const;
        IOperand const      *operator-(const IOperand &rhs) const;
        IOperand const      *operator*(const IOperand &rhs) const;
        IOperand const      *operator/(const IOperand &rhs) const;
        IOperand const      *operator%(const IOperand &rhs) const;

        std::string const   &ToString() const;
};

std::ostream &operator<<(std::ostream &out, const Operand &rhs);

#endif