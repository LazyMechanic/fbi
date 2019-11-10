#include "Exception.hh"

namespace fbi {
Exception::Exception(const std::string& callFunc, const std::string& msg) : m_callFunc(callFunc), m_msg(msg) {}

char const* Exception::what() const
{
    std::string out = m_callFunc + ": " + m_msg;
    return out.c_str();
}

SignError::SignError(const std::string& callFunc, const std::string& msg) : Exception(callFunc, msg) {}

DivideByZeroError::DivideByZeroError(const std::string& callFunc) : Exception(callFunc, "Division by zero") {}

MathError::MathError(const std::string& callFunc, const std::string& msg) : Exception(callFunc, msg) {}
} // namespace fbi