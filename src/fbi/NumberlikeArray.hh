#pragma once

namespace fbi {
/* A NumberlikeArray<Blk> object holds a heap-allocated array of Blk with a
 * length and a capacity and provides basic memory management features.
 * BigUnsigned and BigUnsignedInABase both subclass it.
 *
 * NumberlikeArray provides no information hiding.  Subclasses should use
 * nonpublic inheritance and manually expose members as desired using
 * declarations like this:
 *
 * public:
 *     NumberlikeArray< the-type-argument >::getLength;
 */
template <class Blk>
class NumberlikeArray {
public:
    // Type for the index of a block in the array
    typedef unsigned int Index;
    // The number of bits in a block, defined below.
    static const unsigned int N;

    // The current allocated capacity of this NumberlikeArray (in blocks)
    Index cap;
    // The actual length of the value stored in this NumberlikeArray (in blocks)
    Index len;
    // Heap-allocated array of the blocks (can be NULL if len == 0)
    Blk* blk;

    // Constructs a ``zero'' NumberlikeArray with the given capacity.
    NumberlikeArray(Index c);

    /* Constructs a zero NumberlikeArray without allocating a backing array.
     * A subclass that doesn't know the needed capacity at initialization
     * time can use this constructor and then overwrite blk without first
     * deleting it. */
    NumberlikeArray();

    // Destructor.  Note that `delete NULL' is a no-op.
    ~NumberlikeArray();

    /* Ensures that the array has at least the requested capacity; may
     * destroy the contents. */
    void allocate(Index c);

    /* Ensures that the array has at least the requested capacity; does not
     * destroy the contents. */
    void allocateAndCopy(Index c);

    // Copy constructor
    NumberlikeArray(const NumberlikeArray<Blk>& x);

    // Assignment operator
    NumberlikeArray<Blk>& operator=(const NumberlikeArray<Blk>& x);

    // Constructor that copies from a given array of blocks
    NumberlikeArray(const Blk* b, Index blen);

    // ACCESSORS
    Index getCapacity() const;
    Index getLength() const;
    Blk getBlock(Index i) const;
    bool isEmpty() const;

    /* Equality comparison: checks if both objects have the same length and
     * equal (==) array elements to that length.  Subclasses may wish to
     * override. */
    bool operator==(const NumberlikeArray<Blk>& x) const;

    bool operator!=(const NumberlikeArray<Blk>& x) const;
};

#include "NumberlikeArray.inl"
} // namespace fbi
