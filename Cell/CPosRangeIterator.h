#ifndef CPOSRANGE_H
#define CPOSRANGE_H
#include "CPos.h"


/**
 *  Takes two positions and iterates over all positions in the rectangle defined by them
 *  iterates from left to right, top to bottom
 */
class CPosRangeIterator {
public:
    /**
        *  Constructor
        *  takes two corners and computes right upper and left lower corners
        * @param start one of the rectangle corners
        * @param end  the other rectangle corner
        */
    CPosRangeIterator(const CPos &start, const CPos &end);

    CPosRangeIterator(const CPosRangeIterator &other);

    CPosRangeIterator(CPosRangeIterator &&other) noexcept;

    CPosRangeIterator &operator=(CPosRangeIterator other);

    /**
     *  Checks if the iterator is at the end
     * @return true if the iterator is at the end
     */
    bool isEnd() const;

    /**
     *  Resets the iterator to the start
     */
    void reset();

    /**
     *  Moves the iterator to the next position
     *  left to right, top to bottom
     * @return reference to this
     */
    CPosRangeIterator &operator++();

    /**
     *  Returns the current position
     * @return  current position
     */
    CPos get() const;

private:
    /**
     *  Offset of the active position in the x direction from the left upper corner
     */
    size_t m_activeXOffset;


    /**
     *  Offset of the active position in the y direction from the left upper corner
     */
    size_t m_activeYOffset;

    /**
    *  left upper corner of the rectangle
    */
    CPos m_leftUpper;


    /**
     *  right lower corner of the rectangle
     */
    CPos m_rightLower;
};
#endif //CPOSRANGE_H
