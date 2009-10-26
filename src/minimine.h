/******************************************************************************
 * MINIMINE.H
 * Definitions used by the GUI and resources.
 ******************************************************************************/

#ifndef __MINIMINE_H_
#define __MINIMINE_H_

#include "minefield.h"
#include "timer.h"

#define _WIN32_IE 0x0400

#define IDI_ICON1 100
#define IDB_WINDOW 101
#define IDB_ELEMENTS 102
#define ID_TRAY 666
#define IDB_MIN 1000
#define IDB_CLOSE 1001
#define IDB_NEW 1002
#define IDS_SCORE 1500
#define IDS_TIME 1600
#define IDB_MINE 2000

int FieldOffset( Position );
bool MakeWindow();
void BuildGUI();
LRESULT CALLBACK WinProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK BtnSubclassProc( HWND, UINT, WPARAM, LPARAM );

Minefield mfield;
Position posClicked;
Timer mtimer;

WNDPROC mainProc;
HWND hWnd = NULL;
HINSTANCE hInstance = NULL;
int h_minimized;

HRGN hWndRgn;
HWND hMinButton;
HWND hCloseButton;
HWND hNewButton;
HWND hScore[ 2 ];
HWND hTime[ 3 ];
HWND hField[ COLS ][ ROWS ];
NOTIFYICONDATA nid;

HBITMAP SkinBmpa;
HDC SkinSdc;
HBITMAP SkinBmpb;
HBITMAP ButtonBmpa;
HDC ButtonSdc;
HBITMAP ButtonBmpb;

#endif
