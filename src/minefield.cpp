/******************************************************************************
 * MINEFIELD.CPP
 * Implementation of the Position and Minefield classes.
 ******************************************************************************/

#include <ctime>
#include <cstdlib>

#include "minefield.h"


/* Constructor function for the Position class using coordinates.
 ******************************************************************************/
Position::Position( int a, int b )
{
  setPos( a, b );
}


/* Constructor function for the Position class using button ID.
 ******************************************************************************/
Position::Position( int btn )
{
  setPos( btn );
}


/* Default constructor function for the Position class.
 ******************************************************************************/
Position::Position()
{
  x = 0;
  y = 0;
}


/* Sets the coordinates of the position using coordinates.
 ******************************************************************************/
void Position::setPos( int a, int b )
{
  x = ( a > -1 && a < COLS )?a:0;
  y = ( b > -1 && b < ROWS )?b:0;
}


/* Sets the coordinates of the position using button ID.
 ******************************************************************************/
void Position::setPos( int btn )
{
  int a, b;
  a = btn % COLS;
  b = ( btn - a ) / COLS;
  setPos( a, b );
}


/* Returns the X coordinate.
 ******************************************************************************/
int Position::getx()
{
  return x;
}


/* Returns the Y coordinate.
 ******************************************************************************/
int Position::gety()
{
  return y;
}


/* Overloaded equality operator.
 ******************************************************************************/
bool Position::operator==( const Position &pos )
{
  return ( this->x == pos.x && this->y == pos.y );
}


/* Constructor function for the Minefield class.
 ******************************************************************************/
Minefield::Minefield()
{
  initMines();
}


/* Initializes the mine array with a randomly generated field of mines.
 ******************************************************************************/
void Minefield::initMines()
{
  int i, x, y;

  // initialize the array to zeros
  for( x = 0; x < COLS; x++ )
    for( y = 0; y < ROWS; y++ )
      mines[ x ][ y ] = 0;

  // place random 1s throughout the array
  srand( ( unsigned )time( 0 ) );
  i = 0;
  while( i < MINES )
  {
    x = int( COLS * rand() / (RAND_MAX + 1.0 ) );
    y = int( ROWS * rand() / (RAND_MAX + 1.0 ) );
    if( ! ( mines[ x ][ y ] % 2 ) )
    {
      mines[ x ][ y ] += 1;
      i++;
    }
  }
  minesLeft = MINES;
  lost = false;
}


/* Flags or unflags a position.
 ******************************************************************************/
void Minefield::flagPos( Position pos )
{
  if( ( mines[ pos.getx() ][ pos.gety() ] % 4 ) < 2 && minesLeft )
  {
    mines[ pos.getx() ][ pos.gety() ] += 2;
    minesLeft--;
  }
  else if( ( mines[ pos.getx() ][ pos.gety() ] % 4 ) > 1 )
  {
    mines[ pos.getx() ][ pos.gety() ] -= 2;
    minesLeft++;
  }
}


/* Digs a position, if it has not already been dug.
 ******************************************************************************/
void Minefield::digPos( Position pos )
{
  int m, n;
  if( mines[ pos.getx() ][ pos.gety() ] < 4 )
  {
    mines[ pos.getx() ][ pos.gety() ] += 4;
    if( checkPos( pos ) == 0 )
    {
      for( n = 0; n < 9; n++ )
        if( ( pos.getx() + ( n / 3 ) - 1 ) > -1 &&
            ( pos.gety() + ( n % 3 ) - 1 ) > -1 &&
            ( pos.getx() + ( n / 3 ) - 1 ) < COLS &&
            ( pos.gety() + ( n % 3 ) - 1 ) < ROWS )
          digPos( Position( pos.getx() + ( n / 3 ) - 1,
                            pos.gety() + ( n % 3 ) - 1 ) );
    }
    else if( checkPos( pos ) == 9 )
    {
      for( m = 0; m < COLS; m++ )
        for( n = 0; n < ROWS; n++ )
          if( mines[ m ][ n ] % 4 == 1 )
            digPos( Position( m, n ) );
      minesLeft = 0;
      lost = true;
    }
  }
}


/* Checks the status of a position.
 *
 * Returns a number from 0 to 8 of surrounding mines, 9 if the position itself
 * contains a mine, and -1 if the position has not been dug.
 ******************************************************************************/
int Minefield::checkPos( Position pos )
{
  if( mines[ pos.getx() ][ pos.gety() ] > 3 )
  {
    if( mines[ pos.getx() ][ pos.gety() ] % 2 )
      return 9;
    else
    {
      int m = 0, n;
      for( n = 0; n < 9; n++ )
        if( ( pos.getx() + ( n / 3 ) - 1 ) > -1 &&
            ( pos.gety() + ( n % 3 ) - 1 ) > -1 &&
            ( pos.getx() + ( n / 3 ) - 1 ) < COLS &&
            ( pos.gety() + ( n % 3 ) - 1 ) < ROWS )
          m += mines[ pos.getx() + ( n / 3 ) - 1 ]
                    [ pos.gety() + ( n % 3 ) - 1 ] % 2;
      return m;
    }
  }
  else if( mines[ pos.getx() ][ pos.gety() ] > 1 )
    return ( ( mines[ pos.getx() ][ pos.gety() ] % 2 )?-2:-3 );
  else
    return -1;
}


/* Returns the number of mines left.
 ******************************************************************************/
unsigned int Minefield::numMines()
{
  return minesLeft;
}


/* Returns whether the field is inactive.
 ******************************************************************************/
bool Minefield::Done()
{
  int m, n;
  if( lost )
    return true;
  else if( ! minesLeft )
  {
    for( m = 0; m < COLS; m++ )
      for( n = 0; n < ROWS; n++ )
        if( checkPos( Position( m, n ) ) == -1 ||
            checkPos( Position( m, n ) ) == -3 )
          return false;
    return true;
  }
  return false;
}
