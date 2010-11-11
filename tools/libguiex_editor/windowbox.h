/** 
* @file windowbox.h
* @brief windows box
* @author ken
* @date 2007-09-12
*/


#ifndef __KEN_WINDOW_BOX_20070913_H_
#define __KEN_WINDOW_BOX_20070913_H_

//============================================================================//
// include
//============================================================================// 



//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWidget;
}

/** The resize box exists of 8 points.*/
#define NUM_RESIZE_POINTS 8

/** Locations of the points.*/
#define RESIZE_POINT_WN 0   // West-North
#define RESIZE_POINT_N  1   // North
#define RESIZE_POINT_NE 2   // North-East
#define RESIZE_POINT_E  3   // East
#define RESIZE_POINT_ES 4   // East-South
#define RESIZE_POINT_S  5   // South
#define RESIZE_POINT_SW 6   // South-West
#define RESIZE_POINT_W  7   // West

/** Value indicating that the mouse is not hovering on a resize point.*/
#define RESIZE_POINT_NONE -1


//============================================================================//
// class
//============================================================================// 

class CWindowBox
{
public:
	/** Constructor. Visually creates an eight-points box around the given element.*/
	CWindowBox( );
	~CWindowBox( );

	void SetWindow(guiex::CGUIWidget* pWindow);
	

	/** Re-positions the resize-points. Used when the resizer is initialized or when the 
	* contained GuiElement has been resized.*/
	void Reset();

	/** Returns the window to which this resizer is attached.*/
	guiex::CGUIWidget* GetWindow()
	{
		return m_pSelectWidget;
	}

	/** Checks if the mouse is within a resize point. If so, it returns the point's index (0-7).*/
	int GetPointAtPosition(wxPoint point);

	/** Returns a GDI rectangle for the given point.
	*
	* @param iIndex the point (0-7).
	* @return absolute GDI rectangle.
	*/
	const wxRect & GetPointRect(int anIndex) ;

	const wxRect & GetAnchorRect();

	/** Returns a GDI rectangle for the boxed Window.
	*
	* @return absolute GDI rectangle.
	*/
	const wxRect &  GetWindowRect();

	/** Locks or unlocks the boxed window from allowing its properties to be changed.*/
	void Lock(bool lock = true)
	{
		m_locked = lock;
	}

	/** Returns the lock state of the boxed window.*/
	bool IsLocked()
	{
		return m_locked;
	}

	//change window's position
	void	MoveWindowPosition(int deltaX, int deltaY);

	//change window's size
	void	SetWindowSize(float deltaleft, float deltatop, float deltaright, float deltabottom);

private:

	/** The windows around which this resize box resides.*/
	guiex::CGUIWidget*    m_pSelectWidget ;

	/** 8 rectangles with absolute and unclipped positions and sizes per point.*/
	wxRect      m_resizePoints [NUM_RESIZE_POINTS] ;
	wxRect		m_aAnchorRect;
	wxRect		m_aWindowsRect;

	/** Holds whether the boxed window is locked. When true, its position and size can not be changed.
	* in the future, each window should be wrapped in this box so that locking information can also be remembered,
	* when a window is not selected anymore. Then add methods for "setSelected" and such.*/
	bool			m_locked;
};

#endif // __KEN_WINDOW_BOX_20070913_H_
