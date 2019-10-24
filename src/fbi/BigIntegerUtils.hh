#pragma once

#include <iostream>
#include <string>
#include <bitset>

#include "BigInteger.hh"

namespace fbi {
/* This file provides:
 * - Convenient std::string <-> BigUnsigned/BigInteger conversion routines
 * - std::ostream << operators for BigUnsigned/BigInteger */

// std::string conversion routines.  Base 10 only.
std::string bigUnsignedToString(const BigUnsigned& x);
std::string bigIntegerToString(const BigInteger& x);

BigUnsigned stringToBigUnsigned(const std::string& s);
BigInteger stringToBigInteger(const std::string& s);

template <std::size_t N>
BigUnsigned bitsetToBigUnsigned(const std::bitset<N>& b);
template <std::size_t N>
BigInteger bitsetToBigInteger(const std::bitset<N>& b);

// Creates a BigInteger from data such as `char's; read below for details.
template <class T>
BigInteger dataToBigInteger(const T* data, BigInteger::Index length, BigInteger::Sign sign);

// Outputs x to os, obeying the flags `dec', `hex', `bin', and `showbase'.
std::ostream& operator<<(std::ostream& os, const BigUnsigned& x);

// Outputs x to os, obeying the flags `dec', `hex', `bin', and `showbase'.
// My somewhat arbitrary policy: a negative sign comes before a base indicator (like -0xFF).
std::ostream& operator<<(std::ostream& os, const BigInteger& x);

// BEGIN TEMPLATE DEFINITIONS.
/*
 * Converts binary data to a BigInteger.
 * Pass an array `data', its length, and the desired sign.
 *
 * Elements of `data' may be of any type `T' that has the following
 * two properties (this includes almost all integral types):
 *
 * (1) `sizeof(T)' correctly gives the amount of binary data in one
 * value of `T' and is a factor of `sizeof(Blk)'.
 *
 * (2) When a value of `T' is casted to a `Blk', the low bytes of
 * the result contain the desired binary data.
 */

template <class T>
BigInteger dataToBigInteger(const T* data, BigInteger::Index length, BigInteger::Sign sign);

#include "BigIntegerUtils.inl"
} // namespace fbi
