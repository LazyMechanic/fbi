#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

#include "NumberlikeArray.hh"

namespace fbi {
/* A BigUnsigned object represents a nonnegative integer of size limited only by
 * available memory.  BigUnsigneds support most mathematical operators and can
 * be converted to and from most primitive integer types.
 *
 * The number is stored as a NumberlikeArray of unsigned longs as if it were
 * written in base 256^sizeof(unsigned long).  The least significant block is
 * first, and the length is such that the most significant block is nonzero. */
class BigUnsigned : protected NumberlikeArray<unsigned long long> {
public:
    // Enumeration for the result of a comparison.
    enum CmpRes { less = -1, equal = 0, greater = 1 };

    // BigUnsigneds are built with a Blk type of unsigned long.
    typedef unsigned long long Blk;

    typedef NumberlikeArray<Blk>::Index Index;
    using NumberlikeArray<Blk>::N;

protected:
    // Creates a BigUnsigned with a capacity; for internal use.
    BigUnsigned(int, Index c);

    // Decreases len to eliminate any leading zero blocks.
    void zapLeadingZeros();

public:
    // Constructs zero.
    BigUnsigned();

    // Copy constructor
    BigUnsigned(const BigUnsigned& x);

    // Assignment operator
    BigUnsigned& operator=(const BigUnsigned& x);

    // Constructor that copies from a given array of blocks.
    BigUnsigned(const Blk* b, Index blen);

    // Destructor.  NumberlikeArray does the delete for us.
    ~BigUnsigned();

    // Constructors from primitive integer types

    BigUnsigned(unsigned long long x);
    BigUnsigned(unsigned long x);
    BigUnsigned(unsigned int x);
    BigUnsigned(unsigned short x);
    BigUnsigned(unsigned char x);

    BigUnsigned(long long x);
    BigUnsigned(long x);
    BigUnsigned(int x);
    BigUnsigned(short x);
    BigUnsigned(char x);

    // Constructor from string
    BigUnsigned(const std::string& str);

protected:
    // Helpers
    template <class X>
    void initFromPrimitive(X x);
    template <class X>
    void initFromSignedPrimitive(X x);

public:
    /* Converters to primitive integer types
     * The implicit conversion operators caused trouble, so these are now
     * named. */
    unsigned long long toUnsignedLongLong() const;
    unsigned long toUnsignedLong() const;
    unsigned int toUnsignedInt() const;
    unsigned short toUnsignedShort() const;
    unsigned char toUnsignedChar() const;

    long long toLongLong() const;
    long toLong() const;
    int toInt() const;
    short toShort() const;
    char toChar() const;

    // Convert to string
    std::string toString() const;

protected:
    // Helpers
    template <class X>
    X convertToSignedPrimitive() const;
    template <class X>
    X convertToPrimitive() const;

public:
    // BIT/BLOCK ACCESSORS

    // Expose these from NumberlikeArray directly.
    using NumberlikeArray<Blk>::getCapacity;
    using NumberlikeArray<Blk>::getLength;

    /* Returns the requested block, or 0 if it is beyond the length (as if
     * the number had 0s infinitely to the left). */
    Blk getBlock(Index i) const;
    /* Sets the requested block.  The number grows or shrinks as necessary. */
    void setBlock(Index i, Blk newBlock);

    // The number is zero if and only if the canonical length is zero.
    bool isZero() const;

    /* Returns the length of the number in bits, i.e., zero if the number
     * is zero and otherwise one more than the largest value of bi for
     * which getBit(bi) returns true. */
    Index bitLength() const;
    /* Get the state of bit bi, which has value 2^bi.  Bits beyond the
     * number's length are considered to be 0. */
    bool getBit(Index bi) const;
    /* Sets the state of bit bi to newBit.  The number grows or shrinks as
     * necessary. */
    void setBit(Index bi, bool newBit);

    // COMPARISONS

    // Compares this to x like Perl's <=>
    CmpRes compareTo(const BigUnsigned& x) const;

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

    /*
     * BigUnsigned and BigInteger both provide three kinds of operators.
     * Here ``big-integer'' refers to BigInteger or BigUnsigned.
     *
     * (1) Overloaded ``return-by-value'' operators:
     *     +, -, *, /, %, unary -, &, |, ^, <<, >>.
     * Big-integer code using these operators looks identical to code using
     * the primitive integer types.  These operators take one or two
     * big-integer inputs and return a big-integer result, which can then
     * be assigned to a BigInteger variable or used in an expression.
     * Example:
     *     BigInteger a(1), b = 1;
     *     BigInteger c = a + b;
     *
     * (2) Overloaded assignment operators:
     *     +=, -=, *=, /=, %=, flipSign, &=, |=, ^=, <<=, >>=, ++, --.
     * Again, these are used on big integers just like on ints.  They take
     * one writable big integer that both provides an operand and receives a
     * result.  Most also take a second read-only operand.
     * Example:
     *     BigInteger a(1), b(1);
     *     a += b;
     *
     * (3) Copy-less operations: `add', `subtract', etc.
     * These named methods take operands as arguments and store the result
     * in the receiver (*this), avoiding unnecessary copies and allocations.
     * `divideWithRemainder' is special: it both takes the dividend from and
     * stores the remainder into the receiver, and it takes a separate
     * object in which to store the quotient.  NOTE: If you are wondering
     * why these don't return a value, you probably mean to use the
     * overloaded return-by-value operators instead.
     *
     * Examples:
     *     BigInteger a(43), b(7), c, d;
     *
     *     c = a + b;   // Now c == 50.
     *     c.add(a, b); // Same effect but without the two copies.
     *
     *     c.divideWithRemainder(b, d);
     *     // 50 / 7; now d == 7 (quotient) and c == 1 (remainder).
     *
     *     // ``Aliased'' calls now do the right thing using a temporary
     *     // copy, but see note on `divideWithRemainder'.
     *     a.add(a, b);
     */

