#include "BigInteger.hh"

#include "BigIntegerUtils.hh"

namespace fbi {
BigInteger::BigInteger() : sign(zero), mag() {}

BigInteger::BigInteger(const BigInteger& x) : sign(x.sign), mag(x.mag) {}

BigInteger& BigInteger::operator=(const BigInteger& x)
{
    // Calls like a = a have no effect
    if (this == &x)
        return *this;
    // Copy sign
    sign = x.sign;
    // Copy the rest
    mag = x.mag;

    return *this;
}

BigInteger::BigInteger(const Blk* b, Index blen, Sign s) : mag(b, blen)
{
    switch (s) {
        case zero:
            if (!mag.isZero())
                throw SignError{ "BigInteger::BigInteger(const Blk *, Index, Sign)",
                                 "Cannot use a sign of zero with a nonzero magnitude" };
            sign = zero;
            break;
        case positive:
        case negative:
            // If the magnitude is zero, force the sign to zero.
            sign = mag.isZero() ? zero : s;
            break;
        default:
            /* g++ seems to be optimizing out this case on the assumption
             * that the sign is a valid member of the enumeration.  Oh well. */
            throw SignError{ "BigInteger::BigInteger(const Blk *, Index, Sign)", "Invalid sign" };
    }
}

BigInteger::BigInteger(const Blk* b, Index blen) : mag(b, blen)
{
    sign = mag.isZero() ? zero : positive;
}

BigInteger::BigInteger(const BigUnsigned& x, Sign s) : mag(x)
{
    switch (s) {
        case zero:
            if (!mag.isZero())
                throw SignError{ "BigInteger::BigInteger(const BigUnsigned &, Sign)",
                                 "Cannot use a sign of zero with a nonzero magnitude" };

            sign = zero;
            break;
        case positive:
        case negative:
            // If the magnitude is zero, force the sign to zero.
            sign = mag.isZero() ? zero : s;
            break;
        default:
            /* g++ seems to be optimizing out this case on the assumption
             * that the sign is a valid member of the enumeration.  Oh well. */
            throw SignError{ "BigInteger::BigInteger(const BigUnsigned &, Sign)", "Invalid sign" };
    }
}

BigInteger::BigInteger(const BigUnsigned& x) : mag(x)
{
    sign = mag.isZero() ? zero : positive;
}

namespace {
template <class X, class UX>
BigInteger::Blk magOf(X x)
{
    /* UX(...) cast needed to stop short(-2^15), which negates to
     * itself, from sign-extending in the conversion to Blk. */
    return BigInteger::Blk(x < 0 ? UX(-x) : x);
}
template <class X>
BigInteger::Sign signOf(X x)
{
    return (x == 0) ? BigInteger::zero : (x > 0) ? BigInteger::positive : BigInteger::negative;
}
} // namespace

/* CONSTRUCTION FROM PRIMITIVE INTEGERS
 * Same idea as in BigUnsigned.cc, except that negative input results in a
 * negative BigInteger instead of an exception. */

// Done longhand to let us use initialization.

BigInteger::BigInteger(unsigned long long x) : mag(x)
{
    sign = mag.isZero() ? zero : positive;
}

BigInteger::BigInteger(unsigned long x) : mag(x)
{
    sign = mag.isZero() ? zero : positive;
}

BigInteger::BigInteger(unsigned x) : mag(x)
{
    sign = mag.isZero() ? zero : positive;
}

BigInteger::BigInteger(unsigned short x) : mag(x)
{
    sign = mag.isZero() ? zero : positive;
}

BigInteger::BigInteger(unsigned char x) : mag(x)
{
    sign = mag.isZero() ? zero : positive;
}

BigInteger::BigInteger(long long x) : sign(signOf(x)), mag(magOf<long long, unsigned long long>(x)) {}

BigInteger::BigInteger(long x) : sign(signOf(x)), mag(magOf<long, unsigned long>(x)) {}

BigInteger::BigInteger(int x) : sign(signOf(x)), mag(magOf<int, unsigned int>(x)) {}

BigInteger::BigInteger(short x) : sign(signOf(x)), mag(magOf<short, unsigned short>(x)) {}

BigInteger::BigInteger(char x) : sign(signOf(x)), mag(magOf<char, unsigned char>(x)) {}

// For signed input, determine the desired magnitude and sign separately.

BigInteger::BigInteger(const std::string& str) : BigInteger(stringToBigInteger(str)) {}

unsigned long long BigInteger::toUnsignedLongLong() const
{
    return convertToUnsignedPrimitive<unsigned long long>();
}

unsigned long BigInteger::toUnsignedLong() const
{
    return convertToUnsignedPrimitive<unsigned long>();
}

unsigned int BigInteger::toUnsignedInt() const
{
    return convertToUnsignedPrimitive<unsigned int>();
}

unsigned short BigInteger::toUnsignedShort() const
{
    return convertToUnsignedPrimitive<unsigned short>();
}

unsigned char BigInteger::toUnsignedChar() const
{
    return convertToUnsignedPrimitive<unsigned char>();
}

long long BigInteger::toLongLong() const
{
    return convertToSignedPrimitive<long long, unsigned long long>();
}

long BigInteger::toLong() const
{
    return convertToSignedPrimitive<long, unsigned long>();
}

int BigInteger::toInt() const
{
    return convertToSignedPrimitive<int, unsigned int>();
}

short BigInteger::toShort() const
{
    return convertToSignedPrimitive<short, unsigned short>();
}

char BigInteger::toChar() const
{
    return convertToSignedPrimitive<char, unsigned char>();
}

std::string BigInteger::toString() const
{
    return bigIntegerToString(*this);
}

BigInteger::Sign BigInteger::getSign() const
{
    return sign;
}

const BigUnsigned& BigInteger::getMagnitude() const

{
    return mag;
}

BigInteger::Index BigInteger::getLength() const

{
    return mag.getLength();
}

BigInteger::Index BigInteger::getCapacity() const
{
    return mag.getCapacity();
}

BigInteger::Blk BigInteger::getBlock(Index i) const
{
    return mag.getBlock(i);
}

bool BigInteger::isZero() const

{
    return sign == zero;
}

// COMPARISON
BigInteger::CmpRes BigInteger::compareTo(const BigInteger& x) const
{
    // A greater sign implies a greater number
    if (sign < x.sign)
        return less;
    else if (sign > x.sign)
        return greater;
    else
        switch (sign) {
                // If the signs are the same...
            case zero:
                return equal; // Two zeros are equal
            case positive:
                // Compare the magnitudes
                return mag.compareTo(x.mag);
            case negative:
                // Compare the magnitudes, but return the opposite result
                return CmpRes(-mag.compareTo(x.mag));
            default:
                throw SignError{ "BigInteger::compareTo(const BigInteger& x)", "Invalid sign" };
        }
}

bool BigInteger::operator==(const BigUnsigned& x) const
{
    return operator==(BigInteger{ x });
}

bool BigInteger::operator!=(const BigUnsigned& x) const
{
    return operator!=(BigInteger{ x });
}

bool BigInteger::operator<(const BigUnsigned& x) const
{
    return operator<(BigInteger{ x });
}

bool BigInteger::operator<=(const BigUnsigned& x) const
{
    return operator<=(BigInteger{ x });
}

bool BigInteger::operator>=(const BigUnsigned& x) const
{
    return operator>=(BigInteger{ x });
}

bool BigInteger::operator>(const BigUnsigned& x) const
{
    return operator>(BigInteger{ x });
}

bool BigInteger::operator==(const BigInteger& x) const

{
    return sign == x.sign && mag == x.mag;
}

bool BigInteger::operator!=(const BigInteger& x) const
{
    return !operator==(x);
}

bool BigInteger::operator<(const BigInteger& x) const
{
    return compareTo(x) == less;
}

bool BigInteger::operator<=(const BigInteger& x) const
{
    return compareTo(x) != greater;
}

bool BigInteger::operator>=(const BigInteger& x) const
{
    return compareTo(x) != less;
}

bool BigInteger::operator>(const BigInteger& x) const
{
    return compareTo(x) == greater;
}

/* COPY-LESS OPERATIONS
 * These do some messing around to determine the sign of the result,
 * then call one of BigUnsigned's copy-less operations. */

// See remarks about aliased calls in BigUnsigned.cc .
#define DTRT_ALIASED(cond, op) \
    if (cond) {                \
        BigInteger tmpThis;    \
        tmpThis.op;            \
        *this = tmpThis;       \
        return;                \
    }

void BigInteger::add(const BigInteger& a, const BigInteger& b)
{
    DTRT_ALIASED(this == &a || this == &b, add(a, b));
    // If one argument is zero, copy the other.
    if (a.sign == zero)
        operator=(b);
    else if (b.sign == zero)
        operator=(a);
    // If the arguments have the same sign, take the
    // common sign and add their magnitudes.
    else if (a.sign == b.sign) {
        sign = a.sign;
        mag.add(a.mag, b.mag);
    }
    else {
        // Otherwise, their magnitudes must be compared.
        switch (a.mag.compareTo(b.mag)) {
            case equal:
                // If their magnitudes are the same, copy zero.
                mag = 0;
                sign = zero;
                break;
                // Otherwise, take the sign of the greater, and subtract
                // the lesser magnitude from the greater magnitude.
            case greater:
                sign = a.sign;
                mag.subtract(a.mag, b.mag);
                break;
            case less:
                sign = b.sign;
                mag.subtract(b.mag, a.mag);
                break;
        }
    }
}

void BigInteger::subtract(const BigInteger& a, const BigInteger& b)
{
    // Notice that this routine is identical to BigInteger::add,
    // if one replaces b.sign by its opposite.
    DTRT_ALIASED(this == &a || this == &b, subtract(a, b));
    // If a is zero, copy b and flip its sign.  If b is zero, copy a.
    if (a.sign == zero) {
        mag = b.mag;
        // Take the negative of _b_'s, sign, not ours.
        // Bug pointed out by Sam Larkin on 2005.03.30.
        sign = Sign(-b.sign);
    }
    else if (b.sign == zero)
        operator=(a);
    // If their signs differ, take a.sign and add the magnitudes.
    else if (a.sign != b.sign) {
        sign = a.sign;
        mag.add(a.mag, b.mag);
    }
    else {
        // Otherwise, their magnitudes must be compared.
        switch (a.mag.compareTo(b.mag)) {
                // If their magnitudes are the same, copy zero.
            case equal:
                mag = 0;
                sign = zero;
                break;
                // If a's magnitude is greater, take a.sign and
                // subtract a from b.
            case greater:
                sign = a.sign;
                mag.subtract(a.mag, b.mag);
                break;
                // If b's magnitude is greater, take the opposite
                // of b.sign and subtract b from a.
            case less:
                sign = Sign(-b.sign);
                mag.subtract(b.mag, a.mag);
                break;
        }
    }
}

void BigInteger::multiply(const BigInteger& a, const BigInteger& b)
{
    DTRT_ALIASED(this == &a || this == &b, multiply(a, b));
    // If one object is zero, copy zero and return.
    if (a.sign == zero || b.sign == zero) {
        sign = zero;
        mag = 0;
        return;
    }
    // If the signs of the arguments are the same, the result
    // is positive, otherwise it is negative.
    sign = (a.sign == b.sign) ? positive : negative;
    // Multiply the magnitudes.
    mag.multiply(a.mag, b.mag);
}

/*
 * DIVISION WITH REMAINDER
 * Please read the comments before the definition of
 * `BigUnsigned::divideWithRemainder' in `BigUnsigned.cc' for lots of
 * information you should know before reading this function.
 *
 * Following Knuth, I decree that x / y is to be
 * 0 if y==0 and floor(real-number x / y) if y!=0.
 * Then x % y shall be x - y*(integer x / y).
 *
 * Note that x = y * (x / y) + (x % y) always holds.
 * In addition, (x % y) is from 0 to y - 1 if y > 0,
 * and from -(|y| - 1) to 0 if y < 0.  (x % y) = x if y = 0.
 *
 * Examples: (q = a / b, r = a % b)
 *	a	b	q	r
 *	===	===	===	===
 *	4	3	1	1
 *	-4	3	-2	2
 *	4	-3	-2	-2
 *	-4	-3	1	-1
 */
void BigInteger::divideWithRemainder(const BigInteger& b, BigInteger& q)
{
    // Defend against aliased calls;
    // same idea as in BigUnsigned::divideWithRemainder .
    if (this == &q)
        throw MathError{ "BigInteger::divideWithRemainder",
                         "Cannot write quotient and remainder into the same variable" };
    if (this == &b || &q == &b) {
        BigInteger tmpB(b);
        divideWithRemainder(tmpB, q);
        return;
    }

    // Division by zero gives quotient 0 and remainder *this
    if (b.sign == zero) {
        q.mag = 0;
        q.sign = zero;
        return;
    }
    // 0 / b gives quotient 0 and remainder 0
    if (sign == zero) {
        q.mag = 0;
        q.sign = zero;
        return;
    }

    // Here *this != 0, b != 0.

    // Do the operands have the same sign?
    if (sign == b.sign) {
        // Yes: easy case.  Quotient is zero or positive.
        q.sign = positive;
    }
    else {
        // No: harder case.  Quotient is negative.
        q.sign = negative;
        // Decrease the magnitude of the dividend by one.
        mag--;
        /*
         * We tinker with the dividend before and with the
         * quotient and remainder after so that the result
         * comes out right.  To see why it works, consider the following
         * list of examples, where A is the magnitude-decreased
         * a, Q and R are the results of BigUnsigned division
         * with remainder on A and |b|, and q and r are the
         * final results we want:
         *
         *	a	A	b	Q	R	q	r
         *	-3	-2	3	0	2	-1	0
         *	-4	-3	3	1	0	-2	2
         *	-5	-4	3	1	1	-2	1
         *	-6	-5	3	1	2	-2	0
         *
         * It appears that we need a total of 3 corrections:
         * Decrease the magnitude of a to get A.  Increase the
         * magnitude of Q to get q (and make it negative).
         * Find r = (b - 1) - R and give it the desired sign.
         */
    }

    // Divide the magnitudes.
    mag.divideWithRemainder(b.mag, q.mag);

    if (sign != b.sign) {
        // More for the harder case (as described):
        // Increase the magnitude of the quotient by one.
        q.mag++;
        // Modify the remainder.
        mag.subtract(b.mag, mag);
        mag--;
    }

    // Sign of the remainder is always the sign of the divisor b.
    sign = b.sign;

    // Set signs to zero as necessary.  (Thanks David Allen!)
    if (mag.isZero())
        sign = zero;
    if (q.mag.isZero())
        q.sign = zero;

    // WHEW!!!
}

// Negation
void BigInteger::negate(const BigInteger& a)
{
    DTRT_ALIASED(this == &a, negate(a));
    // Copy a's magnitude
    mag = a.mag;
    // Copy the opposite of a.sign
    sign = Sign(-a.sign);
}

/*
 * NORMAL OPERATORS
 *
 * These create an object to hold the result and invoke
 * the appropriate put-here operation on it, passing
 * this and x.  The new object is then returned.
 */

BigInteger BigInteger::operator+(const BigUnsigned& x) const
{
    return operator+(BigInteger{ x });
}

BigInteger BigInteger::operator-(const BigUnsigned& x) const
{
    return operator-(BigInteger{ x });
}

BigInteger BigInteger::operator*(const BigUnsigned& x) const
{
    return operator*(BigInteger{ x });
}

BigInteger BigInteger::operator/(const BigUnsigned& x) const
{
    return operator/(BigInteger{ x });
}

BigInteger BigInteger::operator%(const BigUnsigned& x) const
{
    return operator%(BigInteger{ x });
}

BigInteger BigInteger::operator+(const BigInteger& x) const
{
    BigInteger ans;
    ans.add(*this, x);
    return ans;
}

BigInteger BigInteger::operator-(const BigInteger& x) const
{
    BigInteger ans;
    ans.subtract(*this, x);
    return ans;
}

BigInteger BigInteger::operator*(const BigInteger& x) const
{
    BigInteger ans;
    ans.multiply(*this, x);
    return ans;
}

BigInteger BigInteger::operator/(const BigInteger& x) const
{
    if (x.isZero())
        throw DivideByZeroError{ "BigInteger::operator /" };
    BigInteger q, r;
    r = *this;
    r.divideWithRemainder(x, q);
    return q;
}

BigInteger BigInteger::operator%(const BigInteger& x) const
{
    if (x.isZero())
        throw DivideByZeroError{ "BigInteger::operator %" };
    BigInteger q, r;
    r = *this;
    r.divideWithRemainder(x, q);
    return r;
}

BigInteger BigInteger::operator-() const
{
    BigInteger ans;
    ans.negate(*this);
    return ans;
}

/*
 * ASSIGNMENT OPERATORS
 *
 * Now the responsibility for making a temporary copy if necessary
 * belongs to the put-here operations.  See Assignment Operators in
 * BigUnsigned.hh.
 */

BigInteger& BigInteger::operator+=(const BigUnsigned& x)
{
    return operator+=(BigInteger{ x });
}

BigInteger& BigInteger::operator-=(const BigUnsigned& x)
{
    return operator-=(BigInteger{ x });
}

BigInteger& BigInteger::operator*=(const BigUnsigned& x)
{
    return operator*=(BigInteger{ x });
}

BigInteger& BigInteger::operator/=(const BigUnsigned& x)
{
    return operator/=(BigInteger{ x });
}

BigInteger& BigInteger::operator%=(const BigUnsigned& x)
{
    return operator%=(BigInteger{ x });
}

BigInteger& BigInteger::operator+=(const BigInteger& x)
{
    add(*this, x);
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& x)
{
    subtract(*this, x);
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& x)
{
    multiply(*this, x);
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& x)
{
    if (x.isZero())
        throw DivideByZeroError{ "BigInteger::operator /=" };
    /* The following technique is slightly faster than copying *this first
     * when x is large. */
    BigInteger q;
    divideWithRemainder(x, q);
    // *this contains the remainder, but we overwrite it with the quotient.
    *this = q;
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& x)
{
    if (x.isZero())
        throw DivideByZeroError{ "BigInteger::operator %=" };
    BigInteger q;
    // Mods *this by x.  Don't care about quotient left in q.
    divideWithRemainder(x, q);
    return *this;
}

// This one is trivial
BigInteger& BigInteger::flipSign()
{
    sign = Sign(-sign);
    return *this;
}

// INCREMENT/DECREMENT OPERATORS

// Prefix increment
BigInteger& BigInteger::operator++()
{
    if (sign == negative) {
        mag--;
        if (mag == 0)
            sign = zero;
    }
    else {
        mag++;
        sign = positive; // if not already
    }

    return *this;
}

// Postfix increment: same as prefix
BigInteger BigInteger::operator++(int)
{
    BigInteger res = *this;
    operator++();

    return res;
}

// Prefix decrement
BigInteger& BigInteger::operator--()
{
    if (sign == positive) {
        mag--;
        if (mag == 0)
            sign = zero;
    }
    else {
        mag++;
        sign = negative;
    }

    return *this;
}

// Postfix decrement: same as prefix
BigInteger BigInteger::operator--(int)
{
    BigInteger res = *this;
    operator--();

    return res;
}
} // namespace fbi