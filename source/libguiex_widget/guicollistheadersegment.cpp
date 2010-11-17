/** 
* @file guicollistheadersegment.cpp
* @brief segment of column's list header
* @author ken
* @date 2007-08-03
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guicollistheadersegment.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIColListHeaderSegment);
	//------------------------------------------------------------------------------
	CGUIString CGUIColListHeaderSegment::ms_strType = "CGUIColListHeaderSegment";
	//------------------------------------------------------------------------------
	CGUIColListHeaderSegment::CGUIColListHeaderSegment( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(ms_strType, rName, rSceneName)
	{
		InitColListHeaderSegment();
	}
	//------------------------------------------------------------------------------
	CGUIColListHeaderSegment::CGUIColListHeaderSegment( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitColListHeaderSegment();
	}
	//------------------------------------------------------------------------------
	CGUIColListHeaderSegment::~CGUIColListHeaderSegment(void)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIColListHeaderSegment::InitColListHeaderSegment()
	{
		m_nID = 0;
		m_eSortDir = eDir_None;
		m_bMovingEnabled = true;
		m_bAllowClicks = true;

		m_bHovering = false;
		m_bPushing = false;

		m_pImageNormal = NULL;
		m_pImageHovering = NULL;
		m_pImagePush = NULL;
	}
	//------------------------------------------------------------------------------
	int32 CGUIColListHeaderSegment::Create()
	{
		if( !m_pImageNormal )
		{
			throw CGUIException("[CGUIColListHeaderSegment::Create]: the image <BTN_NORMAL> hasn't been found!");
		}

		return CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void	CGUIColListHeaderSegment::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "COLUMN_HEADER_NORMAL")
		{
			m_pImageNormal = pImage;
		}
		else if( rName == "COLUMN_HEADER_HOVER")
		{
			m_pImageHovering = pImage;
		}
		else if( rName == "COLUMN_HEADER_PUSH")
		{
			m_pImagePush = pImage;
		}
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	void CGUIColListHeaderSegment::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIImage* pImage = NULL;

		if( m_bHovering && m_bPushing )
		{
			pImage = m_pImagePush?m_pImagePush:m_pImageNormal;
		}
		else if( m_bHovering )
		{
			pImage = m_pImageHovering?m_pImageHovering:m_pImageNormal; 
		}
		else
		{
			pImage = m_pImageNormal;
		}

		DrawImage( pRender, pImage, GetRect(),);
		DrawString(pRender, GetText(), GetClientRect(), GetTextAlignment());
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	uint32		CGUIColListHeaderSegment::OnMouseEnter( CGUIEventMouse* pEvent )
	{
		m_bHovering = true;

		return CGUIWidget::OnMouseEnter( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListHeaderSegment::OnMouseLeave( CGUIEventMouse* pEvent )
	{
		m_bHovering = false;

		return CGUIWidget::OnMouseLeave( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListHeaderSegment::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		m_bPushing = true;

		return CGUIWidget::OnMouseLeftDown( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListHeaderSegment::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		m_bPushing = false;

		return CGUIWidget::OnMouseLeftUp( pEvent );
	}
	//------------------------------------------------------------------------------


}//namespace guiex


