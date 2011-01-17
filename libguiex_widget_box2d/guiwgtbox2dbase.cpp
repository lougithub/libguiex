/** 
* @file guiwgtbox2dbase.cpp
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtbox2dbase.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guisystem.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIWgtBox2DBase::CGUIWgtBox2DBase( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitBox2DBase();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBase::InitBox2DBase()
	{
		m_pImageBG = NULL;
		SetFocusable(false);
		SetActivable(false);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBase::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "BGIMAGE")
		{
			m_pImageBG = pImage;
			if( GetSize().IsEqualZero() && m_pImageBG )
			{
				SetPixelSize(m_pImageBG->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBase::RenderSelf(IGUIInterfaceRender* pRender)
	{
		if( m_pImageBG )
		{
			DrawImage( pRender, m_pImageBG, GetBoundArea( ));
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBase::OnCreate()
	{
		CGUIWidget::OnCreate();

		if( !GSystem->IsEditorMode() )
		{
			InitializeBox2D();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBase::OnUpdate(real fDeltaTime)
	{
		CGUIWidget::OnUpdate(fDeltaTime );

		if( !GSystem->IsEditorMode() )
		{
			UpdateBox2D();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBase::OnDestroy()
	{
		if( !GSystem->IsEditorMode() )
		{
			ReleaseBox2D();
		}

		CGUIWidget::OnDestroy();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBase::InitializeBox2D()
	{

	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBase::ReleaseBox2D()
	{

	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBase::UpdateBox2D()
	{

	}
	//------------------------------------------------------------------------------

}//namespace guiex

