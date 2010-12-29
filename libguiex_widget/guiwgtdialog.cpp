/** 
* @file guiwgtdialog.cpp
* @brief dialog used in guiex system
* @author ken
* @date 2006-08-16
*/



//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtdialog.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtDialog);
	//------------------------------------------------------------------------------
	CGUIWgtDialog::CGUIWgtDialog(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitDialog();
	}
	//------------------------------------------------------------------------------
	CGUIWgtDialog::CGUIWgtDialog( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitDialog();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtDialog::InitDialog()
	{
		m_pImageBG= NULL;

		SetMovable( true );
		SetOpenWithParent( false );
	}	
	//------------------------------------------------------------------------------
	void CGUIWgtDialog::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "BGIMAGE")
		{
			m_pImageBG = pImage;
			if( pImage && GetSize().IsEqualZero() )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtDialog::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageBG, GetBoundArea() );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtDialog::OnOpen( CGUIEventNotification* pEvent )
	{
		MoveToTop();
		return CGUIWidget::OnOpen(pEvent);
	}



	//------------------------------------------------------------------------------
}//namespace guiex
