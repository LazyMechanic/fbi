#pragma once

#include <stdexcept>
#include <string>

namespace fbi {
class Exception : public std::exception
{
public:
    explicit Exception(const std::string& callFunc, const std::string& msg);
    char const* what() const override;

protected:
    std::string m_callFunc;
    std::string m_msg;
};

class SignError : public Exception
{
public:
    explicit SignError(const std::string& callFunc, const std::string& msg);
};

class DivideByZeroError : public Exception {
public:
    explicit DivideByZeroError(const std::string& callFunc);

};

class MathError : public Exception
{
public:
    explicit MathError(const std::string& callFunc, const std::string& msg);
};
}