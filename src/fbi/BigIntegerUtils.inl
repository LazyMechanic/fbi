
template <std::size_t N>
BigUnsigned bitsetToBigUnsigned(const std::bitset<N>& b)
{
    return stringToBigUnsigned(b.to_string());
}

template <std::size_t N>
BigInteger bitsetToBigInteger(const std::bitset<N>& b)
{
    return stringToBigInteger(b.to_string());
}

template <class T>
BigInteger dataToBigInteger(const T* data, BigInteger::Index length, BigInteger::Sign sign)
{
    // really ceiling(numBytes / sizeof(BigInteger::Blk))
    unsigned int pieceSizeInBits = 8 * sizeof(T);
    unsigned int piecesPerBlock = sizeof(BigInteger::Blk) / sizeof(T);
    unsigned int numBlocks = (length + piecesPerBlock - 1) / piecesPerBlock;

    // Allocate our block array
    BigInteger::Blk* blocks = new BigInteger::Blk[numBlocks];

    BigInteger::Index blockNum, pieceNum, pieceNumHere;

    // Convert
    for (blockNum = 0, pieceNum = 0; blockNum < numBlocks; blockNum++) {
        BigInteger::Blk curBlock = 0;
        for (pieceNumHere = 0; pieceNumHere < piecesPerBlock && pieceNum < length; pieceNumHere++, pieceNum++)
            curBlock |= (BigInteger::Blk(data[pieceNum]) << (pieceSizeInBits * pieceNumHere));
        blocks[blockNum] = curBlock;
    }

    // Create the BigInteger.
    BigInteger x(blocks, numBlocks, sign);

    delete[] blocks;
    return x;
}

