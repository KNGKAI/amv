#include "../include/Machine.hpp"
#include "../include/AVMExceptions.hpp"
#include <string>
#include <stdlib.h>

Machine::Machine()
{
    return;
}

Machine::Machine(const Machine &src)
{
    *this = src;
    return;
}

Machine::~Machine()
{
    return;
}

Machine const *Machine::operator=(const Machine &rhs)
{
    this->_stack = rhs.GetStackVector();
    return (this);
}

void Machine::Execute(std::string filename)
{
    EInstructionsType   instructionType;
    IOperand const      *operand;
    std::ifstream       file(filename);
    std::string         line;
    int                 lineCount;
    bool                exit;

    lineCount = 1;
    exit = false;
    while (std::getline(file, line))
    {
        line = line == ";;" ? line : line.substr(0, line.find(';'));
        while (isspace(line[0])) { line = line.substr(1, line.size()); }
        while (isspace(line[line.size() - 1])) { line = line.substr(0, line.size() - 1); }
        if (line.length() > 0)
        {
            instructionType = this->GetInstruction(line);
            if (instructionType == Exit) { exit = true; }
            if (instructionType < Pop)
            {
                operand = this->GetOperand(line);
                if (operand == nullptr) { instructionType = Error; }
                try
                {
                    this->Instruction(instructionType, operand);
                }
                catch(std::exception &e)
                {
                    std::cerr << "Error on line " << lineCount << ": " << e.what() << std::endl;
                }
            }
            else
            {
                try
                {
                    this->Instruction(instructionType);
                }
                catch(std::exception &e)
                {
                    std::cerr << "Error on line " << lineCount << ": " << e.what() << std::endl;
                }
            }
        }
        lineCount++;
    }
    if (!exit)
    {
        try
        {
            throw AVMExceptions::NoExit();
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void Machine::ExecuteInstruction(std::string instruction)
{
    EInstructionsType   instructionType;
    IOperand const      *operand;

    instruction = instruction.substr(0, instruction.find(';'));
    while (isspace(instruction[0])) { instruction = instruction.substr(1, instruction.size()); }
    while (isspace(instruction[instruction.size() - 1])) { instruction = instruction.substr(0, instruction.size() - 1); }
    if (instruction.length() > 0)
    {
        instructionType = this->GetInstruction(instruction);
        if (instructionType < Pop)
        {
            try
            {
                operand = this->GetOperand(instruction);
                if (operand == nullptr) { instructionType = Error; }
                this->Instruction(instructionType, operand);
            }
            catch(std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        else
        {
            try
            {
                this->Instruction(instructionType);
            }
            catch(std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
}

void Machine::Instruction(EInstructionsType type)
{
    if (type < Pop) { return; }
    switch (type)
    {
        case Pop:
            this->_pop();
            break;
        case Dump:
            this->_dump();
            break;
        case Print:
            this->_print();
            break;
        case Exit:
            this->_exit();
            break;
        case Add:
            this->_add();
            break;
        case Sub:
            this->_sub();
            break;
        case Mul:
            this->_mul();
            break;
        case Div:
            this->_div();
            break;
        case Mod:
            this->_mod();
            break;
        case Error:
            throw AVMExceptions::UnknownInstruction();
        default:
            break;
    }
}

void Machine::Instruction(EInstructionsType type, IOperand const *o)
{
    if (type > Assert) { return; }
    switch (type)
    {
        case Push:
            this->_push(o);
            break;
        case Assert:
            this->_assert(o);
            break;
        case Error:
            throw AVMExceptions::UnknownInstruction();
            break;
        default:
            break;
    }
}

std::string Machine::GetStack() const
{
    std::string s;

    s = "";
    for (std::vector<const IOperand * const>::iterator i = this->_stack.begin(); i < this->_stack.end(); i++)
    {
        s += (*i)->ToString() + "\n";
    }
    return (s);
}

std::vector<const IOperand *> Machine::GetStackVector() const
{
    return (this->_stack);
}

EInstructionsType Machine::GetInstruction(std::string instruction)
{
    if (instruction.substr(0, 4) == "push") { return (Push); }
    if (instruction.substr(0, 6) == "assert") { return (Assert); }
    if (instruction == "pop") { return (Pop); }
    if (instruction == "dump") { return (Dump); }
    if (instruction == "add") { return (Add); }
    if (instruction == "sub") { return (Sub); }
    if (instruction == "mul") { return (Mul); }
    if (instruction == "div") { return (Div); }
    if (instruction == "mod") { return (Dump); }
    if (instruction == "print") { return (Print); }
    if (instruction == "exit") { return (Exit); }
    if (instruction == ";;") { return (Exit); }
    return (Error);
}

IOperand const *Machine::GetOperand(std::string instruction)
{
    EOperandType type;
    std::string value;
    std::string sub;

    (void)type;
    if (this->CheckInstruction(instruction))
    {
        throw AVMExceptions::ErrorInstruction();
        return (nullptr);
    }
    if (instruction.substr(0, 4) == "push")
    {
        sub = instruction.substr(5);
        return (this->_factory.CreateOperand(this->GetOperandType(sub), this->GetOperandValue(sub)));
    }
    if (instruction.substr(0, 6) == "assert")
    {
        sub = instruction.substr(7);
        return (this->_factory.CreateOperand(this->GetOperandType(sub), this->GetOperandValue(sub)));
    }
    return (nullptr);
}

EOperandType Machine::GetOperandType(std::string instruction)
{
    if (instruction.substr(0, 4) == "int8") return (Int8);
    if (instruction.substr(0, 5) == "int16") return (Int16);
    if (instruction.substr(0, 5) == "int32") return (Int32);
    if (instruction.substr(0, 5) == "float") return (Float);
    if (instruction.substr(0, 6) == "double") return (Double);
    return (Int8);
}

std::string Machine::GetOperandValue(std::string instruction)
{
    std::string value;
    int i;

    i = 0;
    if (instruction.substr(0, 4) == "int8") i = 5;
    if (instruction.substr(0, 5) == "int16") i = 6;
    if (instruction.substr(0, 5) == "int32") i = 6;
    if (instruction.substr(0, 5) == "float") i = 6;
    if (instruction.substr(0, 6) == "double") i = 7;
    return (std::to_string(std::stod(instruction.substr(i))));
}

bool Machine::CheckInstruction(std::string instruction)
{
    std::string sub;
    
    sub = instruction.substr(instruction.substr(0, 4) == "push" ? 5 : 7);
    if (
        sub.substr(0, 4) == "int8" ||
        sub.substr(0, 5) == "int16" ||
        sub.substr(0, 5) == "int32" ||
        sub.substr(0, 5) == "float" ||
        sub.substr(0, 6) == "double"
    )
    {
        sub = sub.substr(sub.find('('));
        if (
            sub.size() >= 3 &&
            sub.substr(sub.find(')')).size() == 1
        )
        {
            for (int i = 1; i < (int)(sub.size() - 1); i++)
            {
                if (!(i == 1 && sub[i] == '-'))
                {
                    if (!isdigit(sub[i]) && sub[i] != '.') { return (true); }
                }
            }
            return (false);                
        }
    }
    return (true);
}

void Machine::_push(const IOperand *o)
{
    this->_stack.insert(this->_stack.begin(), o);
}

void Machine::_assert(const IOperand *o)
{
    if (this->_stack.empty()) { throw AVMExceptions::TooFewValues(); }
    const IOperand *a = *this->_stack.begin();
    if (a->ToString() != o->ToString() || a->GetType() != o->GetType()) { throw AVMExceptions::AssertFalse(); }
}

void Machine::_pop()
{
    if (this->_stack.empty()) { throw AVMExceptions::TooFewValues();  }
    this->_stack.erase(this->_stack.begin());
}

void Machine::_dump()
{
    if (this->_stack.empty()) { return; }
    std::cout << this->GetStack();
}

void Machine::_print()
{
    if (this->_stack.empty()) { throw AVMExceptions::TooFewValues(); }
    const IOperand *a = *this->_stack.begin();
    if (a->GetType() != Int8) { throw AVMExceptions::AssertFalse(); }
    char c = std::stoi(a->ToString());
    std::cout << c << std::endl;
}

void Machine::_exit()
{
    if (!this->_stack.empty()) { this->_stack.clear(); }
    std::exit(0);
}

void Machine::_add()
{
    if (this->_stack.size() < 2) { throw AVMExceptions::TooFewValues(); }
    const IOperand *a = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *b = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *c = *a + *b;
    delete a;
    delete b;
    this->_push(c);
}
void Machine::_sub()
{
    if (this->_stack.size() < 2) { throw AVMExceptions::TooFewValues(); }
    const IOperand *a = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *b = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *c = *a - *b;
    delete a;
    delete b;
    this->_push(c);
}

void Machine::_mul()
{
    if (this->_stack.size() < 2) { throw AVMExceptions::TooFewValues(); }
    const IOperand *a = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *b = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *c = *a * *b;
    delete a;
    delete b;
    this->_push(c);
}

void Machine::_div()
{
    std::cout << "Div" << std::endl;
    if (this->_stack.size() < 2) { throw AVMExceptions::TooFewValues(); }
    std::vector<const IOperand *>::iterator i = this->_stack.begin();
    i++;
    if (std::stod((*i)->ToString()) == 0) { throw AVMExceptions::DivideByZero(); }
    const IOperand *a = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *b = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *c = *a / *b;
    delete a;
    delete b;
    this->_push(c);
}

void Machine::_mod()
{
    if (this->_stack.size() < 2) { throw AVMExceptions::TooFewValues(); }
    std::vector<const IOperand *>::iterator i = this->_stack.begin();
    i++;
    if (std::stod((*i)->ToString()) == 0) { throw AVMExceptions::ModuloByZero(); }
    const IOperand *a = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *b = *this->_stack.begin();
    this->_stack.erase(this->_stack.begin());
    const IOperand *c = *a % *b;
    delete a;
    delete b;
    this->_push(c);
}

std::ostream &operator<<(std::ostream &out, const Machine &rhs)
{
    out << rhs.GetStack();
    return (out);
}