#include "../include/Operand.hpp"
#include "../include/AVMExceptions.hpp"
#include <iostream>

Operand::Operand()
{
    return;
}

Operand::Operand(Factory const *factory, EOperandType type, std::string value)
{
    long double v;

    v = std::stold(value);
    if (type < Float)
    {
        long double o = 1.0;
        if ((o = std::modf(v, &o)) > 0)
        {
            v -= o;
        }
    }
    this->_type = type;
    this->_factory = factory;
    switch (type)
    {
        case Int8:
            if (v > INT8_MAX) { throw AVMExceptions::OverflowValue(); }
            if (v < INT8_MIN) { throw AVMExceptions::UnderflowValue(); }
        case Int16:
            if (v > INT16_MAX) { throw AVMExceptions::OverflowValue(); }
            if (v < INT16_MIN) { throw AVMExceptions::UnderflowValue(); }
        case Int32:
            if (v > INT32_MAX) { throw AVMExceptions::OverflowValue(); }
            if (v < INT32_MIN) { throw AVMExceptions::UnderflowValue(); }
        case Float:
            if (v > INT32_MAX) { throw AVMExceptions::OverflowValue(); }
            if (v < INT32_MIN) { throw AVMExceptions::UnderflowValue(); }
        case Double:
            if (v > INT32_MAX) { throw AVMExceptions::OverflowValue(); }
            if (v < INT32_MIN) { throw AVMExceptions::UnderflowValue(); }
        default:
            break;
    }
    this->_value = std::to_string(v);
    if (type < Float) { this->_value = this->_value.substr(0, this->_value.find('.')); }
    if (type == Float) { this->_value += 'f'; }
    return;
}

Operand::Operand(const Operand &src)
{
    *this = src;
    return;
}

Operand::~Operand()
{
    return;
}

int Operand::GetPrecision() const
{
    switch (this->_type)
    {
        case Int8:
            return(0);
            break;
        case Int16:
            return(1);
            break;
        case Int32:
            return(2);
            break;
        case Float:
            return(3);
            break;
        case Double:
            return(4);
            break;
        default:
            break;
    }
    return (-1);
}

EOperandType Operand::GetType() const
{
    return (this->_type);
}

IOperand const *Operand::Arithmetic(IOperand const *lhs, IOperand const *rhs, int operation) const
{
    EOperandType type;
    double a;
    double b;

    type = lhs->GetPrecision() >= rhs->GetPrecision() ? lhs->GetType() : rhs->GetType();
    a = std::stod(lhs->ToString());
    b = std::stod(rhs->ToString());
    if (operation == 0)
    {
        a += b;
    }
    else if (operation == 1)
    {
        a -= b;
    }
    else if (operation == 2)
    {
        a *= b;
    }
    else if (operation == 3)
    {
        a /= b;
    }
    else
    {
        a = std::modf(a, &b);
    }
    return (this->_factory->CreateOperand(type, std::to_string(a)));
}

IOperand const *Operand::operator=(const IOperand &rhs)
{
    this->_type = rhs.GetType();
    this->_value = rhs.ToString();
    return (this);
}

IOperand const *Operand::operator+(const IOperand &rhs) const
{
    return (this->Arithmetic(this, &rhs, 0));
}

IOperand const *Operand::operator-(const IOperand &rhs) const
{
    return (this->Arithmetic(this, &rhs, 1));
}

IOperand const *Operand::operator*(const IOperand &rhs) const
{
    return (this->Arithmetic(this, &rhs, 2));
}

IOperand const *Operand::operator/(const IOperand &rhs) const
{
    return (this->Operand::Arithmetic(this, &rhs, 3));
}

IOperand const *Operand::operator%(const IOperand &rhs) const
{
    return (this->Arithmetic(this, &rhs, 4));
}

std::string const &Operand::ToString() const
{
    return (this->_value);
}

std::ostream &operator<<(std::ostream &out, const Operand &rhs)
{
    out << rhs.ToString();
    return (out);
}