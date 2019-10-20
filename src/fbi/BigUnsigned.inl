/* Templates for conversions of BigUnsigned to and from primitive integers.
 * BigInteger.cc needs to instantiate convertToPrimitive, and the uses in
 * BigUnsigned.cc didn't do the trick; I think g++ inlined convertToPrimitive
 * instead of generating linkable instantiations.  So for consistency, I put
 * all the templates here. */

// CONSTRUCTION FROM PRIMITIVE INTEGERS

/* Initialize this BigUnsigned from the given primitive integer.  The same
 * pattern works for all primitive integer types, so I put it into a template to
 * reduce code duplication.  (Don't worry: this is protected and we instantiate
 * it only with primitive integer types.)  Type X could be signed, but x is
 * known to be nonnegative. */
template <class X>
void BigUnsigned::initFromPrimitive(X x)
{
    if (x == 0)
        ; // NumberlikeArray already initialized us to zero.
    else {
        // Create a single block.  blk is NULL; no need to delete it.
        cap = 1;
        blk = new Blk[1];
        len = 1;
        blk[0] = Blk(x);
    }
}

/* Ditto, but first check that x is nonnegative.  I could have put the check in
 * initFromPrimitive and let the compiler optimize it out for unsigned-type
 * instantiations, but I wanted to avoid the warning stupidly issued by g++ for
 * a condition that is constant in *any* instantiation, even if not in all. */
template <class X>
void BigUnsigned::initFromSignedPrimitive(X x)
{
    if (x < 0)
        throw "BigUnsigned constructor: "
              "Cannot construct a BigUnsigned from a negative number";
    else
        initFromPrimitive(x);
}

// CONVERSION TO PRIMITIVE INTEGERS

/* Template with the same idea as initFromPrimitive.  This might be slightly
 * slower than the previous version with the masks, but it's much shorter and
 * clearer, which is the library's stated goal. */
template <class X>
X BigUnsigned::convertToPrimitive() const
{
    if (len == 0)
        // The number is zero; return zero.
        return 0;
    else if (len == 1) {
        // The single block might fit in an X.  Try the conversion.
        X x = X(blk[0]);
        // Make sure the result accurately represents the block.
        if (Blk(x) == blk[0])
            // Successful conversion.
            return x;
        // Otherwise fall through.
    }
    throw "BigUnsigned::to<Primitive>: "
          "Value is too big to fit in the requested type";
}

/* Wrap the above in an x >= 0 test to make sure we got a nonnegative result,
 * not a negative one that happened to convert back into the correct nonnegative
 * one.  (E.g., catch incorrect conversion of 2^31 to the long -2^31.)  Again,
 * separated to avoid a g++ warning. */
template <class X>
X BigUnsigned::convertToSignedPrimitive() const
{
    X x = convertToPrimitive<X>();
    if (x >= 0)
        return x;
    else
        throw "BigUnsigned::to(Primitive): "
              "Value is too big to fit in the requested type";
}

template <typename Integer>
bool BigUnsigned::operator==(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator==(BigUnsigned{ x });
}

template <typename Integer>
bool BigUnsigned::operator!=(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator!=(BigUnsigned{ x });
}

template <typename Integer>
bool BigUnsigned::operator<(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator<(BigUnsigned{ x });
}

template <typename Integer>
bool BigUnsigned::operator<=(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator<=(BigUnsigned{ x });
}

template <typename Integer>
bool BigUnsigned::operator>=(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator>=(BigUnsigned{ x });
}

template <typename Integer>
bool BigUnsigned::operator>(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator>(BigUnsigned{ x });
}

// ======================================================================================== //

template <typename Integer>
BigUnsigned BigUnsigned::operator+(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator+(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned BigUnsigned::operator-(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator-(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned BigUnsigned::operator*(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator*(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned BigUnsigned::operator/(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator/(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned BigUnsigned::operator%(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator%(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned BigUnsigned::operator&(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator&(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned BigUnsigned::operator|(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator|(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned BigUnsigned::operator^(const Integer& x) const
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator^(BigUnsigned{ x });
}

// ======================================================================================== //

template <typename Integer>
BigUnsigned& BigUnsigned::operator+=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator+=(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned& BigUnsigned::operator-=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator-=(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned& BigUnsigned::operator*=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator*=(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned& BigUnsigned::operator/=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator/=(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned& BigUnsigned::operator%=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator%=(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned& BigUnsigned::operator&=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator&=(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned& BigUnsigned::operator|=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator|=(BigUnsigned{ x });
}

template <typename Integer>
BigUnsigned& BigUnsigned::operator^=(const Integer& x)
{
    static_assert(std::is_arithmetic<Integer>::value, "Integer type must be arithmetic");
    return operator^=(BigUnsigned{ x });
}
