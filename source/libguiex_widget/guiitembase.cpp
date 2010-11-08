/** 
* @file guiitembase.h
* @brief base class of item, can be used for list box or manu
* @author ken
* @date 2007-07-31
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiitembase.h>
#include <libguiex_core/guiinterfacerender.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIItemBase);
	//------------------------------------------------------------------------------
	CGUIString CGUIItemBase::ms_strType = "CGUIItemBase";
	//------------------------------------------------------------------------------
	CGUIItemBase::CGUIItemBase( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitItemBase();
	}
	//------------------------------------------------------------------------------
	CGUIItemBase::CGUIItemBase( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
	{
		InitItemBase();
	}
	//------------------------------------------------------------------------------
	CGUIItemBase::~CGUIItemBase(void)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIItemBase::InitItemBase()
	{
		m_bSelected = false;
		m_pUserData = NULL;
		m_nItemID = 0;
		m_pImageSelection = NULL;

		SetGenerateClickEvent( true );
		SetGenerateDBClickEvent(true);
		SetFocusAgency( true );
	}
	//------------------------------------------------------------------------------
	uint32	CGUIItemBase::GetID(void) const
	{
		return m_nItemID;
	}
	//------------------------------------------------------------------------------
	void	CGUIItemBase::SetID(uint32 nItemId)
	{
		m_nItemID = nItemId;
	}
	//------------------------------------------------------------------------------
	void*	CGUIItemBase::GetUserData(void) const
	{
		return m_pUserData;
	}
	//------------------------------------------------------------------------------
	void		CGUIItemBase::SetUserData(void* pUserData)
	{
		m_pUserData = pUserData;
	}
	//------------------------------------------------------------------------------
	bool		CGUIItemBase::IsSelected(void) const
	{
		return m_bSelected;
	}
	//------------------------------------------------------------------------------
	void		CGUIItemBase::SetSelected(bool bSelect)
	{
		m_bSelected = bSelect;
	}
	//------------------------------------------------------------------------------
	void		CGUIItemBase::SetSelectionImage(CGUIImage* pImage)
	{
		m_pImageSelection = pImage;
	}
	//------------------------------------------------------------------------------
	bool	CGUIItemBase::operator<(const CGUIItemBase& rItem) const
	{
		return GetText() < rItem.GetText();
	}
	//------------------------------------------------------------------------------
	bool	CGUIItemBase::operator>(const CGUIItemBase& rItem) const
	{
		return GetText() > rItem.GetText();
	}
	//------------------------------------------------------------------------------
	real		CGUIItemBase::GetHeight(void)
	{
		return GetChild()?GetChild()->GetRect().GetHeight():0.0f;
	}
	//------------------------------------------------------------------------------
	real		CGUIItemBase::GetWidth(void)
	{
		return GetChild()?GetChild()->GetRect().GetWidth():0.0f;
	}
	//------------------------------------------------------------------------------
	void	CGUIItemBase::SetItemInfo(
		const wchar_t* pText, 
		const CGUIStringInfo& rInfo,
		uint32 nItemID,
		void* pItemData ,
		bool bDisable)	
	{
		SetDisable(bDisable);
		SetID(nItemID);
		SetUserData(pItemData);
		SetTextInfo(rInfo);
		SetTextContent(pText);
	}
	//------------------------------------------------------------------------------
	void CGUIItemBase::RenderSelf(IGUIInterfaceRender* pRender)
	{
		///draw selection color if selected
		if( IsSelected())
		{
			DrawImage( pRender, m_pImageSelection, GetRect( ) );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex


