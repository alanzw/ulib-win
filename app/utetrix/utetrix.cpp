#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udc.h"

#include "utetrix.h"

UTetrix::UTetrix(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
    isFallingFinished = false;
    isStarted = false;
    isPaused = false;
    numLinesRemoved = 0;
    curX = 0;
    curY = 0;

    ClearBoard();
}

UTetrix::UTetrix()
{}

UTetrix::~UTetrix()
{}

UTetrix::UTetrix(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, nID)
{
    m_dwStyles &= ~SS_SIMPLE;
    m_dwStyles |= SS_NOTIFY;
    m_nResourceID = nID;

    isFallingFinished = false;
    isStarted = false;
    isPaused = false;
    numLinesRemoved = 0;
    curX = 0;
    curY = 0;

    ClearBoard();
}

BOOL UTetrix::create()
{
    BOOL bRet = UStatic::create();
    this->subclassProc();
    return  bRet;
}

BOOL UTetrix::onPaint()
{
    UPaintDC dc(*this);

    RECT rc;
    this->getClientRect(&rc);
    int boardTop = rc.bottom-rc.top - BoardHeight * SquareHeight();


    for (int i = 0; i < BoardHeight; ++i)
    {
        for (int j = 0; j < BoardWidth; ++j)
        {
            Tetrominoes shape = ShapeAt(j, BoardHeight - i - 1);
            if (shape != NoShape)
                DrawSquare(dc, 0 + j * SquareWidth(),
                           boardTop + i * SquareHeight(), shape);
        }
    }

    if (curPiece.GetShape() != NoShape)
    {
        for (int i = 0; i < 4; ++i)
        {
            int x = curX + curPiece.x(i);
            int y = curY - curPiece.y(i);
            DrawSquare(dc, 0 + x * SquareWidth(),
                       boardTop + (BoardHeight - y - 1) * SquareHeight(),
                       curPiece.GetShape());
        }
    }


    return FALSE;
}


void UTetrix::Start()
{
    if (isPaused)
    {
        return;
    }

    isStarted = true;
    isFallingFinished = false;
    numLinesRemoved = 0;

    ClearBoard();

    NewPiece();
    this->setTimer(ID_TIMER_UPDATE, 300);
}

void UTetrix::Pause()
{
    if (!isStarted)
        return;

    isPaused = !isPaused;
    if (isPaused)
    {
        this->killTimer(ID_TIMER_UPDATE);
    }
    else
    {
        this->setTimer(ID_TIMER_UPDATE, 300);
    }
    invalidate();
}

BOOL UTetrix::onKeyDown(WPARAM wParam, LPARAM lParam)
{
    if (!isStarted || curPiece.GetShape() == NoShape) {
        return FALSE;
    }

    int keycode = wParam;

    if (keycode == 'p' || keycode == 'P')
    {
        Pause();
        return FALSE;
    }

    if (isPaused)
        return FALSE;

    switch (keycode)
    {
    case VK_LEFT:
        TryMove(curPiece, curX - 1, curY);
        break;
    case VK_RIGHT:
        TryMove(curPiece, curX + 1, curY);
        break;
    case VK_DOWN:
        TryMove(curPiece.RotateRight(), curX, curY);
        break;
    case VK_UP:
        TryMove(curPiece.RotateLeft(), curX, curY);
        break;
    case VK_SPACE:
        DropDown();
        break;
    case 'd':
        OneLineDown();
        break;
    case 'D':
        OneLineDown();
        break;
    default:
        ;
    }

    return FALSE;

}

BOOL UTetrix::onTimer(WPARAM wParam, LPARAM lParam)
{
    if (isFallingFinished)
    {
        isFallingFinished = false;
        NewPiece();
    }
    else
    {
        OneLineDown();
    }
}


void UTetrix::ClearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)
        board[i] = NoShape;
}

void UTetrix::DropDown()
{
    int newY = curY;
    while (newY > 0)
    {
        if (!TryMove(curPiece, curX, newY - 1))
            break;
        --newY;
    }
    PieceDropped();
}

void UTetrix::OneLineDown()
{
    if (!TryMove(curPiece, curX, curY - 1))
        PieceDropped();
}

