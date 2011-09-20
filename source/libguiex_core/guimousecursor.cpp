/** 
* @file guimousecursor.cpp
* @brief mouse cursor, used to draw mouse
* @author ken
* @date 2006-07-18
*/


//============================================================================//
// include 
//============================================================================// 
#include "guimousecursor.h"
#include "guiinterfacerender.h"
#include "guiexception.h"
#include "guiimagemanager.h"
#include "guiimage.h"
#include "guianimation.h"
#include "guisystem.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUIEXPORT CGUIMouseCursor* GetMouseCursor()
	{
		return CGUIMouseCursor::Instance();
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	CGUIMouseCursor * CGUIMouseCursor::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIMouseCursor::CGUIMouseCursor()
		:m_pAnimation(NULL)
		,m_bVisible(false)
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIMouseCursor::CGUIMouseCursor]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIMouseCursor::~CGUIMouseCursor()
	{
		m_pSingleton = NULL; 
	}	
	//------------------------------------------------------------------------------
	CGUIMouseCursor* CGUIMouseCursor::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	void CGUIMouseCursor::SetVisible( bool bVisible)
	{
		m_bVisible = bVisible;
	}
	//------------------------------------------------------------------------------
	bool CGUIMouseCursor::IsVisible( ) const
	{
		return m_bVisible;
	}
	//------------------------------------------------------------------------------
	void CGUIMouseCursor::Render(IGUIInterfaceRender* pRender)
	{

	}
	//------------------------------------------------------------------------------
	void CGUIMouseCursor::SetCursor(const CGUIString& rCursorName)
	{
		if( rCursorName == m_strCurrentCursor)
		{
			return;
		}

		m_strCurrentCursor = rCursorName;
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIMouseCursor::GetCursor() const
	{
		return m_strCurrentCursor;
	}
	//------------------------------------------------------------------------------
	void CGUIMouseCursor::SetPosition(const CGUIVector2& rPoint)
	{
		m_aMousePoint = rPoint;
	}
	//------------------------------------------------------------------------------

}//namespace guiex

