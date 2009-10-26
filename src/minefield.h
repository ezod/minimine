/******************************************************************************
 * MINEFIELD.H
 * Definition of the Minefield class, used for interfacing with minefield data.
 *
 * Minefield data is stored in the 'mines' array, encoded in 3-bit binary.
 *   Bit 0: Mine present.
 *   Bit 1: Position flagged.
 *   Bit 2: Position dug.
 ******************************************************************************/

#ifndef __MINEFIELD_H_
#define __MINEFIELD_H_

#define COLS 30
#define ROWS 16
#define MINES 99


class Position
{
  public:
    Position( int, int );
    Position( int );
    Position();
    void setPos( int, int );
    void setPos( int );
    int getx();
    int gety();
    bool operator==( const Position & );
  private:
    int x;
    int y;
};

class Minefield
{
  public:
    Minefield();
    void initMines();
    void flagPos( Position );
    void digPos( Position );
    int checkPos( Position );
    unsigned int numMines();
    bool Done();
  private:
    unsigned int mines[ COLS ][ ROWS ];
    unsigned int minesLeft;
    bool lost;
};

#endif
