#pragma once

#include <string>
#include <stdexcept>
#include <type_traits>

#include "BigUnsigned.hh"

namespace fbi {
/* A BigInteger object represents a signed integer of size limited only by
 * available memory.  BigUnsigneds support most mathematical operators and can
 * be converted to and from most primitive integer types.
 *
 * A BigInteger is just an aggregate of a BigUnsigned and a sign.  (It is no
 * longer derived from BigUnsigned because that led to harmful implicit
 * conversions.) */
class BigInteger {
public:
    typedef BigUnsigned::Blk Blk;
    typedef BigUnsigned::Index Index;
    typedef BigUnsigned::CmpRes CmpRes;

    static constexpr CmpRes less = BigUnsigned::less;
    static constexpr CmpRes equal = BigUnsigned::equal;
    static constexpr CmpRes greater = BigUnsigned::greater;

    // Enumeration for the sign of a BigInteger.
    enum Sign { negative = -1, zero = 0, positive = 1 };

protected:
    Sign sign;
    BigUnsigned mag;

public:
    // Constructs zero.
    BigInteger();

    // Copy constructor
    BigInteger(const BigInteger& x);

    // Assignment operator
    BigInteger& operator=(const BigInteger& x);

    // Constructor that copies from a given array of blocks with a sign.
    BigInteger(const Blk* b, Index blen, Sign s);

    // Nonnegative constructor that copies from a given array of blocks.
    BigInteger(const Blk* b, Index blen);

    // Constructor from a BigUnsigned and a sign
    BigInteger(const BigUnsigned& x, Sign s);

    // Nonnegative constructor from a BigUnsigned
    BigInteger(const BigUnsigned& x);

    // Constructors from primitive integer types

    // BigInteger(unsigned long x);
    // BigInteger(long x);
    // BigInteger(unsigned int x);
    // BigInteger(int x);
    // BigInteger(unsigned short x);
    // BigInteger(short x);

    BigInteger(unsigned long long x);
    BigInteger(unsigned long x);
    BigInteger(unsigned int x);
    BigInteger(unsigned short x);
    BigInteger(unsigned char x);

    BigInteger(long long x);
    BigInteger(long x);
    BigInteger(int x);
    BigInteger(short x);
    BigInteger(char x);

    // Constructor from string
    BigInteger(const std::string& str);

    /* Converters to primitive integer types
     * The implicit conversion operators caused trouble, so these are now
     * named. */
    unsigned long toUnsignedLong() const;
    long toLong() const;
    unsigned int toUnsignedInt() const;
    int toInt() const;
    unsigned short toUnsignedShort() const;
    short toShort() const;

    // Converter to string
    std::string toString() const;

protected:
    // Helper
    template <class X>
    X convertToUnsignedPrimitive() const;
    template <class X, class UX>
    X convertToSignedPrimitive() const;

public:
    // ACCESSORS
    Sign getSign() const;
    /* The client can't do any harm by holding a read-only reference to the
     * magnitude. */
    const BigUnsigned& getMagnitude() const;

    // Some accessors that go through to the magnitude
    Index getLength() const;
    Index getCapacity() const;
    Blk getBlock(Index i) const;
    bool isZero() const; // A bit special

    // COMPARISONS

    // Compares this to x like Perl's <=>
    CmpRes compareTo(const BigInteger& x) const;

    // Ordinary comparison operators
    template <typename Integer>
    bool operator==(const Integer& x) const;
    template <typename Integer>
    bool operator!=(const Integer& x) const;
    template <typename Integer>
    bool operator<(const Integer& x) const;
    template <typename Integer>
    bool operator<=(const Integer& x) const;
    template <typename Integer>
    bool operator>=(const Integer& x) const;
    template <typename Integer>
    bool operator>(const Integer& x) const;

    bool operator==(const BigUnsigned& x) const;
    bool operator!=(const BigUnsigned& x) const;
    bool operator<(const BigUnsigned& x) const;
    bool operator<=(const BigUnsigned& x) const;
    bool operator>=(const BigUnsigned& x) const;
    bool operator>(const BigUnsigned& x) const;

    bool operator==(const BigInteger& x) const;
    bool operator!=(const BigInteger& x) const;
    bool operator<(const BigInteger& x) const;
    bool operator<=(const BigInteger& x) const;
    bool operator>=(const BigInteger& x) const;
    bool operator>(const BigInteger& x) const;

    // OPERATORS -- See the discussion in BigUnsigned.hh.
    void add(const BigInteger& a, const BigInteger& b);
    void subtract(const BigInteger& a, const BigInteger& b);
    void multiply(const BigInteger& a, const BigInteger& b);
    /* See the comment on BigUnsigned::divideWithRemainder.  Semantics
     * differ from those of primitive integers when negatives and/or zeros
     * are involved. */
    void divideWithRemainder(const BigInteger& b, BigInteger& q);
    void negate(const BigInteger& a);

    /* Bitwise operators are not provided for BigIntegers.  Use
     * getMagnitude to get the magnitude and operate on that instead. */

    template <typename Integer>
    BigInteger operator+(const Integer& x) const;
    template <typename Integer>
    BigInteger operator-(const Integer& x) const;
    template <typename Integer>
    BigInteger operator*(const Integer& x) const;
    template <typename Integer>
    BigInteger operator/(const Integer& x) const;
    template <typename Integer>
    BigInteger operator%(const Integer& x) const;

    BigInteger operator+(const BigUnsigned& x) const;
    BigInteger operator-(const BigUnsigned& x) const;
    BigInteger operator*(const BigUnsigned& x) const;
    BigInteger operator/(const BigUnsigned& x) const;
    BigInteger operator%(const BigUnsigned& x) const;

    BigInteger operator+(const BigInteger& x) const;
    BigInteger operator-(const BigInteger& x) const;
    BigInteger operator*(const BigInteger& x) const;
    BigInteger operator/(const BigInteger& x) const;
    BigInteger operator%(const BigInteger& x) const;
    BigInteger operator-() const;

    // OVERLOADED ASSIGNMENT OPERATORS
    template <typename Integer>
    BigInteger& operator+=(const Integer& x);
    template <typename Integer>
    BigInteger& operator-=(const Integer& x);
    template <typename Integer>
    BigInteger& operator*=(const Integer& x);
    template <typename Integer>
    BigInteger& operator/=(const Integer& x);
    template <typename Integer>
    BigInteger& operator%=(const Integer& x);

    BigInteger& operator+=(const BigUnsigned& x);
    BigInteger& operator-=(const BigUnsigned& x);
    BigInteger& operator*=(const BigUnsigned& x);
    BigInteger& operator/=(const BigUnsigned& x);
    BigInteger& operator%=(const BigUnsigned& x);

    BigInteger& operator+=(const BigInteger& x);
    BigInteger& operator-=(const BigInteger& x);
    BigInteger& operator*=(const BigInteger& x);
    BigInteger& operator/=(const BigInteger& x);
    BigInteger& operator%=(const BigInteger& x);
    BigInteger& flipSign();

    // INCREMENT/DECREMENT OPERATORS
    BigInteger& operator++();
    BigInteger operator++(int);
    BigInteger& operator--();
    BigInteger operator--(int);
};

#include "BigInteger.inl"
} // namespace fbi
