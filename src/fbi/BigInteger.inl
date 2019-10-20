// CONVERSION TO PRIMITIVE INTEGERS

/* Reuse BigUnsigned's conversion to an unsigned primitive integer.
 * The friend is a separate function rather than
 * BigInteger::convertToUnsignedPrimitive to avoid requiring BigUnsigned to
 * declare BigInteger. */
template <class X>
X convertBigUnsignedToPrimitiveAccess(const BigUnsigned& a)
{
    return a.convertToPrimitive<X>();
}

template <class X>
X BigInteger::convertToUnsignedPrimitive() const
{
    if (sign == negative)
        throw "BigInteger::to<Primitive>: "
              "Cannot convert a negative integer to an unsigned type";
    else
        return convertBigUnsignedToPrimitiveAccess<X>(mag);
}

/* Similar to BigUnsigned::convertToPrimitive, but split into two cases for
 * nonnegative and negative numbers. */
template <class X, class UX>
X BigInteger::convertToSignedPrimitive() const
{
    if (sign == zero)
        return 0;
    else if (mag.getLength() == 1) {
        // The single block might fit in an X.  Try the conversion.
        Blk b = mag.getBlock(0);
        if (sign == positive) {
            X x = X(b);
            if (x >= 0 && Blk(x) == b)
                return x;
        }
        else {
            X x = -X(b);
            /* UX(...) needed to avoid rejecting conversion of
             * -2^15 to a short. */
            if (x < 0 && Blk(UX(-x)) == b)
                return x;
        }
        // Otherwise fall through.
    }
    throw "BigInteger::to<Primitive>: "
          "Value is too big to fit in the requested type";
}

template <typename Integer>
bool BigInteger::operator==(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator==(BigInteger{ x });
}

template <typename Integer>
bool BigInteger::operator!=(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator!=(BigInteger{ x });
}

template <typename Integer>
bool BigInteger::operator<(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator<(BigInteger{ x });
}

template <typename Integer>
bool BigInteger::operator<=(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator<=(BigInteger{ x });
}

template <typename Integer>
bool BigInteger::operator>=(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator>=(BigInteger{ x });
}

template <typename Integer>
bool BigInteger::operator>(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator>(BigInteger{ x });
}

// ======================================================================================== //

template <typename Integer>
BigInteger BigInteger::operator+(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator+(BigInteger{ x });
}

template <typename Integer>
BigInteger BigInteger::operator-(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator-(BigInteger{ x });
}

template <typename Integer>
BigInteger BigInteger::operator*(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator*(BigInteger{ x });
}

template <typename Integer>
BigInteger BigInteger::operator/(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator/(BigInteger{ x });
}

template <typename Integer>
BigInteger BigInteger::operator%(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator%(BigInteger{ x });
}

// ======================================================================================== //

template <typename Integer>
BigInteger& BigInteger::operator+=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator+=(BigInteger{ x });
}

template <typename Integer>
BigInteger& BigInteger::operator-=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator-=(BigInteger{ x });
}

template <typename Integer>
BigInteger& BigInteger::operator*=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator*=(BigInteger{ x });
}

template <typename Integer>
BigInteger& BigInteger::operator/=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator/=(BigInteger{ x });
}

template <typename Integer>
BigInteger& BigInteger::operator%=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator%=(BigInteger{ x });
}