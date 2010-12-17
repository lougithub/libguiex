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
#include <wx/wxprec.h>
#include <libguiex_core/guiex.h>

using namespace guiex;

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
	CWindowBox( );
	~CWindowBox( );

	void Reset();

	void SetWindow(CGUIWidget* pWindow);
	CGUIWidget* GetWindow();

	int GetPointAtPosition(wxPoint point);
	const wxRect& GetPointRect(int anIndex) ;
	const wxRect& GetAnchorRect();
	const wxRect& GetWindowRect();

	void Lock(bool lock = true);
	bool IsLocked() const;

	void MoveWindowPosition(int deltaX, int deltaY);
	void SetWindowSize(float deltaleft, float deltatop, float deltaright, float deltabottom);

private:
	CGUIWidget* m_pSelectWidget ;

	wxRect m_resizePoints [NUM_RESIZE_POINTS] ;
	wxRect m_aAnchorRect;
	wxRect m_aWindowsRect;

	bool m_locked;
};

#endif // __KEN_WINDOW_BOX_20070913_H_