    // COPY-LESS OPERATIONS

    // These 8: Arguments are read-only operands, result is saved in *this.
    void add(const BigUnsigned& a, const BigUnsigned& b);
    void subtract(const BigUnsigned& a, const BigUnsigned& b);
    void multiply(const BigUnsigned& a, const BigUnsigned& b);
    void bitAnd(const BigUnsigned& a, const BigUnsigned& b);
    void bitOr(const BigUnsigned& a, const BigUnsigned& b);
    void bitXor(const BigUnsigned& a, const BigUnsigned& b);
    /* Negative shift amounts translate to opposite-direction shifts,
     * except for -2^(8*sizeof(int)-1) which is unimplemented. */
    void bitShiftLeft(const BigUnsigned& a, int b);
    void bitShiftRight(const BigUnsigned& a, int b);

    /* `a.divideWithRemainder(b, q)' is like `q = a / b, a %= b'.
     * / and % use semantics similar to Knuth's, which differ from the
     * primitive integer semantics under division by zero.  See the
     * implementation in BigUnsigned.cc for details.
     * `a.divideWithRemainder(b, a)' throws an exception: it doesn't make
     * sense to write quotient and remainder into the same variable. */
    void divideWithRemainder(const BigUnsigned& b, BigUnsigned& q);

    /* `divide' and `modulo' are no longer offered.  Use
     * `divideWithRemainder' instead. */

    // OVERLOADED RETURN-BY-VALUE OPERATORS
    template <typename Integer>
    BigUnsigned operator+(const Integer& x) const;
    template <typename Integer>
    BigUnsigned operator-(const Integer& x) const;
    template <typename Integer>
    BigUnsigned operator*(const Integer& x) const;
    template <typename Integer>
    BigUnsigned operator/(const Integer& x) const;
    template <typename Integer>
    BigUnsigned operator%(const Integer& x) const;
    template <typename Integer>
    BigUnsigned operator&(const Integer& x) const;
    template <typename Integer>
    BigUnsigned operator|(const Integer& x) const;
    template <typename Integer>
    BigUnsigned operator^(const Integer& x) const;

    BigUnsigned operator+(const BigUnsigned& x) const;
    BigUnsigned operator-(const BigUnsigned& x) const;
    BigUnsigned operator*(const BigUnsigned& x) const;
    BigUnsigned operator/(const BigUnsigned& x) const;
    BigUnsigned operator%(const BigUnsigned& x) const;
    /* OK, maybe unary minus could succeed in one case, but it really
     * shouldn't be used, so it isn't provided. */
    BigUnsigned operator&(const BigUnsigned& x) const;
    BigUnsigned operator|(const BigUnsigned& x) const;
    BigUnsigned operator^(const BigUnsigned& x) const;
    BigUnsigned operator<<(int b) const;
    BigUnsigned operator>>(int b) const;

    // OVERLOADED ASSIGNMENT OPERATORS
    template <typename Integer>
    BigUnsigned& operator+=(const Integer& x);
    template <typename Integer>
    BigUnsigned& operator-=(const Integer& x);
    template <typename Integer>
    BigUnsigned& operator*=(const Integer& x);
    template <typename Integer>
    BigUnsigned& operator/=(const Integer& x);
    template <typename Integer>
    BigUnsigned& operator%=(const Integer& x);
    template <typename Integer>
    BigUnsigned& operator&=(const Integer& x);
    template <typename Integer>
    BigUnsigned& operator|=(const Integer& x);
    template <typename Integer>
    BigUnsigned& operator^=(const Integer& x);

    BigUnsigned& operator+=(const BigUnsigned& x);
    BigUnsigned& operator-=(const BigUnsigned& x);
    BigUnsigned& operator*=(const BigUnsigned& x);
    BigUnsigned& operator/=(const BigUnsigned& x);
    BigUnsigned& operator%=(const BigUnsigned& x);
    BigUnsigned& operator&=(const BigUnsigned& x);
    BigUnsigned& operator|=(const BigUnsigned& x);
    BigUnsigned& operator^=(const BigUnsigned& x);
    BigUnsigned& operator<<=(int b);
    BigUnsigned& operator>>=(int b);

    // INCREMENT/DECREMENT OPERATORS
    BigUnsigned& operator++();
    BigUnsigned operator++(int);
    BigUnsigned& operator--();
    BigUnsigned operator--(int);

    // Helper function that needs access to BigUnsigned internals
    friend Blk getShiftedBlock(const BigUnsigned& num, Index x, unsigned int y);

    // See BigInteger.cc.
    template <class X>
    friend X convertBigUnsignedToPrimitiveAccess(const BigUnsigned& a);
};

#include "BigUnsigned.inl"
} // namespace fbi
