#ifndef U_TETRIX_H
#define U_TETRIX_H

#include "ustatic.h"

enum Tetrominoes {
    NoShape,
    ZShape,
    SShape,
    LineShape, 
    TShape,
    SquareShape,
    LShape,
    MirroredLShape
};

class Shape
{
public:
    Shape() { SetShape(NoShape); }
    void SetShape(Tetrominoes shape);
    void SetRandomShape();

    Tetrominoes GetShape() const { return pieceShape; }
    int x(int index) const { return coords[index][0]; }
    int y(int index) const { return coords[index][1]; }

    int MinX() const;
    int MaxX() const;
    int MinY() const;
    int MaxY() const;

    Shape RotateLeft() const;
    Shape RotateRight() const;

private:
    void SetX(int index, int x) { coords[index][0] = x; }
    void SetY(int index, int y) { coords[index][1] = y; }
    Tetrominoes pieceShape;
    int coords[4][2];
};


class UTetrix : public UStatic
{
    enum {
        ID_TIMER_UPDATE = 1111
    };
public:
    UTetrix(HWND hParent, UINT nID, HINSTANCE hInst);
    UTetrix();
    ~UTetrix();
    UTetrix(UBaseWindow *pWndParent, UINT nID = -1);
    
    //
    BOOL create();

    //
    BOOL onPaint();
    
    void Start();
    void Pause();
    void linesRemovedChanged(int numLines);

    BOOL onKeyDown(WPARAM wParam, LPARAM lParam);
    
    BOOL onTimer(WPARAM wParam, LPARAM lParam);
    
private:
    enum { BoardWidth = 10, BoardHeight = 22 };
    Tetrominoes & ShapeAt(int x, int y) { return board[(y * BoardWidth) + x]; }

    int SquareWidth() { RECT rc; this->getClientRect(&rc);return (rc.right-rc.left) / BoardWidth; }
    int SquareHeight() { RECT rc; this->getClientRect(&rc);return (rc.bottom-rc.top)/ BoardHeight; }
    void ClearBoard();
    void DropDown();
    void OneLineDown();
    void PieceDropped();
    void RemoveFullLines();
    void NewPiece();
    bool TryMove(const Shape& newPiece, int newX, int newY);
    void DrawSquare(class UPaintDC &dc, int x, int y, Tetrominoes shape);

    bool isStarted;
    bool isPaused;
    bool isFallingFinished;
    Shape curPiece;
    int curX;
    int curY;
    int numLinesRemoved;
    Tetrominoes board[BoardWidth * BoardHeight];

};

#endif // U_TETRIX_H
