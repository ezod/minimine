/******************************************************************************
 * TIMER.H
 * Definition of the timer class.
 ******************************************************************************/

#ifndef __TIMER_H_
#define __TIMER_H_

#include <windows.h>

#define TIMER_MAX 999

class Timer
{
  public:
    Timer();
    void Start();
    void Stop();
    void Reset();
    int getTimer();
    void setWnd( HWND );
  private:
    static DWORD WINAPI timerThread( LPVOID );
    bool tmr_enabled;
    int timer;
    HWND tWnd;
};

#endif
