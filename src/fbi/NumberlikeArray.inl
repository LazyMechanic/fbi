/* BEGIN TEMPLATE DEFINITIONS.  They are present here so that source files that
 * include this header file can generate the necessary real definitions. */

template <class Blk>
const unsigned int NumberlikeArray<Blk>::N = 8 * sizeof(Blk);

template <class Blk>
NumberlikeArray<Blk>::NumberlikeArray(Index c) : cap(c), len(0)
{
    blk = (cap > 0) ? (new Blk[cap]) : nullptr;
}

template <class Blk>
NumberlikeArray<Blk>::NumberlikeArray() : cap(0), len(0)
{
    blk = nullptr;
}

template <class Blk>
NumberlikeArray<Blk>::~NumberlikeArray()
{
    delete[] blk;
}

template <class Blk>
void NumberlikeArray<Blk>::allocate(Index c)
{
    // If the requested capacity is more than the current capacity...
    if (c > cap) {
        // Delete the old number array
        delete[] blk;
        // Allocate the new array
        cap = c;
        blk = new Blk[cap];
    }
}

template <class Blk>
void NumberlikeArray<Blk>::allocateAndCopy(Index c)
{
    // If the requested capacity is more than the current capacity...
    if (c > cap) {
        Blk* oldBlk = blk;
        // Allocate the new number array
        cap = c;
        blk = new Blk[cap];
        // Copy number blocks
        Index i;
        for (i = 0; i < len; i++)
            blk[i] = oldBlk[i];
        // Delete the old array
        delete[] oldBlk;
    }
}

template <class Blk>
NumberlikeArray<Blk>::NumberlikeArray(const NumberlikeArray<Blk>& x) : len(x.len)
{
    // Create array
    cap = len;
    blk = new Blk[cap];
    // Copy blocks
    Index i;
    for (i = 0; i < len; i++)
        blk[i] = x.blk[i];
}

template <class Blk>
NumberlikeArray<Blk>& NumberlikeArray<Blk>::operator=(const NumberlikeArray<Blk>& x)
{
    /* Calls like a = a have no effect; catch them before the aliasing
     * causes a problem */
    if (this == &x)
        return *this;
    // Copy length
    len = x.len;
    // Expand array if necessary
    allocate(len);
    // Copy number blocks
    Index i;
    for (i = 0; i < len; i++)
        blk[i] = x.blk[i];

    return *this;
}

template <class Blk>
NumberlikeArray<Blk>::NumberlikeArray(const Blk* b, Index blen) : cap(blen), len(blen)
{
    // Create array
    blk = new Blk[cap];
    // Copy blocks
    Index i;
    for (i = 0; i < len; i++)
        blk[i] = b[i];
}

template <class Blk>
typename NumberlikeArray<Blk>::Index NumberlikeArray<Blk>::getCapacity() const
{
    return cap;
}

template <class Blk>
typename NumberlikeArray<Blk>::Index NumberlikeArray<Blk>::getLength() const
{
    return len;
}

template <class Blk>
Blk NumberlikeArray<Blk>::getBlock(Index i) const
{
    return blk[i];
}

template <class Blk>
bool NumberlikeArray<Blk>::isEmpty() const
{
    return len == 0;
}

template <class Blk>
bool NumberlikeArray<Blk>::operator==(const NumberlikeArray<Blk>& x) const
{
    if (len != x.len)
        // Definitely unequal.
        return false;
    else {
        // Compare corresponding blocks one by one.
        Index i;
        for (i = 0; i < len; i++)
            if (blk[i] != x.blk[i])
                return false;
        // No blocks differed, so the objects are equal.
        return true;
    }
}

template <class Blk>
bool NumberlikeArray<Blk>::operator!=(const NumberlikeArray<Blk>& x) const
{
    return !operator==(x);
}