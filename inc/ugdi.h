#ifndef U_GDI_H
#define U_GDI_H

#include "ulib.h"

namespace huys
{

/************************************************************************/
/* brush style                                                          */
/*   BS_DIBPATTERN                                                      */
/*   BS_DIBPATTERN8X8                                                   */
/*   BS_DIBPATTERNPT                                                    */
/*   BS_HATCHED                                                         */
/*   BS_HOLLOW                                                          */
/*   BS_NULL                                                            */
/*   BS_PATTERN                                                         */
/*   BS_PATTERN8X8                                                      */
/*   BS_SOLID                                                           */
/* default:                                                             */
/*   BS_SOLID                                                           */
/************************************************************************/

class ULIB_API UShape
{
public:
    UShape();
    virtual ~UShape();
    virtual void Draw(HDC hdc)=0;
    //
    virtual void setLineColor(COLORREF color);
    virtual void setLineWidth(int width);
    virtual void setLineStyle(UINT style);
    virtual void setFilledColor(COLORREF color);
    virtual void setFilledHatch(ULONG hatch);
    virtual void setFilledStyle(UINT style);
protected:
    LOGPEN LinePen;
    LOGBRUSH FilledBrush;
    //
    bool bIsSelected;
private:
};

class ULIB_API ULine : public UShape
{
public:
    ULine(int x=0, int y=0, int x2=0, int y2=0);
    ULine(POINT start, POINT end);
    virtual ~ULine();
    virtual void Draw(HDC hdc);
    //
    void SetStartPnt(POINT start);
    void SetStartPnt(int x, int y);
    void SetEndPnt(POINT end);
    void SetEndPnt(int x, int y);
    void offsetX(int x);
    void offsetY(int y);

    int getStartX() {return m_nStartX;}
    int getStartY() {return m_nStartY;}
    int getEndX() {return m_nEndX;}
    int getEndY() {return m_nEndY;}
private:
    int m_nStartX;
    int m_nStartY;
    int m_nEndX;
    int m_nEndY;
};

class ULIB_API UPolyLine : public UShape
{
public:
    UPolyLine(POINT *pts=NULL, int num=0);
    virtual ~UPolyLine();
    virtual void Draw(HDC hdc);
    BOOL setPoints(POINT *pts, int num);
protected:
private:
    POINT *m_pts;
    int m_num;
};

class ULIB_API URectangle : public UShape
{
public:
    URectangle(int x=0, int y=0, int x2=0, int y2=0);
    URectangle(POINT start, POINT end);
    URectangle(const RECT &rc);
    virtual ~URectangle();
    virtual void Draw(HDC hdc);
    //
    void SetStartPnt(POINT start);
    void SetStartPnt(int x, int y);
    void SetEndPnt(POINT end);
    void SetEndPnt(int x, int y);
    //
    void SetRect(const RECT &rc);

    void moveCenterTo(int x, int y);

    void shrinkTo(int nWidth, int nHeight);

    void offset(int nX, int nY = 0);

    int getStartX() {return m_nStartX;}
    int getStartY() {return m_nStartY;}
    int getEndX() {return m_nEndX;}
    int getEndY() {return m_nEndY;}
private:
    int m_nStartX;
    int m_nStartY;
    int m_nEndX;
    int m_nEndY;
};

class ULIB_API UAngleArc : public UShape
{
public:
    UAngleArc(int centerX=0, int centerY=0, DWORD radius=1,
        FLOAT startAngle=0.0f, FLOAT sweepAngle=10.0f);
    UAngleArc(POINT center, DWORD radius=1,
        FLOAT startAngle=0.0f, FLOAT sweepAngle=10.0f);
    virtual ~UAngleArc();
    virtual void Draw(HDC hdc);

    void setCenter(int x, int y);
    void setRadius(DWORD radius);
    void setStartAngle(FLOAT angle);
    void setSweepAngle(FLOAT angle);
protected:
private:
    int m_nCenterX;
    int m_nCenterY;
    DWORD m_dwRadius;
    FLOAT m_fStartAngle;
    FLOAT m_fSweepAngle;
};

class ULIB_API UTriangle : public UShape
{
public:
    UTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    UTriangle(const LPPOINT p1, const LPPOINT p2, const LPPOINT p3);
    virtual void Draw(HDC hdc);
private:
    UPointI m_upts[3];
};

class ULIB_API UMarker : public UShape
{
public:
    UMarker(int left=0, int top=0, int right=10, int bottom=10);
    UMarker(POINT leftTop, POINT rightBottom);
    virtual ~UMarker();
    virtual void Draw(HDC hdc);
protected:
private:
    int m_nLeft;
    int m_nTop;
    int m_nRight;
    int m_nBottom;
};

class ULIB_API UCircle : public UShape
{
public:
    UCircle(int centerX=0, int centerY=0, DWORD radius=1);
    virtual void Draw(HDC hdc);
    void setCenter(int x, int y);
    void setRadius(DWORD radius);
private:
    int m_nCenterX;
    int m_nCenterY;
    DWORD m_dwRadius;
};

class ULIB_API UPolygon : public UShape
{
public:
	UPolygon(POINT *pts=NULL, int num=0);
	virtual ~UPolygon();
	virtual void Draw(HDC hdc);
	void setPoints(POINT *pts, int num);
private:
	POINT *m_pts;
	int m_num;
};

} // namespace huys

#endif // U_GDI_H
