/** 
* @file guiwgtdialog.cpp
* @brief dialog used in guiex system
* @author ken
* @date 2006-08-16
*/



//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtdialog.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtDialog);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtDialog::ms_strType = "CGUIWgtDialog";
	//------------------------------------------------------------------------------
	CGUIWgtDialog::CGUIWgtDialog(const CGUIString& rName, const CGUIString& rProjectName)
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitDialog();
	}
	//------------------------------------------------------------------------------
	CGUIWgtDialog::CGUIWgtDialog( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
	{
		InitDialog();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtDialog::InitDialog()
	{
		m_pImageBG= NULL;


		SetFlag(eFLAG_EXCLUSIVE, true);
		SetFlag(eFLAG_MOVABLE, true);
		SetFlag(eFLAG_OPEN_WITH_PARENT, false);
	}	

	//------------------------------------------------------------------------------
	int32 CGUIWgtDialog::Create()
	{
		return CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtDialog::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "BGIMAGE")
		{
			m_pImageBG = pImage;
			if( GetSize().IsEqualZero() && pImage )
			{
				SetSize(pImage->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtDialog::PostUpdateDirtyRect()
	{
		CGUIRect	aClientClipRect = m_aClientClipRect;

		CGUIWidget::PostUpdateDirtyRect();

		m_aClipRect = m_aWidgetRect;
		m_aClientClipRect = aClientClipRect;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtDialog::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageBG, GetRect(), pRender->GetAndIncZ(), &GetClipRect());
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtDialog::OnOpen( CGUIEventNotification* pEvent )
	{
		MoveToTop();
		return CGUIWidget::OnOpen(pEvent);
	}



	//------------------------------------------------------------------------------
}//namespace guiex
