#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Operand.hpp"
#include "Factory.hpp"
#include "EInstructionsType.hpp"
#include "AVMExceptions.hpp"

class Machine
{
    private:
        Factory                         _factory;
        std::vector<const IOperand *>   _stack;

        void    _push(const IOperand *o);
        void    _assert(const IOperand *o);
        void    _pop();
        void    _dump();
        void    _print();
        void    _exit();
        void    _add();
        void    _sub();
        void    _mul();
        void    _div();
        void    _mod();

        EInstructionsType   GetInstruction(std::string instruction);
        IOperand const      *GetOperand(std::string instruction);
        EOperandType        GetOperandType(std::string instruction);
        std::string         GetOperandValue(std::string instruction);
        bool                CheckInstruction(std::string instruction);
        void                Instruction(EInstructionsType type);
        void                Instruction(EInstructionsType type, IOperand const *o);

    public:
        Machine();
        Machine(const Machine &src);
        ~Machine();
        
        void        Execute(std::string file);
        void        ExecuteInstruction(std::string instruction);
        std::string GetStack() const;
        std::vector<const IOperand *> GetStackVector() const;

        Machine const      *operator=(const Machine &rhs);
};

std::ostream &operator<<(std::ostream &out, const Machine &rhs);

#endif