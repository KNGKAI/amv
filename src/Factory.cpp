#include "../include/Factory.hpp"
#include "../include/Operand.hpp"

Factory::Factory()
{
    return;
}

Factory::Factory(const Factory &src)
{
    *this = src;
    return;
}

Factory const *Factory::operator=(const Factory &rhs)
{
    (void)rhs;
    return (this);
}

Factory::~Factory()
{
    return;
}

IOperand const *Factory::CreateOperand(EOperandType type, std::string const &value) const
{
    switch (type)
    {
        case Int8:
            return (this->CreateInt8(value));
        case Int16:
            return (this->CreateInt16(value));
        case Int32:
            return (this->CreateInt32(value));
        case Float:
            return (this->CreateFloat(value));
        case Double:
            return (this->CreateDouble(value));
        default:
            break;
    }
    return (nullptr);
}

IOperand const *Factory::CreateInt8(std::string const &value) const
{
    return (new Operand(this, Int8, value));
}

IOperand const *Factory::CreateInt16(std::string const &value) const
{
    return (new Operand(this, Int16, value));
}

IOperand const *Factory::CreateInt32(std::string const &value) const
{
    return (new Operand(this, Int32, value));
}

IOperand const *Factory::CreateFloat(std::string const &value) const
{
    return (new Operand(this, Float, value));
}

IOperand const *Factory::CreateDouble(std::string const &value) const
{
    return (new Operand(this, Double, value));
}

std::string const &Factory::ToString() const
{
    return (*(new std::string("Factory")));
}

std::ostream &operator<<(std::ostream &out, const Factory &rhs)
{
    out << rhs.ToString();
    return (out);
}