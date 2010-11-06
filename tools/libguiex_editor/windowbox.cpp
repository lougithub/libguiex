/** 
* @file windowbox.h
* @brief windows box
* @author ken
* @date 2007-09-12
*/

//============================================================================//
// include
//============================================================================// 
#include "libguiex_editor.h"


//-----------------------------------------------------------------------
CWindowBox::CWindowBox ( )
{
	m_pSelectWidget = NULL;
}
//-----------------------------------------------------------------------
CWindowBox::~CWindowBox( )
{
	
}
//-----------------------------------------------------------------------
void CWindowBox::Reset () 
{
	if( !m_pSelectWidget )
	{
		return;
	}

	m_locked = false;
	// Find the amount to convert from relative- to screen coordinates
	// Do this by finding the parent of the element (until no parent)
	const guiex::CGUIRect& winRc = m_pSelectWidget->GetBoundArea();

	// Make boxes 3x3 and always in pixels
	//int width = 7;
	//int height = 7;
	int paddX = 3;
	int paddY = 3;

	// Position the "TopLeft" resizer
	m_resizePoints [RESIZE_POINT_WN].x = winRc.m_fLeft - paddX;
	m_resizePoints [RESIZE_POINT_WN].y = winRc.m_fTop - paddY;
	// Position the "Top" resizer
	m_resizePoints [RESIZE_POINT_N].x = winRc.m_fLeft + winRc.GetWidth() / 2 - paddX;
	m_resizePoints [RESIZE_POINT_N].y = winRc.m_fTop - paddY;
	// Position the "TopRight" resizer
	m_resizePoints [RESIZE_POINT_NE].x = winRc.m_fLeft + winRc.GetWidth() - paddX;
	m_resizePoints [RESIZE_POINT_NE].y = winRc.m_fTop - paddY;
	// Position the "Right" resizer
	m_resizePoints [RESIZE_POINT_E].x = winRc.m_fLeft + winRc.GetWidth() - paddX;
	m_resizePoints [RESIZE_POINT_E].y = winRc.m_fTop + winRc.GetHeight() / 2 - paddY;
	// Position the "BottomRight" resizer
	m_resizePoints [RESIZE_POINT_ES].x = winRc.m_fLeft + winRc.GetWidth() - paddX;
	m_resizePoints [RESIZE_POINT_ES].y = winRc.m_fTop + winRc.GetHeight() - paddY;
	// Position the "Bottom" resizer
	m_resizePoints [RESIZE_POINT_S].x = winRc.m_fLeft + winRc.GetWidth() / 2 - paddX;
	m_resizePoints [RESIZE_POINT_S].y = winRc.m_fTop + winRc.GetHeight() - paddY;
	// Position the "BottomLeft" resizer
	m_resizePoints [RESIZE_POINT_SW].x = winRc.m_fLeft - paddX;
	m_resizePoints [RESIZE_POINT_SW].y = winRc.m_fTop + winRc.GetHeight() - paddY;
	// Position the "Left" resizer
	m_resizePoints [RESIZE_POINT_W].x = winRc.m_fLeft - paddX;
	m_resizePoints [RESIZE_POINT_W].y = winRc.m_fTop + winRc.GetHeight() / 2 - paddY;

	for (int i = 0; i < NUM_RESIZE_POINTS; ++i) 
	{
		m_resizePoints [i].width = paddX * 2;
		m_resizePoints [i].height = paddY * 2;
	}

	paddX -=1;
	paddY -=1;
	const guiex::CGUIVector2& rPoint = m_pSelectWidget->GetAnchorPoint();
	m_aAnchorRect.x = winRc.m_fLeft + winRc.GetWidth()* rPoint.x-paddX;
	m_aAnchorRect.y = winRc.m_fTop + winRc.GetHeight()* rPoint.y-paddY;
	m_aAnchorRect.width = paddX * 2;
	m_aAnchorRect.height = paddY * 2;

	const guiex::CGUIRect& rClientRect = m_pSelectWidget->GetClientArea();
	m_aClientRect.x = rClientRect.m_fLeft;
	m_aClientRect.y = rClientRect.m_fTop;
	m_aClientRect.width = rClientRect.GetWidth();
	m_aClientRect.height = rClientRect.GetHeight();
}
//-----------------------------------------------------------------------
void	CWindowBox::MoveWindowPosition(int deltaX, int deltaY)
{
	guiex::CGUIVector2 aPoint = m_pSelectWidget->GetPixelPosition();
	aPoint.x += deltaX;
	aPoint.y += deltaY;
	m_pSelectWidget->SetPixelPosition(aPoint);

	Reset();
}
//-----------------------------------------------------------------------
void	CWindowBox::SetWindowSize(float deltaleft, float deltatop, float deltaright, float deltabottom)
{
	//TODO: do it later
//	guiex::CGUIRect aRect = m_pSelectWidget->ExpandBoundArea( deltaleft, deltatop, deltaright, deltabottom );
	Reset();
}
//-----------------------------------------------------------------------
void CWindowBox::SetWindow(guiex::CGUIWidget* pWindow)
{
	m_pSelectWidget = pWindow;
	if( m_pSelectWidget )
	{
		//m_pSelectWidget->MoveToTop();

		Reset();
	}
}
//-----------------------------------------------------------------------
const wxRect& CWindowBox::GetPointRect (int anIndex) 
{
	return m_resizePoints [anIndex];
}
//-----------------------------------------------------------------------
const wxRect & CWindowBox::GetAnchorRect()
{
	return m_aAnchorRect;
}
const wxRect & CWindowBox::GetClientRect()
{
	return m_aClientRect;
}
//-----------------------------------------------------------------------
wxRect CWindowBox::GetWindowRect () 
{
	const guiex::CGUIRect& rc = m_pSelectWidget->GetBoundArea();
	guiex::CGUIVector2 aLeftTop(rc.m_fLeft, rc.m_fTop);
	guiex::CGUIVector2 aBottomRight(rc.m_fRight, rc.m_fBottom );
	m_pSelectWidget->LocalToWorld( aLeftTop );
	m_pSelectWidget->LocalToWorld( aBottomRight );
	return wxRect (wxPoint((int)aLeftTop.x, (int)aLeftTop.y), 
		wxPoint((int)aBottomRight.x, (int)aBottomRight.y));
}

//-----------------------------------------------------------------------
int CWindowBox::GetPointAtPosition (wxPoint aPoint) 
{
	// If the window is locked, we just cheat the Document
	if (m_locked)
	{
		return RESIZE_POINT_NONE;
	}
	for (int i = 0; i < NUM_RESIZE_POINTS; ++i) 
	{
		if (m_resizePoints [i].Contains (aPoint))
		{
			return i ;
		}    
	}
	return RESIZE_POINT_NONE;
}

//-----------------------------------------------------------------------
void CWindowBox::SetNewWindowArea(wxRect newArea)
{
	//TODO: do it later
	//m_pSelectWidget->SetRect(guiex::CGUIRect(newArea.x, newArea.y, newArea.x+newArea.width, newArea.y+newArea.height));

	// Update the resizer positions
	Reset();
}

//-----------------------------------------------------------------------
void CWindowBox::SetNewWindowPosition(wxPoint newPosition)
{
	//TODO: do it later
	//	m_pSelectWidget->SetGlobalPosition(guiex::CGUIVector2(newPosition.x, newPosition.y));

	// Update the resizer positions
	Reset();
}
//-----------------------------------------------------------------------