void UTetrix::PieceDropped()
{
    for (int i = 0; i < 4; ++i)
    {
        int x = curX + curPiece.x(i);
        int y = curY - curPiece.y(i);
        ShapeAt(x, y) = curPiece.GetShape();
    }

    RemoveFullLines();

    if (!isFallingFinished)
        NewPiece();
}

void UTetrix::RemoveFullLines()
{
     int numFullLines = 0;

     for (int i = BoardHeight - 1; i >= 0; --i)
     {
         bool lineIsFull = true;

         for (int j = 0; j < BoardWidth; ++j)
         {
             if (ShapeAt(j, i) == NoShape)
             {
                 lineIsFull = false;
                 break;
             }
         }

         if (lineIsFull) {
             ++numFullLines;
             for (int k = i; k < BoardHeight - 1; ++k)
             {
                 for (int j = 0; j < BoardWidth; ++j)
                     ShapeAt(j, k) = ShapeAt(j, k + 1);
             }
         }
     }

     if (numFullLines > 0) {
         numLinesRemoved += numFullLines;

         isFallingFinished = true;
         curPiece.SetShape(NoShape);
         invalidate();
     }
 }

void UTetrix::NewPiece()
{
    curPiece.SetRandomShape();
    curX = BoardWidth / 2 + 1;
    curY = BoardHeight - 1 + curPiece.MinY();

    if (!TryMove(curPiece, curX, curY))
    {
        curPiece.SetShape(NoShape);
        killTimer(ID_TIMER_UPDATE);
        isStarted = false;
    }
}

bool UTetrix::TryMove(const Shape& newPiece, int newX, int newY)
{
    for (int i = 0; i < 4; ++i)
    {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
            return false;
        if (ShapeAt(x, y) != NoShape)
            return false;
    }

    curPiece = newPiece;
    curX = newX;
    curY = newY;
    invalidate();
    return true;
}

void UTetrix::DrawSquare(UPaintDC & dc, int x, int y, Tetrominoes shape)
{
    static huys::Color colors[] = { RGB(0, 0, 0), RGB(204, 102, 102),
             RGB(102, 204, 102), RGB(102, 102, 204), RGB(204, 204, 102),
             RGB(204, 102, 204), RGB(102, 204, 204), RGB(218, 170, 0) };

    static huys::Color light[] = { RGB(0, 0, 0), RGB(248, 159, 171),
             RGB(121, 252, 121), RGB(121, 121, 252), RGB(252, 252, 121),
             RGB(252, 121, 252), RGB(121, 252, 252), RGB(252, 198, 0) };

    static huys::Color dark[] = { RGB(0, 0, 0), RGB(128, 59, 59), RGB(59, 128, 59),
             RGB(59, 59, 128), RGB(128, 128, 59), RGB(128, 59, 128),
             RGB(59, 128, 128), RGB(128, 98, 0) };

/*
    wxPen pen(light[int(shape)]);
    pen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(pen);

    dc.DrawLine(x, y + SquareHeight() - 1, x, y);
    dc.DrawLine(x, y, x + SquareWidth() - 1, y);

    wxPen darkpen(dark[int(shape)]);
    darkpen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(darkpen);
*/
    dc.setPenColor(light[int(shape)]);
    dc.drawLine(x, y + SquareHeight() - 1, x, y);
    dc.drawLine(x, y, x + SquareWidth() - 1, y);

    dc.setPenColor(huys::black);

    dc.drawLine(x + 1, y + SquareHeight() - 1,
        x + SquareWidth() - 1, y + SquareHeight() - 1);
    dc.drawLine(x + SquareWidth() - 1,
        y + SquareHeight() - 1, x + SquareWidth() - 1, y + 1);
/*
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(colors[int(shape)]));
    dc.DrawRectangle(x + 1, y + 1, SquareWidth() - 2,
        SquareHeight() - 2);
*/
    //dc.setBKMode(TRANSPARENT);
    dc.setBrushColor(colors[int(shape)]);
    dc.rectangle(x + 1, y + 1, SquareWidth() - 2,
        SquareHeight() - 2);
}
