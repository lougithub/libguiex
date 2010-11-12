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

	//==========================================================
	//update windows rect
	const guiex::CGUIRect& rc = m_pSelectWidget->GetBoundArea();
	guiex::CGUIRenderRect aRenderRect;
	m_pSelectWidget->LocalToWorld( rc, aRenderRect );
	float fLeft = aRenderRect.m_vecVertex[0].m_aVector.x;
	float fRight = aRenderRect.m_vecVertex[0].m_aVector.x;
	float fTop = aRenderRect.m_vecVertex[0].m_aVector.y;
	float fBottom = aRenderRect.m_vecVertex[0].m_aVector.y;
	for( int i=0; i<4; ++i )
	{
		fLeft = std::min( aRenderRect.m_vecVertex[i].m_aVector.x, fLeft );
		fRight = std::max( aRenderRect.m_vecVertex[i].m_aVector.x, fRight );
		fTop = std::min( aRenderRect.m_vecVertex[i].m_aVector.y, fTop );
		fBottom = std::max( aRenderRect.m_vecVertex[i].m_aVector.y, fBottom );
	}
	m_aWindowsRect = wxRect(fLeft, fTop, fRight-fLeft, fBottom-fTop);

	//==========================================================
	//update resizer 
	m_locked = false;
	// Find the amount to convert from relative- to screen coordinates
	// Do this by finding the parent of the element (until no parent)
	guiex::CGUIRect boundRect( m_aWindowsRect.GetLeft(), 
		m_aWindowsRect.GetTop(), 
		m_aWindowsRect.GetRight(),
		m_aWindowsRect.GetBottom());

	// Make boxes 3x3 and always in pixels
	int paddX = 3;
	int paddY = 3;

	// Position the "TopLeft" resizer
	m_resizePoints [RESIZE_POINT_WN].x = boundRect.m_fLeft - paddX;
	m_resizePoints [RESIZE_POINT_WN].y = boundRect.m_fTop - paddY;
	// Position the "Top" resizer
	m_resizePoints [RESIZE_POINT_N].x = boundRect.m_fLeft + boundRect.GetWidth() / 2 - paddX;
	m_resizePoints [RESIZE_POINT_N].y = boundRect.m_fTop - paddY;
	// Position the "TopRight" resizer
	m_resizePoints [RESIZE_POINT_NE].x = boundRect.m_fLeft + boundRect.GetWidth() - paddX;
	m_resizePoints [RESIZE_POINT_NE].y = boundRect.m_fTop - paddY;
	// Position the "Right" resizer
	m_resizePoints [RESIZE_POINT_E].x = boundRect.m_fLeft + boundRect.GetWidth() - paddX;
	m_resizePoints [RESIZE_POINT_E].y = boundRect.m_fTop + boundRect.GetHeight() / 2 - paddY;
	// Position the "BottomRight" resizer
	m_resizePoints [RESIZE_POINT_ES].x = boundRect.m_fLeft + boundRect.GetWidth() - paddX;
	m_resizePoints [RESIZE_POINT_ES].y = boundRect.m_fTop + boundRect.GetHeight() - paddY;
	// Position the "Bottom" resizer
	m_resizePoints [RESIZE_POINT_S].x = boundRect.m_fLeft + boundRect.GetWidth() / 2 - paddX;
	m_resizePoints [RESIZE_POINT_S].y = boundRect.m_fTop + boundRect.GetHeight() - paddY;
	// Position the "BottomLeft" resizer
	m_resizePoints [RESIZE_POINT_SW].x = boundRect.m_fLeft - paddX;
	m_resizePoints [RESIZE_POINT_SW].y = boundRect.m_fTop + boundRect.GetHeight() - paddY;
	// Position the "Left" resizer
	m_resizePoints [RESIZE_POINT_W].x = boundRect.m_fLeft - paddX;
	m_resizePoints [RESIZE_POINT_W].y = boundRect.m_fTop + boundRect.GetHeight() / 2 - paddY;

	for (int i = 0; i < NUM_RESIZE_POINTS; ++i) 
	{
		m_resizePoints [i].width = paddX * 2;
		m_resizePoints [i].height = paddY * 2;
	}

	//==========================================================
	//update windows rect
	paddX = 2;
	paddY = 2;
	guiex::CGUIVector2 rPoint = m_pSelectWidget->GetAnchorPoint();
	const guiex::CGUIRect& rBoundRect = m_pSelectWidget->GetBoundArea();
	rPoint.x = rBoundRect.GetPosition().x + rBoundRect.GetWidth() * rPoint.x;
	rPoint.x = rBoundRect.GetPosition().y + rBoundRect.GetHeight() * rPoint.y;
	m_pSelectWidget->LocalToWorld( rPoint );
	m_aAnchorRect.x = rPoint.x-paddX;
	m_aAnchorRect.y = rPoint.y-paddY;
	m_aAnchorRect.width = paddX * 2;
	m_aAnchorRect.height = paddY * 2;
}
//-----------------------------------------------------------------------
void	CWindowBox::MoveWindowPosition(int deltaX, int deltaY)
{
	guiex::CGUIVector2 aLocalOldPoint = m_pSelectWidget->GetPixelPosition();
	guiex::CGUIVector3 aWorldOldPoint( aLocalOldPoint.x, aLocalOldPoint.y, 0.0f );
	m_pSelectWidget->GetParent()->LocalToWorld(aWorldOldPoint);

	guiex::CGUIVector3 aTestPoint = aWorldOldPoint;
	m_pSelectWidget->GetParent()->WorldToLocal(aTestPoint);

	guiex::CGUIVector3 aWorldNewPoint = aWorldOldPoint + guiex::CGUIVector3(deltaX, deltaY, 0.0f);
	guiex::CGUIVector3 aLocalNewPoint = aWorldNewPoint;
	m_pSelectWidget->GetParent()->WorldToLocal( aLocalNewPoint );
	m_pSelectWidget->SetPixelPosition(aLocalNewPoint.x, aLocalNewPoint.y);
	m_pSelectWidget->Refresh();
	Reset();
}
//-----------------------------------------------------------------------
void	CWindowBox::SetWindowSize(float deltaleft, float deltatop, float deltaright, float deltabottom)
{
	guiex::CGUIRect aBoundRect = m_pSelectWidget->GetBoundArea();
	aBoundRect.m_fLeft += deltaleft;
	aBoundRect.m_fTop += deltatop;
	aBoundRect.m_fRight += deltaright;
	aBoundRect.m_fBottom += deltabottom;
	m_pSelectWidget->SetPixelSize(aBoundRect.GetSize());
	m_pSelectWidget->Refresh();
	Reset();
}
//-----------------------------------------------------------------------
void CWindowBox::SetWindow(guiex::CGUIWidget* pWindow)
{
	m_pSelectWidget = pWindow;
	if( m_pSelectWidget )
	{
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
//-----------------------------------------------------------------------
const wxRect & CWindowBox::GetWindowRect () 
{
	return m_aWindowsRect;
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
