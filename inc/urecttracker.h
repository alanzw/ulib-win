#ifndef U_RECT_TRACKER_H
#define U_RECT_TRACKER_H

#include "ulib.h"

class ULIB_API URectTracker
{
public:
	// Style Flags
	enum StyleFlags
	{
		solidLine = 1, dottedLine = 2, hatchedBorder = 4,
		resizeInside = 8, resizeOutside = 16, hatchInside = 32,
	};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
	};
public:
    URectTracker();
	URectTracker(LPRECT lpRect, UINT nStyle);
    ~URectTracker();
private:
	RECT m_rect;
	UINT m_nStyle;
	int m_nHandleSize;
};

#endif // U_RECT_TRACKER_H
