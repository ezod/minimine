/******************************************************************************
 * MINIMINE.CPP
 * Main program. Initializes the GUI.
 ******************************************************************************/

#include <windows.h>
#include <commctrl.h>
#include <cmath>

#include "minimine.h"


/* Main program.
 ******************************************************************************/
int WINAPI WinMain( HINSTANCE whInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow ) 
{
  hInstance = whInstance;

  // initialize the GUI
  if( !MakeWindow() ) return -1;
  BuildGUI();
  ShowWindow( hWnd, SW_SHOW );

  // tell the timer what window to talk to
  mtimer.setWnd( hWnd );

  // pump messages
  MSG Msg;
  while( GetMessage( &Msg, NULL, 0, 0 ) )
  {
      TranslateMessage( &Msg );
      DispatchMessage( &Msg );
  }

  // clean up on exit
  DestroyWindow( hWnd );
  return Msg.wParam;
}


/* Checks the minefield for the correct bitmap offset for a position.
 *
 * pos: The position to be checked.
 ******************************************************************************/
int FieldOffset( Position pos )
{
  int offset;
  offset = mfield.checkPos( pos );
  if( offset == -1 )
    return 3;
  else if( offset == -2 )
    return 4;
  else if( offset == -3 )
    return ( mfield.Done()?16:4 );
  else if( offset == 9 && posClicked == pos )
    return 15;
  else
    return 5 + offset;
}


/* Creates the Minimine window.
 ******************************************************************************/
bool MakeWindow()
{
  WNDCLASS wndWc;
  wndWc.style = CS_OWNDC;
  wndWc.lpfnWndProc = ( WNDPROC )WinProc;
  wndWc.cbClsExtra = 0;
  wndWc.cbWndExtra = 0;
  wndWc.hInstance = GetModuleHandle( NULL );
  wndWc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON1 ) );
  wndWc.hCursor = LoadCursor( 0, IDC_ARROW );
  wndWc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
  wndWc.lpszMenuName = NULL;
  wndWc.lpszClassName = "Minimine";

  // register class
  if ( ! RegisterClass( &wndWc ) )
    return false;

  // get actual screen resolution
  int iSw = ( WORD )GetSystemMetrics( SM_CXSCREEN );
  int iSh = ( WORD )GetSystemMetrics( SM_CYSCREEN );

  // make a rectangle on the center of the screen
  RECT rc = { (iSw - 245)/2, (iSh - 144)/2, 245, 144 };

  // create the window
  hWnd = CreateWindow( "Minimine", "Minimine", WS_POPUP | WS_SYSMENU,
                       rc.left,rc.top, 245, 144, NULL, NULL, 
                       GetModuleHandle( NULL ), NULL);

  // return result
  return ( hWnd?true:false );
}


/* Builds the individual elements of the GUI.
 ******************************************************************************/
void BuildGUI()
{
  int i, j;

  // minimize button
  hMinButton = CreateWindow( "BUTTON", "M", WS_VISIBLE | WS_CHILD |
                               BS_OWNERDRAW, 227, 3, 7, 7, hWnd,
                               ( HMENU )IDB_MIN, hInstance, NULL );

  // close button
  hCloseButton = CreateWindow( "BUTTON", "C", WS_VISIBLE | WS_CHILD |
                                 BS_OWNERDRAW, 235, 3, 7, 7, hWnd,
                                 ( HMENU )IDB_CLOSE, hInstance, NULL );

  // new game button
  hNewButton = CreateWindow( "BUTTON", "N", WS_VISIBLE | WS_CHILD |
                               BS_OWNERDRAW, 131, 3, 7, 7, hWnd,
                               ( HMENU )IDB_NEW, hInstance, NULL );

  // number of mines left display
  for( i = 0; i < 2; i++ )
    hScore[ i ] = CreateWindow( "STATIC", "0", WS_VISIBLE | WS_CHILD |
                                SS_BITMAP, 117 - ( 5 * i ), 3, 7, 7, hWnd,
                                ( HMENU )( IDS_SCORE + i ), hInstance, NULL );

  // timer display
  for( i = 0; i < 3; i++ )
    hTime[ i ] = CreateWindow( "STATIC", "0", WS_VISIBLE | WS_CHILD |
                                SS_BITMAP, 155 - ( 5 * i ), 3, 7, 7, hWnd,
                                ( HMENU )( IDS_TIME + i ), hInstance, NULL );

  // subclassed minefield position buttons
  for( i = 0; i < COLS; i++ )
    for( j = 0; j < ROWS; j++ )
    {
      hField[ i ][ j ] = CreateWindow( "BUTTON", "x", WS_VISIBLE |
                                       WS_CHILD | BS_OWNERDRAW,
                                       ( 3 + 8 * i ), ( 14 + 8 * j ), 7, 7,
                                       hWnd,
                                       ( HMENU )( IDB_MINE + i + COLS * j ),
                                       hInstance, NULL );
      mainProc = ( WNDPROC )SetWindowLongPtr( hField[ i ][ j ], GWLP_WNDPROC,
                   PtrToLong( BtnSubclassProc ) );
    }

  SkinBmpa = LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_WINDOW ) );
  ButtonBmpa = LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_ELEMENTS ) );
}


