#ifndef HU_GDI_H
#define HU_GDI_H

namespace huys
{

class UShape;
class ULine;
class UPolyLine;
class UAgngleArc;
class URectangle;

class UMarker;

class UShape : public CObject
{
    //DECLARE_DYNAMIC(UShape)
	//DECLARE_SERIAL(UShape)
public:
    UShape();
    virtual ~UShape();
    virtual void Draw(CDC *pDC)=0;
    virtual void Serialize(CArchive &ar);
    //
	virtual void setLineColor(COLORREF color);
	virtual void setLneWidth(int width);
	virtual void setLineStyle(UINT style);
	virtual void setFilledColor(COLORREF color);
    virtual void setFilledHatch(ULONG hatch);
	virtual void setFilledStyle(UINT style); 
protected:
	LOGPEN LinePen;
	LOGBRUSH FilledBrush;

    bool bIsSelected;
private:    
};

class ULine : public UShape
{
    //
    DECLARE_SERIAL(ULine)
public:
    ULine(int x=0, int y=0, int x2=0, int y2=0);
    ULine(POINT start, POINT end);
    virtual ~ULine();    
    virtual void Serialize(CArchive &ar);
    virtual void Draw(CDC *pDC);
    //
    void SetStartPnt(POINT start);
    void SetStartPnt(int x, int y);
    void SetEndPnt(POINT end);
    void SetEndPnt(int x, int y);
    void offsetX(int x);
    void offsetY(int y);
private:
    int m_nStartX;
    int m_nStartY;
    int m_nEndX;
    int m_nEndY;
};

class UPolyLine : public UShape
{
    //
    DECLARE_SERIAL(UPolyLine)

public:
    UPolyLine(POINT *pts=NULL, int num=0);
    virtual ~UPolyLine();
    virtual void Serialize(CArchive &ar);
    virtual void Draw(CDC *pDC);
    BOOL setPoints(POINT *pts, int num);
protected:
private:
    POINT *m_pts;
    int m_num;
};

class URectangle : public UShape
{
	//
	DECLARE_SERIAL(URectangle)		
public:
	URectangle(int x=0, int y=0, int x2=0, int y2=0);
	URectangle(POINT start, POINT end);
    URectangle(RECT rc);
	virtual ~URectangle();    
	virtual void Serialize(CArchive &ar);
	virtual void Draw(CDC *pDC);
	//
	void SetStartPnt(POINT start);
	void SetStartPnt(int x, int y);
	void SetEndPnt(POINT end);
	void SetEndPnt(int x, int y);

    void moveCenterTo(int x, int y);
	
private:
	int m_nStartX;
	int m_nStartY;
	int m_nEndX;
	int m_nEndY;
};

class UAgngleArc : public UShape
{
	DECLARE_SERIAL(UAgngleArc)
public:
	UAgngleArc(int centerX=0, int centerY=0, DWORD radius=1, 
		FLOAT startAngle=0.0f, FLOAT sweepAngle=10.0f);
	UAgngleArc(POINT center, DWORD radius=1,
		FLOAT startAngle=0.0f, FLOAT sweepAngle=10.0f);
	virtual ~UAgngleArc();
	virtual void Serialize(CArchive &ar);
	virtual void Draw(CDC *pDC);

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

class UMarker : public UShape
{
    DECLARE_SERIAL(UMarker)
public:
    UMarker(int left=0, int top=0, int right=10, int bottom=10);
    UMarker(POINT leftTop, POINT rightBottom);
    virtual ~UMarker();
    virtual void Serialize(CArchive &ar);
	virtual void Draw(CDC *pDC);
protected:
private:
    int m_nLeft;
    int m_nTop;
    int m_nRight;
    int m_nBottom;
};


}

#endif // HU_GDI_H
