#ifndef U_CHART_H
#define U_CHART_H

typedef enum tagMapMode
{

};

class UChart : public CStatic
{
    DECLARE_DYNAMIC(UChart)
public:
    UChart();
	~UChart() {};
protected:
    afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	BOOL attachPoints(POINT *pts, int num);
	void setRange(int x1, int y1, int x2, int y2);
private:
	void MapPoint(POINT *pt);
	void MapRect(RECT *rect);
	int m_x1;
	int m_x2;
	int m_y1;
	int m_y2;
};

#endif // U_CHART_H