/* Callback function for main window.
 ******************************************************************************/
LRESULT CALLBACK WinProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  LPDRAWITEMSTRUCT lpdis;
  HDC hdcMem;
  Position posDraw;
  RECT updaterect;
  switch( LOWORD( uMsg ) )
  {
  case WM_CREATE:
    h_minimized = 0;
    nid.cbSize = sizeof( NOTIFYICONDATA );
    nid.hWnd = hWnd;
    nid.uID = ID_TRAY;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER;
    nid.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON1 ) );
    Shell_NotifyIcon( NIM_ADD, &nid );
    return 0;
    break;
  case WM_DESTROY:
    Shell_NotifyIcon( NIM_DELETE, &nid );
    PostQuitMessage( WM_QUIT );
    break;
  case WM_COMMAND:
    switch( LOWORD( wParam ) )
    {
    case IDB_MIN:
      mtimer.Stop();
      SendMessage( hWnd, WM_SIZE, SIZE_MINIMIZED, 0 );
      break;
    case IDB_CLOSE:
      SendMessage( hWnd, WM_DESTROY, 0, 0 );
      break;
    case IDB_NEW:
      mtimer.Stop();
      mtimer.Reset();
      mfield.initMines();
      InvalidateRect( hWnd, NULL, true );
      UpdateWindow( hWnd );
      break;
    default:
      int btn = ( int )LOWORD( wParam );
      if( btn > IDB_MINE - 1 && btn < COLS * ROWS + IDB_MINE )
      {
        mtimer.Start();
        posClicked.setPos( btn - IDB_MINE );
        if( mfield.checkPos( posClicked ) > -2 )
          mfield.digPos( posClicked );
        if( mfield.checkPos( posClicked ) == 0 ||
            mfield.checkPos( posClicked ) == 9 )
        {
          InvalidateRect( hWnd, NULL, true );
          UpdateWindow( hWnd );
        }
        else
        {
          updaterect.left = ( 3 + 8 * posClicked.getx() );
          updaterect.top = ( 14 + 8 * posClicked.gety() );
          updaterect.right = updaterect.left + 7;
          updaterect.bottom = updaterect.top + 7;
          InvalidateRect( hWnd, &updaterect, true );
          UpdateWindow( hWnd );
        }
        if( mfield.Done() )
          mtimer.Stop();
      }
    }
    break;
  case WM_LBUTTONDOWN:
    SendMessage( hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
    break;
  case WM_PAINT:
    int i;
    hdcMem = CreateCompatibleDC( 0 );
    BeginPaint( hWnd, &ps );
    SelectObject( hdcMem, SkinBmpa );
    BitBlt( ps.hdc, 0, 0, 245, 144, hdcMem, 0, 0, SRCCOPY );
    EndPaint( hWnd, &ps );
    for( i = 0; i < 2; i++ )
    {
      BeginPaint( hScore[ i ], &ps );
      SelectObject( hdcMem, ButtonBmpa );
      BitBlt( ps.hdc, 0, 0, 7, 7, hdcMem,
              119 + 7 * ( mfield.numMines() / ( int )pow( 10, i ) % 10 ),
              0, SRCCOPY );
      EndPaint( hScore[ i ], &ps );
    }
    for( i = 0; i < 3; i++ )
    {
      BeginPaint( hTime[ i ], &ps );
      SelectObject( hdcMem, ButtonBmpa );
      BitBlt( ps.hdc, 0, 0, 7, 7, hdcMem,
              119 + 7 * ( mtimer.getTimer() / ( int )pow( 10, i ) % 10 ),
              0, SRCCOPY );
      EndPaint( hTime[ i ], &ps );
    }
    DeleteDC( hdcMem );
    break;
  case WM_DRAWITEM:
    lpdis = ( LPDRAWITEMSTRUCT )lParam; 
    hdcMem = CreateCompatibleDC( lpdis->hDC );
    SelectObject( hdcMem, ButtonBmpa );
    if( lpdis->itemState & ODS_SELECTED )
      BitBlt( lpdis->hDC, 0, 0, 7, 7, hdcMem, 189, 0, SRCCOPY );
    else if( LOWORD( wParam ) == IDB_MIN )
      BitBlt( lpdis->hDC, 0, 0, 7, 7, hdcMem, 0, 0, SRCCOPY );
    else if( LOWORD( wParam ) == IDB_CLOSE )
      BitBlt( lpdis->hDC, 0, 0, 7, 7, hdcMem, 7, 0, SRCCOPY );
    else if( LOWORD( wParam ) == IDB_NEW )
      BitBlt( lpdis->hDC, 0, 0, 7, 7, hdcMem, 14, 0, SRCCOPY );
    else if( LOWORD( wParam ) > IDB_MINE - 1 &&
             LOWORD( wParam) < IDB_MINE + ROWS * COLS )
    {
      posDraw.setPos( LOWORD( wParam ) - IDB_MINE );
      BitBlt( lpdis->hDC, 0, 0, 7, 7, hdcMem, 7 * FieldOffset( posDraw ), 0,
              SRCCOPY );
      EnableWindow( hField[ posDraw.getx() ][ posDraw.gety() ],
                    ( mfield.checkPos( posDraw ) < 0 ) &&
                    ( ! mfield.Done() ) );
    }
    DeleteDC( hdcMem );
    break;
  case WM_SIZE:
    switch( wParam )
    {
    case SIZE_RESTORED:
    case SIZE_MAXIMIZED:
      ShowWindow( hWnd, SW_SHOW );
      SetActiveWindow( hWnd );
      SetForegroundWindow( hWnd );
      return 0;
      break;
    case SIZE_MINIMIZED:
      ShowWindow( hWnd, SW_HIDE );
      h_minimized = 1;
      return 0;
      break;
    }
    break;
  case WM_USER:
    if( wParam == ID_TRAY && LOWORD( lParam ) == WM_LBUTTONUP && h_minimized )
    {
      ShowWindow( hWnd, SW_RESTORE );
      h_minimized = 0;
      if( mtimer.getTimer() && ( ! mfield.Done() ) )
        mtimer.Start();
    }
    break;
  case WM_USER + 1:
    updaterect.left = 145;
    updaterect.top = 3;
    updaterect.right = updaterect.left + 17;
    updaterect.bottom = updaterect.top + 7;
    InvalidateRect( hWnd, &updaterect, true );
    UpdateWindow( hWnd );
    break;
  default:
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
  }
  return 0;
}


/* Callback function for subclassed minefield position buttons.
 ******************************************************************************/
LRESULT CALLBACK BtnSubclassProc( HWND bWnd, UINT uMsg, WPARAM wParam,
                                  LPARAM lParam )
{
  RECT updaterect;
  if( uMsg == WM_RBUTTONUP )
  {
    posClicked.setPos( ( int )GetWindowLong( bWnd, GWL_ID ) - IDB_MINE );
    mfield.flagPos( posClicked );
    updaterect.left = ( 3 + 8 * posClicked.getx() );
    updaterect.top = ( 14 + 8 * posClicked.gety() );
    updaterect.right = updaterect.left + 7;
    updaterect.bottom = updaterect.top + 7;
    InvalidateRect( hWnd, &updaterect, true );
    updaterect.left = 112;
    updaterect.top = 3;
    updaterect.right = updaterect.left + 12;
    updaterect.bottom = updaterect.top + 7;
    InvalidateRect( hWnd, &updaterect, true );
    UpdateWindow( hWnd );
    if( mfield.Done() )
      mtimer.Stop();
    return 0;
  }
  return CallWindowProc( mainProc, bWnd, uMsg, wParam, lParam );
}
