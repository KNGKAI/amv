#ifndef AVMEXCEPTIONS_HPP
#define AVMEXCEPTIONS_HPP

#include <string>

struct AVMExceptions : public std::exception
{
    public:
        struct UnknownInstruction : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("Unknown instruction");
            };
        };
        struct ErrorInstruction : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("Instruction Error");
            };
        };
        struct OverflowValue : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("Overflow on a value");
            };
        };
        struct UnderflowValue : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("Underflow on a value");
            };
        };
        struct DivideByZero : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("Attempted to divide by zero");
            };
        };
        struct ModuloByZero : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("Attempted to modulo by zero");
            };
        };
        struct NoExit : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("No exit instruction");
            };
        };
        struct AssertFalse : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("Assert intruction is not true");
            };
        };
        struct TooFewValues : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("Too few values in stack");
            };
        };
        struct InvalidValue : public std::exception
        {
            virtual const char *what() const throw()
            {
                return ("Invalid value to type");
            };
        };
};


#endif