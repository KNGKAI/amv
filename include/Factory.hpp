#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "IOperand.hpp"

class Factory
{
    private:
        IOperand const  *CreateInt8(std::string const &value) const;
        IOperand const  *CreateInt16(std::string const &value) const;
        IOperand const  *CreateInt32(std::string const &value) const;
        IOperand const  *CreateFloat(std::string const &value) const;
        IOperand const  *CreateDouble(std::string const &value) const;
    public:
        Factory();
        Factory(const Factory &src);
        ~Factory();

        IOperand const      *CreateOperand(EOperandType type, std::string const &value) const;
        std::string const   &ToString() const;

        Factory const      *operator=(const Factory &rhs);
};

std::ostream &operator<<(std::ostream &out, const Factory &rhs);

#endif