/******************************************************************************
 * TIMER.CPP
 * Implementation of the timer class.
 ******************************************************************************/

#include "timer.h"


/* Constructor function for the Timer class.
 ******************************************************************************/
Timer::Timer()
{
  Reset();
  DWORD dwThreadID;
  CreateThread( NULL, 0, timerThread, ( VOID * )this, 0, &dwThreadID );
}


/* Starts the timer.
 ******************************************************************************/
void Timer::Start()
{
  tmr_enabled = true;
}


/* Stops the timer.
 ******************************************************************************/
void Timer::Stop()
{
  tmr_enabled = false;
}


/* Resets the timer.
 ******************************************************************************/
void Timer::Reset()
{
  timer = 0;
}


/* Returns the timer value.
 ******************************************************************************/
int Timer::getTimer()
{
  return timer;
}


/* Sets the timer window.
 *
 * hWnd: The HWND identifier of the program window.
 ******************************************************************************/
void Timer::setWnd( HWND hWnd )
{
  tWnd = hWnd;
}


/* Thread for timing.
 ******************************************************************************/
DWORD Timer::timerThread( LPVOID pvoid )
{
  while( 1 )
  {
    Sleep( 1000 );
    if( ( ( Timer * )pvoid )->tmr_enabled &&
        ( ( Timer * )pvoid )->timer < TIMER_MAX )
    {
      ( ( Timer * )pvoid )->timer++;
      SendMessage( ( ( Timer * )pvoid )->tWnd, WM_USER + 1, 0, 0 );
    }
  }
  return ( DWORD )0;
}
