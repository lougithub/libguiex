/** 
* @file guiwidget.cpp
* @brief base class, define most operation of widget
* @author ken
* @date 2006-06-07
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiwidget.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guievent.h>
#include <libguiex_core\guirect.h>
#include <libguiex_core\guiimagemanager.h>
#include <libguiex_core\guianimationmanager.h>
#include <libguiex_core\guiimage.h>
#include <libguiex_core\guianimation.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guipropertymanager.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiinterfacesound.h>
#include <libguiex_core\guiinterfacescript.h>
#include <libguiex_core\guiinterfacefont.h>
#include <libguiex_core\guilogmsgmanager.h>
#include <libguiex_core\guistringconvertor.h>
#include <libguiex_core\guiperfmonitor.h>
#include <libguiex_core\guias.h>

#include <algorithm>

//------------------------------------------------------------------------------

 
//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIWidget::CGUIWidget( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:m_strName(rName)
		,m_strOwnerProjectName(rProjectName)
		,m_strWorkingProjectName(rProjectName)
		,m_pWidgetGenerator(NULL)
		,m_pExclusiveChild(NULL)
		,m_pParent(NULL)
		,m_pChild(NULL)
		,m_pNextSibling(NULL)
		,m_aBitFlag()
		,m_aParamScale(CGUISize(1.0f,1.0f))
		,m_aParamAlpha(1.0f)
		,m_aParamDisable(false)
		,m_aParamVisible(true)
		,m_aParamActivable(false)
		,m_strType(rType)
		,m_bIsOpen( false )
		,m_bRectDirtyFlag(true)
		,m_aWidgetSize(0.0f,0.0f)
		,m_bRelativePos(true)
		,m_bRelativeSize(false)
		,m_aColor(1.0f,1.0f,1.0f,1.0f)
		,m_uTextAlignment(GUI_TA_CENTER)
	{
		//set flag
		m_aBitFlag.set(eFLAG_INHERIT_ALPHA);			//inherit alpha from parent
		m_aBitFlag.reset(eFLAG_FOCUS_AGENCY);			//is this widget a focus agency of it's parent
		m_aBitFlag.set(eFLAG_FOCUSABLE);				//could be set focus
		m_aBitFlag.reset(eFLAG_HIDE);					//hidden by parent
		m_aBitFlag.reset(eFLAG_MOVABLE);				//not movable
		m_aBitFlag.reset(eFLAG_EVENT_CLICK);			//disable click event
		m_aBitFlag.reset(eFLAG_EVENT_DBCLICK);			//disable double click event
		m_aBitFlag.reset(eFLAG_EVENT_MULTICLICK);		//disable multi click event
		m_aBitFlag.reset(eFLAG_EVENT_UPDATE);			//disable update event
		m_aBitFlag.reset(eFLAG_EXCLUSIVE);				//make this widget not a exclusive widget.
		m_aBitFlag.reset(eFLAG_SCROLLBAR_AUTOPOSITION);	//make this widget unable to position scrollbar automatically
		m_aBitFlag.reset(eFLAG_EVENT_PARENTSIZECHANGE);	//could this widget receive parent change event
		m_aBitFlag.set(eFLAG_OPEN_WITH_PARENT);			// should this widget be opened with parent's open
		m_aBitFlag.set(eFLAG_PARENT_CLIENTRECT);		// use parent client rect or whole rect as a client rect
		m_aBitFlag.set(eFLAG_MOUSE_CONSUMED);			//!< should this mouse consume mouse event
		m_aBitFlag.set(eFLAG_HITABLE);					//!< could this widget be hitted


	}
	//------------------------------------------------------------------------------
	CGUIWidget::~CGUIWidget( )
	{
		DestroyAllResource();


		//delete child
		CGUIWidget* pChild = m_pChild;
		CGUIWidget* pChildTmp = NULL;
		while( pChild )
		{
			pChildTmp = pChild;
			pChild = pChild->m_pNextSibling;

			if( CGUIWidgetSystem::Instance()->HasPage( pChildTmp ))
			{
				//it's child is another page
				pChildTmp->SetParent(NULL);
				continue;
			}

			//destroy it
			GUI_DESTROY_WIDGET(pChildTmp);		
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidget::Create()
	{
		return 0;
	}
	//------------------------------------------------------------------------------

	void	CGUIWidget::Open()
	{
		//open self
		if( IsOpen())
		{
			throw CGUIException("[CGUIWidget::Open]: the widget isn't in closed state. TYPE<%s>  NAME<%s>",
				GetType().c_str(), GetName().c_str());
		}

		m_bIsOpen = true;
		CGUIEventNotification aEvent;
		aEvent.SetEventId(eEVENT_OPEN);
		aEvent.SetReceiver(this);
		CGUIWidgetSystem::Instance()->SendEvent( &aEvent);

		//open child if it is set to "open with parent"
		CGUIWidget*	pWidget = m_pChild;
		while(pWidget)
		{
			if( pWidget->GetFlag(eFLAG_OPEN_WITH_PARENT) && pWidget->IsOpen()==false)
			{
				pWidget->Open();
			}
			pWidget = pWidget->m_pNextSibling;
		}	
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::Close()
	{
		//close self
		if( !IsOpen())
		{
			throw CGUIException("[CGUIWidget::Close]: the widget isn't in opened state. TYPE<%s>  NAME<%s>",
				GetType().c_str(), GetName().c_str());
		}

		m_bIsOpen = false;
		//send event
		CGUIEventNotification aEvent;
		aEvent.SetEventId(eEVENT_CLOSE);
		aEvent.SetReceiver(this);
		CGUIWidgetSystem::Instance()->SendEvent( &aEvent);

		//close child
		CGUIWidget* pWidget = m_pChild;
		while( pWidget )
		{
			if( pWidget->IsOpen() )
			{
				pWidget->Close();
			}
			pWidget = pWidget->m_pNextSibling;
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsOpen() const
	{
		return m_bIsOpen;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetUserData(void*	pData)
	{
		m_pUserData = pData;
	}
	//------------------------------------------------------------------------------
	void*	CGUIWidget::GetUserData() const
	{
		return m_pUserData;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetValue(const CGUIString& rName, const CGUIString& rValue)
	{
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWidget::GetValue(const CGUIString& rName) const
	{
		return CGUIString();
	}
	//------------------------------------------------------------------------------
	const CGUIStringEx&	CGUIWidget::GetTooltipText(void) const
	{
		return m_strTooltipText;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetTooltipText(const CGUIStringEx& rText)
	{
		m_strTooltipText = rText;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetTextColor(const CGUIColor& rColor)
	{
		CGUIStringExInfo aInfo = m_strText.GetDefaultInfo( );
		aInfo.m_aColor = rColor;
		m_strText.SetDefaultInfo(&aInfo);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetTextContent( const wchar_t* pText )
	{
		m_strText.SetContent(pText);
	}
	//------------------------------------------------------------------------------
	const wchar_t*	CGUIWidget::GetTextContent() const
	{
		return m_strText.GetContent();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsTextContentEmpty( ) const
	{
		return m_strText.Empty();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetTextContentUTF8( const CGUIString& rString)
	{
		CGUIStringEx strTemp;
		CGUIStringConvertor::MultiByteToWideChar( rString, strTemp);
		SetTextContent( strTemp.GetContent());
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWidget::GetTextContentUTF8() const
	{
		CGUIString aContentUTF8;
		CGUIStringConvertor::WideByteToMultiChar( m_strText, aContentUTF8 );
		return aContentUTF8;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetTextAlignment( uint8 uAlignment)
	{
		m_uTextAlignment = uAlignment;
	}
	//------------------------------------------------------------------------------
	uint8  CGUIWidget::GetTextAlignment( ) const
	{
		return m_uTextAlignment;
	}
	//------------------------------------------------------------------------------
	const CGUIStringEx&		CGUIWidget::GetText() const
	{
		return m_strText;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetTextInfo(const CGUIStringExInfo& rInfo)
	{
		m_strText.SetDefaultInfo(&rInfo);
	}
	//------------------------------------------------------------------------------
	const CGUIStringExInfo&		CGUIWidget::GetTextInfo( ) const
	{
		return m_strText.GetDefaultInfo();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetGenerator( const CGUIWidgetGenerator* pGenerator)
	{
		m_pWidgetGenerator = pGenerator;
	}
	//------------------------------------------------------------------------------
	const	CGUIWidgetGenerator* CGUIWidget::GetGenerator() const
	{
		return m_pWidgetGenerator;
	}
	//------------------------------------------------------------------------------
	void		CGUIWidget::SetParent(CGUIWidget *pParent)
	{
		if( m_pParent == pParent )
		{
			return;
		}

		//remove from old parent
		if( m_pParent )
		{
			m_pParent->RemoveChild(this);
		}

		//add to current parent
		if( pParent )
		{
			pParent->AddChild(this);
		}
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CGUIWidget::GetParent() const
	{
		return m_pParent;
	}
	//------------------------------------------------------------------------------
	CGUIWidget *CGUIWidget::GetRoot()
	{
		CGUIWidget* pWidget = this;
		while( pWidget->m_pParent )
		{
			pWidget = pWidget->m_pParent;
		}
		return pWidget;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidget::GetChild( ) const
	{
		return m_pChild;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidget::GetChild( const CGUIString& rChildName ) const
	{
		CGUIWidget* pWidget = m_pChild;
		while( pWidget )
		{
			if( pWidget->GetName() == rChildName )
			{
				return pWidget;
			}
			pWidget = pWidget->m_pNextSibling;
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidget::GetNextSibling( ) const
	{
		return m_pNextSibling;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidget::GetExclusiveChild( ) const
	{
		return m_pExclusiveChild;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetWorkingProjectName(const CGUIString& rWorkingProjName)
	{
		m_strWorkingProjectName = rWorkingProjName;

		CGUIWidget* pWidget = m_pChild;
		while( pWidget )
		{
			pWidget->SetWorkingProjectName( rWorkingProjName );
			pWidget = pWidget->m_pNextSibling;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::MoveDown()
	{
		if( m_pParent )
		{
			//check for only one child, or have been last one
			if( !m_pParent->m_pChild->m_pNextSibling ||
				m_pParent->m_pChild == this )
			{
				return;
			}

			//check for second one
			if( this == m_pParent->m_pChild->m_pNextSibling )
			{
				m_pParent->m_pChild->m_pNextSibling = this->m_pNextSibling;
				this->m_pNextSibling = m_pParent->m_pChild;
				m_pParent->m_pChild = this;
			}
			else
			{
				//find my brother's brother
				CGUIWidget* pWidget = m_pParent->m_pChild;
				while( pWidget && pWidget->m_pNextSibling)
				{
					if( pWidget->m_pNextSibling->m_pNextSibling == this )
					{
						break;
					}
					pWidget = pWidget->m_pNextSibling;
				}
				GUI_ASSERT( pWidget, "error");
				pWidget->m_pNextSibling->m_pNextSibling = this->m_pNextSibling;
				this->m_pNextSibling = pWidget->m_pNextSibling;
				pWidget->m_pNextSibling = this;
			}
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::MoveUp()
	{
		if( m_pParent )
		{
			//check for only one child, or have topest
			if( !m_pParent->m_pChild->m_pNextSibling ||
				!this->m_pNextSibling )
			{
				return;
			}

			//check for first
			if( this == m_pParent->m_pChild )
			{
				m_pParent->m_pChild = m_pParent->m_pChild->m_pNextSibling;
				this->m_pNextSibling = m_pParent->m_pChild->m_pNextSibling;
				m_pParent->m_pChild->m_pNextSibling = this;
			}
			else
			{
				//find my brother
				CGUIWidget* pWidget = m_pParent->m_pChild;
				while( pWidget && pWidget->m_pNextSibling)
				{
					if( pWidget->m_pNextSibling == this )
					{
						break;
					}
					pWidget = pWidget->m_pNextSibling;
				}
				GUI_ASSERT( pWidget, "error");
				pWidget->m_pNextSibling = this->m_pNextSibling;
				this->m_pNextSibling = pWidget->m_pNextSibling->m_pNextSibling;
				pWidget->m_pNextSibling->m_pNextSibling = this;

			}
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::MoveToTop()
	{
		if( m_pParent )
		{
			//check for only one child
			if( !m_pParent->m_pChild->m_pNextSibling)
			{
				return;
			}

			//check for first
			if( this == m_pParent->m_pChild )
			{
				m_pParent->m_pChild = m_pParent->m_pChild->m_pNextSibling;
				this->m_pNextSibling = NULL;
			}

			//find this
			CGUIWidget* pWidget = m_pParent->m_pChild;

			while( pWidget && pWidget->m_pNextSibling)
			{
				if( pWidget->m_pNextSibling == this )
				{
					break;
				}
				pWidget = pWidget->m_pNextSibling;
			}

			if( !pWidget->m_pNextSibling )
			{
				//add to last
				pWidget->m_pNextSibling = this;
			}
			else
			{
				//remove it first
				pWidget->m_pNextSibling = this->m_pNextSibling;
				this->m_pNextSibling = NULL;

				//add to last
				while( pWidget )
				{
					if( !pWidget->m_pNextSibling )
					{
						pWidget->m_pNextSibling = this;
						return;
					}
					pWidget = pWidget->m_pNextSibling;
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RemoveChild( CGUIWidget* pChild )
	{
		GUI_ASSERT( pChild, "invalid parameter");
		GUI_ASSERT( pChild->m_pParent == this, "the child's parent isn't this!");

		if( pChild == m_pChild )
		{
			m_pChild = m_pChild->m_pNextSibling;
		}
		else
		{
			bool bFind = false;
			CGUIWidget* pParam = m_pChild;
			while(pParam && pParam->m_pNextSibling)
			{
				if( pParam->m_pNextSibling == pChild )
				{
					pParam->m_pNextSibling = pParam->m_pNextSibling->m_pNextSibling;
					bFind = true;
					break;
				}
				else
				{
					pParam = pParam->m_pNextSibling;
				}
			}
			if( !bFind ) 
			{
				throw CGUIException("[CGUIWidget::RemoveChild]: failed to find widget, type <%s>, name<%s>!",
					pChild->GetType().c_str(), pChild->GetName().c_str());
			}
		}
		pChild->m_pParent = NULL;
		pChild->m_pNextSibling = NULL;

		//for exclusive child
		if( pChild->GetFlag(eFLAG_EXCLUSIVE))
		{
			GUI_ASSERT( pChild == m_pExclusiveChild, "error for remove exclusive child");
			m_pExclusiveChild = NULL;
		}

		//for parameter
		m_aParamScale.RemoveChild(&(pChild->m_aParamScale));
		m_aParamAlpha.RemoveChild(&(pChild->m_aParamAlpha));
		m_aParamDisable.RemoveChild(&(pChild->m_aParamDisable));
		m_aParamActivable.RemoveChild(&(pChild->m_aParamActivable));
		m_aParamVisible.RemoveChild(&(pChild->m_aParamVisible));

		//send event for change parent
		{
			CGUIEventRelativeChange aEvent;
			aEvent.SetEventId(eEVENT_CHANGE_PARENT);
			aEvent.SetRelative(this);
			aEvent.SetReceiver(pChild);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}


		//send event for add child
		{
			CGUIEventRelativeChange aEvent;
			aEvent.SetEventId(eEVENT_REMOVE_CHILD);
			aEvent.SetRelative(pChild);
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidget::GetLastChild( )
	{
		CGUIWidget* pLastChild = m_pChild;
		while( pLastChild )
		{
			if( pLastChild->m_pNextSibling )
			{
				pLastChild = pLastChild->m_pNextSibling;
			}
			else
			{
				return pLastChild;
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::AddChild( CGUIWidget* pChild )
	{
		GUI_ASSERT( pChild, "invalid parameter");
		GUI_ASSERT( pChild->m_pParent == NULL, "the child has haven a parent!");

		CGUIWidget* pLastChild = GetLastChild();
		if( pLastChild )
		{
			pLastChild->m_pNextSibling = pChild;
		}
		else
		{
			m_pChild = pChild;
		}
		pChild->m_pNextSibling = NULL;
		pChild->m_pParent = this;

		//for exclusive child
		if( pChild->GetFlag(eFLAG_EXCLUSIVE))
		{
			GUI_ASSERT(m_pExclusiveChild==NULL, "there has been a exclusive child");
			m_pExclusiveChild = pChild;
		}

		//for parameter
		m_aParamScale.AddChild(&(pChild->m_aParamScale));
		m_aParamAlpha.AddChild(&(pChild->m_aParamAlpha));
		m_aParamDisable.AddChild(&(pChild->m_aParamDisable));
		m_aParamActivable.AddChild(&(pChild->m_aParamActivable));
		m_aParamVisible.AddChild(&(pChild->m_aParamVisible));
		pChild->SetRectDirty();


		//send event for change parent
		{
			CGUIEventRelativeChange aEvent;
			aEvent.SetEventId(eEVENT_CHANGE_PARENT);
			aEvent.SetRelative(this);
			aEvent.SetReceiver(pChild);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}


		//send event for add child
		{
			CGUIEventRelativeChange aEvent;
			aEvent.SetEventId(eEVENT_ADD_CHILD);
			aEvent.SetRelative(pChild);
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetSelfActivable(bool bActivable)
	{
		m_aParamActivable.SetSelfValue(bActivable);
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsSelfActivable( ) const
	{
		return m_aParamActivable.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsActivable()
	{
		return m_aParamActivable.GetFinalValue();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetFocusable(bool bFocusable)
	{
		bFocusable ? m_aBitFlag.set(eFLAG_FOCUSABLE) : m_aBitFlag.reset(eFLAG_FOCUSABLE);
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsFocusable() const
	{
		if( GetFlag(eFLAG_FOCUS_AGENCY))
		{
			//is a focus agency
			if( m_pParent )
			{
				return m_pParent->IsFocusable( );
			}

			throw CGUIException("[CGUIWidget::IsFocusable]: lack of parent! TYPE<%s>  NAME<%s>",
				GetType().c_str(),GetName().c_str());
		}

		return m_aBitFlag.test(eFLAG_FOCUSABLE);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetFocus(bool bFocus)
	{
		if( bFocus && GetFlag(eFLAG_FOCUS_AGENCY))
		{
			//is a focus agency
			if( m_pParent )
			{
				m_pParent->SetFocus( bFocus);
				return;
			}
			throw CGUIException("[CGUIWidget::SetFocus]: lack of parent! TYPE<%s>  NAME<%s>",
				GetType().c_str(),GetName().c_str());
		}

		CGUIWidgetSystem* pSys = CGUIWidgetSystem::Instance();
		if( !bFocus )
		{
			/// lost focus
			if( pSys->GetFocusWidget()  == this)
			{
				CGUIEventNotification aEvent;
				aEvent.SetEventId(eEVENT_FOCUS_LOST);
				aEvent.SetReceiver(this);
				CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
			}
			return;
		}


		if( pSys->GetFocusWidget()  == this)
		{
			return;
		}
		//remove old widget
		if( pSys->GetFocusWidget() )
		{
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_FOCUS_LOST);
			aEvent.SetReceiver(pSys->GetFocusWidget());
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}

		//add this widget 
		if( IsFocusable())
		{
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_FOCUS_GET);
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsFocus() const
	{
		if( GetFlag(eFLAG_FOCUS_AGENCY))
		{
			//is a focus agency
			if( m_pParent )
			{
				return m_pParent->IsFocus( );
			}
			throw CGUIException("[CGUIWidget::IsFocus]: lack of parent! TYPE<%s>  NAME<%s>",
				GetType().c_str(),GetName().c_str());
		}

		return CGUIWidgetSystem::Instance()->GetFocusWidget() == this;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetSelfVisible(bool bVisible)
	{
		if( bVisible != IsVisible() )
		{
			return m_aParamVisible.SetSelfValue(bVisible);

			CGUIEventNotification aEvent;
			if( bVisible )
			{
				aEvent.SetEventId(eEVENT_VISIBLE);
			}
			else
			{
				aEvent.SetEventId(eEVENT_INVISIBLE);
			}
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsSelfVisible() const
	{
		return m_aParamVisible.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsVisible()
	{
		return m_aParamVisible.GetFinalValue() && !GetFlag(eFLAG_HIDE);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::Hide( )
	{
		m_aBitFlag.set(eFLAG_HIDE);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::Show( )
	{
		m_aBitFlag.reset(eFLAG_HIDE);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetFlag( uint32	nFlag, bool bEnable)
	{
		GUI_ASSERT( nFlag >=0 && nFlag < eFLAG_MAX , "unknown widget flag");

		bEnable ? m_aBitFlag.set(nFlag) : m_aBitFlag.reset(nFlag);
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::GetFlag( uint32	nFlag) const
	{
		GUI_ASSERT( nFlag >=0 && nFlag < eFLAG_MAX , "unknown widget flag");

		return m_aBitFlag.test( nFlag );
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetSelfDisable(bool bDisable)
	{
		if( bDisable != IsSelfDisable() )
		{
			CGUIEventNotification aEvent;
			if( bDisable )
			{
				aEvent.SetEventId(eEVENT_DISABLE);
			}
			else
			{
				aEvent.SetEventId(eEVENT_ENABLE);
			}
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsSelfDisable( ) const
	{
		return m_aParamDisable.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsDisable()
	{
		return m_aParamDisable.GetFinalValue();
	}
	//------------------------------------------------------------------------------
	void			CGUIWidget::SetPositionRelative( bool bRelative )
	{
		if( m_bRelativePos != bRelative )
		{
			m_bRelativePos = bRelative;

			SetRectDirty();
		}
	}
	//------------------------------------------------------------------------------
	bool			CGUIWidget::IsPositionRelative() const
	{
		return m_bRelativePos;
	}
	//------------------------------------------------------------------------------
	void			CGUIWidget::SetSizeRelative( bool bRelative )
	{
		if( m_bRelativeSize != bRelative )
		{
			m_bRelativeSize = bRelative;

			SetRectDirty();
		}
	}
	//------------------------------------------------------------------------------
	bool			CGUIWidget::IsSizeRelative() const
	{
		return m_bRelativeSize;
	}
	//------------------------------------------------------------------------------
	void			CGUIWidget::UpdateSize()
	{
		SetSize(GetSize());
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::ClientToGlobal( CGUIVector2& rPos )
	{
		rPos += GetGlobalPosition();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::GlobalToClient( CGUIVector2& rPos )
	{
		rPos -= GetGlobalPosition();
	}
	//------------------------------------------------------------------------------
	CGUIWidget*		CGUIWidget::GetWidgetAtPoint(const CGUIVector2& rPos)
	{
		//check sibling
		if( m_pNextSibling && 
			!(GetFlag(eFLAG_EXCLUSIVE) && IsDisable() == false && IsOpen() && IsVisible()))
		{
			CGUIWidget* pWidget = m_pNextSibling->GetWidgetAtPoint(rPos);
			if( pWidget &&(!pWidget->GetFlag(eFLAG_EXCLUSIVE) || pWidget->HitTest(rPos)))
			{
				//find one
				return pWidget;
			}
		}


		if( IsDisable() == false && IsOpen() && IsVisible()) 
		{
			//check child
			bool bIsChildExclusive = false;
			if( m_pChild )
			{
				CGUIWidget* pWidget = NULL;
				if( m_pExclusiveChild &&  m_pExclusiveChild->IsDisable() == false && m_pExclusiveChild->IsOpen() && m_pExclusiveChild->IsVisible())
				{
					//check exclusive child
					bIsChildExclusive = true;
					pWidget = m_pExclusiveChild->GetWidgetAtPoint(rPos);
				}
				else
				{
					//check normal child
					pWidget = m_pChild->GetWidgetAtPoint(rPos);
				}
				if( pWidget &&(!pWidget->GetFlag(eFLAG_EXCLUSIVE) || pWidget->HitTest(rPos)))
				{
					//find one
					return pWidget;
				}
			}


			//check self
			if( !bIsChildExclusive && HitTest(rPos) )
			{
				return this;
			}
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::HitTest( const CGUIVector2& rPos)
	{
		if( GetFlag(eFLAG_HITABLE) )
		{
			return GetClipRect().IsPointInRect(rPos);
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterUIEvent( const CGUIString& rUIEventName )
	{
		CGUIWidgetSystem::Instance()->RegisterUIEvent( rUIEventName, this );
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterUIEvent( const CGUIString& rUIEventName )
	{
		CGUIWidgetSystem::Instance()->UnregisterUIEvent( rUIEventName, this );
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterSound( const CGUIString& strEventName, int32 nSoundIdx )
	{
		m_mapEventSound.insert( std::make_pair(strEventName, nSoundIdx));
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterSound( const CGUIString& strEventName )
	{
		TMapSound::iterator itor = m_mapEventSound.find(strEventName);
		if( itor != m_mapEventSound.end())
		{
			m_mapEventSound.erase(itor);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterScriptCallbackFunc( 
		const CGUIString& strEventName, 
		const CGUIString& strFunc )
	{
		m_mapScriptFunc.insert( std::make_pair(strEventName, strFunc));
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterScriptCallbackFunc(const CGUIString& strEventName)
	{
		TMapScriptFunc::iterator itor = m_mapScriptFunc.find(strEventName);
		if( itor != m_mapScriptFunc.end())
		{
			m_mapScriptFunc.erase(itor);
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::HasScriptCallbackFunc(const CGUIString& strEventName) const
	{
		return m_mapScriptFunc.find(strEventName) != m_mapScriptFunc.end();
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWidget::GetScriptCallbackFunc(const CGUIString& strEventName) const
	{
		TMapScriptFunc::const_iterator itor = m_mapScriptFunc.find(strEventName);
		if( itor == m_mapScriptFunc.end())
		{
			throw CGUIException("[CGUIWidget::GetScriptCallbackFunc]: failed to find event function by name <%s>!", strEventName.c_str());
		}
		return itor->second;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterGlobalTimerFunc(const CGUIString& strEventName)
	{
		TMapTimer::iterator itor1 = m_aMapTimer.find(strEventName);
		TMapGlobalFunc::iterator itor2 = m_mapGlobalFunc.find(strEventName);
		if( itor1 == m_aMapTimer.end() || itor2 == m_mapGlobalFunc.end())
		{
			throw CGUIException("[CGUIWidget::UnregisterGlobalTimerFunc]: failed to find event <%s>!", strEventName.c_str());
		}
		m_aMapTimer.erase(itor1);
		m_mapGlobalFunc.erase(itor2);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterScriptTimerFunc(const CGUIString& strEventName)
	{
		TMapTimer::iterator itor1 = m_aMapTimer.find(strEventName);
		TMapScriptFunc::iterator itor2 = m_mapScriptFunc.find(strEventName);
		if( itor1 == m_aMapTimer.end() || itor2 == m_mapScriptFunc.end())
		{
			throw CGUIException("[CGUIWidget::UnregisterScriptTimerFunc]: failed to find event <%s>!", strEventName.c_str());
		}
		m_aMapTimer.erase(itor1);
		m_mapScriptFunc.erase(itor2);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterScriptTimerFunc( 
		uint32 nTimeGap, 
		const CGUIString&strEventName, 
		const CGUIString& strFunc )
	{
		m_aMapTimer.insert( std::make_pair(strEventName, STimer(nTimeGap)));
		m_mapScriptFunc.insert( std::make_pair(strEventName, strFunc));
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterGlobalTimerFunc( 		
		uint32 nTimeGap, 
		const CGUIString& strEventName,  
		void (*pFunc)(CGUIEventTimer*) )
	{
		m_aMapTimer.insert( std::make_pair(strEventName, STimer(nTimeGap)));
		m_mapGlobalFunc.insert( std::make_pair(strEventName, reinterpret_cast<CallbackEventFunc>(pFunc)));
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::CallGlobalFunction(const CGUIString& strEventName, CGUIEvent* pEvent)
	{
		TMapGlobalFunc::iterator itor = m_mapGlobalFunc.find(strEventName);
		if( itor != m_mapGlobalFunc.end())
		{
			(*(itor->second))(pEvent);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::CallScriptFunction(const CGUIString& strEventName, CGUIEvent* pEvent)
	{
		if( CGUIWidgetSystem::Instance()->ShouldRunScript())
		{

			TMapScriptFunc::iterator itor = m_mapScriptFunc.find(strEventName);
			if( itor != m_mapScriptFunc.end())
			{
				//get script interface
				guiex::IGUIInterfaceScript* pScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
				pScript->ExecuteEventHandler( itor->second, pEvent, GetWorkingProjectName());
			}
		}

	}
	//------------------------------------------------------------------------------
	void CGUIWidget::CallbackFunction(const CGUIString& strEventName, CGUIEvent* pEvent)
	{		
		//play sound
		TMapSound::iterator itor = m_mapEventSound.find( strEventName );
		if( itor != m_mapEventSound.end())
		{
			IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
			pSound->PlayEffect(itor->second);
		}

		//call global function
		CallGlobalFunction(strEventName, pEvent);

		//call script function
		CallScriptFunction(strEventName, pEvent);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetSelfScale( const CGUISize& rSize )
	{
		if(m_aParamScale.GetSelfValue() != rSize )
		{
			if( rSize.m_fHeight <= 0.0f || rSize.m_fWidth <= 0.0f)
			{
				GUI_FORCE_ASSERT( GUI_FORMAT("can't set the zero scale to a widget <%s>", m_strName.c_str()));
				return;
			}

			m_aParamScale.SetSelfValue(rSize);
			SetRectDirty();

			//send event
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_SCALE_CHANGE);
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent );
		}

	}
	//------------------------------------------------------------------------------
	const CGUISize &  CGUIWidget::GetSelfScale( ) const
	{
		return m_aParamScale.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	const CGUISize &	CGUIWidget::GetScale()
	{
		return m_aParamScale.GetFinalValue();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetSelfAlpha(real fAlpha)
	{
		if( GetSelfAlpha() != fAlpha )
		{
			m_aParamAlpha.SetSelfValue(fAlpha);

			CGUIEventAlpha aEvent;
			aEvent.SetEventId(eEVENT_CHANGE_ALPHA);
			aEvent.SetAlpha(fAlpha);
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	real	CGUIWidget::GetSelfAlpha()  const
	{
		return m_aParamAlpha.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	real	CGUIWidget::GetAlpha() 
	{
		if( GetFlag(eFLAG_INHERIT_ALPHA)&&m_pParent )
		{
			return m_aParamAlpha.GetFinalValue();
		}
		else
		{
			return m_aParamAlpha.GetSelfValue();
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{

	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		//clear old one
		TMapImage::iterator itor = m_aMapImage.find(rName );
		if( itor != m_aMapImage.end())
		{
			m_aMapImage.erase( itor );
		}

		OnSetImage( rName, pImage );
		if( pImage )
		{
			m_aMapImage.insert( std::make_pair( rName, pImage));
		}

		SetRectDirty();
	}
	//------------------------------------------------------------------------------
	CGUIImage*	CGUIWidget::SetImage( const CGUIString& rName, const CGUIString& rImageName )
	{
		//find image
		CGUIImage* pImage = CGUIImageManager::Instance()->FindResource( rImageName );
		if( !pImage )
		{
			throw CGUIException( "failed to get image by name <%s>", rImageName.c_str());
			return NULL;
		};
		SetImage(rName, pImage);
		return pImage;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation )
	{
		TMapAnimation::iterator itor = m_aMapAnimation.find(rName);
		if( itor != m_aMapAnimation.end())
		{
			//remove old one
			m_aMapAnimation.erase(itor);
		}

		m_aMapAnimation.insert(std::make_pair(rName,pAnimation));
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIWidget::SetAnimation( const CGUIString& rName, const CGUIString& rAnimationName )
	{
		//find image
		CGUIAnimation* pAnimation = CGUIAnimationManager::Instance()->FindResource( rAnimationName );
		if( !pAnimation )
		{
			throw CGUIException( "failed to get animation by name <%s>", rAnimationName.c_str());
			return NULL;
		};
		SetAnimation(rName, pAnimation);
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation*	CGUIWidget::GetAnimation( const CGUIString& rAnimationName)
	{
		TMapAnimation::iterator itor = m_aMapAnimation.find(rAnimationName);
		if( itor != m_aMapAnimation.end())
		{
			return itor->second;
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	bool		CGUIWidget::HasImage( const CGUIString& rName )
	{
		return m_aMapImage.find(rName ) != m_aMapImage.end();
	}
	//------------------------------------------------------------------------------
	CGUIImage*	CGUIWidget::GetImage( const CGUIString& rName )
	{
		TMapImage::iterator itor=  m_aMapImage.find(rName);
		if( itor == m_aMapImage.end())
		{
			return NULL;
		}
		else
		{
			return itor->second;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DestroyAllResource( )
	{
		//release image
		m_aMapImage.clear();

		//release animation
		m_aMapAnimation.clear();

		//release as
		for( TListAs::iterator itor = m_listAs.begin();
			itor != m_listAs.end();
			++itor)
		{
			CGUIAsFactory::Instance()->DestroyAs(*itor);
		}
		m_listAs.clear();
	}
	//------------------------------------------------------------------------------
	void			CGUIWidget::SetPropertySet( const CGUIPropertySet&	rProperty)
	{
		ClearProperty();

		m_aPropertySet = rProperty;
	}
	//------------------------------------------------------------------------------
	void			CGUIWidget::SetProperty( const CGUIProperty&	rProperty)
	{
		if( m_aPropertySet.HasProperty(rProperty.GetName()))
		{
			*m_aPropertySet.GetProperty(rProperty.GetName()) = rProperty;
		}
		else
		{
			m_aPropertySet.AddProperty(rProperty.Clone());
		}
	}
	//------------------------------------------------------------------------------
	const CGUIPropertySet&	CGUIWidget::GetProperty() const
	{
		return m_aPropertySet;
	}
	//------------------------------------------------------------------------------
	void			CGUIWidget::ClearProperty()
	{
		m_aPropertySet.Clear();
	}
	//------------------------------------------------------------------------------
	CGUIProperty*	CGUIWidget::GenerateProperty(const CGUIString& rName, const CGUIString& rType )
	{
		CGUIProperty* pProperty = NULL;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		if( rName == "LOCAL_POSITION" && rType == "VECTOR2")
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::Vector2ToString(GetLocalPosition()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "SIZE" && rType == "SIZE" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::SizeToString(GetSize()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "TAG_POINT" && rType == "VECTOR2" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::Vector2ToString(GetTagPoint()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "MAX_SIZE" && rType == "SIZE" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::SizeToString(GetMaximumSize()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "MIN_SIZE" && rType == "SIZE" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::SizeToString(GetMinimumSize()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rType == "IMAGE" )
		{
			CGUIImage* pImage = GetImage(rName);
			if( pImage )
			{
				pProperty = CGUIPropertyManager::Instance()->CreateProperty(
					rName, 
					rType, 
					pImage->GetName());
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rType == "EVENT" )
		{
			if( HasScriptCallbackFunc(rName) )
			{
				pProperty = CGUIPropertyManager::Instance()->CreateProperty(
					rName, 
					rType, 
					GetScriptCallbackFunc(rName));
			}
			else
			{
				pProperty = CGUIPropertyManager::Instance()->CreateProperty(
					rName, 
					rType);
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "ALPHA" && rType == "REAL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::RealToString(GetSelfAlpha()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "ACTIVABLE" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsSelfActivable()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "EXCLUSIVE" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(GetFlag(eFLAG_EXCLUSIVE)));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "DISABLE" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsSelfDisable()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "OPEN_WITH_PARENT" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(GetFlag(eFLAG_OPEN_WITH_PARENT)));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "EVENT_UPDATE" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(GetFlag(eFLAG_EVENT_UPDATE)));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "MOVABLE" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(GetFlag(eFLAG_MOVABLE)));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "HITABLE" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(GetFlag(eFLAG_HITABLE)));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for position
		/*
		*<property name="WIDGET_POSITION" type="WIDGET_POSITION">
		*	<property name="RELATIVE" type="BOOL" value="true" />
		*	<property name="POSITION"	type="VECTOR2" value="0,0" />
		*</property>
		*/
		else if( rName == "WIDGET_POSITION" && rType =="WIDGET_POSITION")
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);

			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("RELATIVE","BOOL",CGUIStringConvertor::BoolToString(IsPositionRelative())));
			CGUIVector2 aPos = IsPositionRelative() ? GetPositionRatio() : GetLocalPosition();
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("POSITION","VECTOR2",CGUIStringConvertor::Vector2ToString(aPos)));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for position
		/*
		*<property name="WIDGET_SIZE" type="WIDGET_SIZE">
		*	<property name="RELATIVE" type="BOOL" value="false" />
		*	<property name="SIZE"	type="SIZE" value="100,100" />
        *   <property name="MAX_SIZE" type="SIZE" value="0,0"/>
        *   <property name="MIN_SIZE" type="SIZE" value="0,0"/>
		*</property>
		*/
		else if( rName == "WIDGET_SIZE" && rType =="WIDGET_SIZE")
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);

			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("RELATIVE","BOOL",CGUIStringConvertor::BoolToString(IsSizeRelative())));
			CGUISize aSize = IsSizeRelative() ? GetSizeRatio() : GetSize();
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("SIZE","SIZE",CGUIStringConvertor::SizeToString(aSize)));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("MAX_SIZE","SIZE",CGUIStringConvertor::SizeToString(GetMaximumSize())));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("MIN_SIZE","SIZE",CGUIStringConvertor::SizeToString(GetMinimumSize())));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "RELATIVE_POS" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsPositionRelative()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "RELATIVE_SIZE" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsSizeRelative()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "TEXT_INFO" && rType == "STRING_INFO" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);

			const CGUIStringExInfo& rInfo = GetTextInfo();
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("SIZE","UINT",CGUIStringConvertor::UIntToString(rInfo.m_nFontSize)));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("FONT_INDEX","UINT",CGUIStringConvertor::UIntToString(rInfo.m_nFontIdx)));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("COLOR","COLOR",CGUIStringConvertor::ColorToString(rInfo.m_aColor)));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "TEXT" && rType == "STRING" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);
			CGUIString aStrText;
			CGUIStringConvertor::WideByteToMultiChar( GetTextContent(), aStrText);
			pProperty->SetValue(aStrText);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "TEXT_ALIGNMENT_H" && rType == "ENUM" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);
			if( (m_uTextAlignment&GUI_TA_HORIZON_MASK) == GUI_TA_H_LEFT )
			{
				pProperty->SetValue("GUI_TA_H_LEFT");
			}
			else if( (m_uTextAlignment&GUI_TA_HORIZON_MASK) == GUI_TA_H_RIGHT )
			{
				pProperty->SetValue("GUI_TA_H_RIGHT");
			}
			else if( (m_uTextAlignment&GUI_TA_HORIZON_MASK) == GUI_TA_H_CENTER )
			{
				pProperty->SetValue("GUI_TA_H_CENTER");
			}
			else
			{
				throw CGUIException("[CGUIWidget::GenerateProperty]: unknown text alignment format");
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rName == "TEXT_ALIGNMENT_V" && rType == "ENUM" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);
			if( (m_uTextAlignment&GUI_TA_VERTICAL_MASK) == GUI_TA_V_UP )
			{
				pProperty->SetValue("GUI_TA_V_UP");
			}
			else if( (m_uTextAlignment&GUI_TA_VERTICAL_MASK) == GUI_TA_V_DOWN )
			{
				pProperty->SetValue("GUI_TA_V_DOWN");
			}
			else if( (m_uTextAlignment&GUI_TA_VERTICAL_MASK) == GUI_TA_V_CENTER )
			{
				pProperty->SetValue("GUI_TA_V_CENTER");
			}
			else
			{
				throw CGUIException("[CGUIWidget::GenerateProperty]: unknown text alignment format");
			}
		}
		return pProperty;
	}
	//------------------------------------------------------------------------------
	void			CGUIWidget::ProcessProperty( const CGUIProperty* pProperty)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for parent
		/*
		*<property name="PARENT" type="STRING" value="sample_frame1" />
		*/
		if( pProperty->GetName() == "PARENT" && pProperty->GetType()=="STRING")
		{
			CGUIWidget* pParent = CGUIWidgetSystem::Instance()->GetWidget( pProperty->GetValue(), GetProjectName());
			SetParent( pParent);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// load image
		/*
		*<property name="BGIMAGE" type="IMAGE">
		*		<property name="PATH" type="STRING" value="./data/sample.png"/>
		*		<property name="UV" type="RECT" value="0,0,0.5,1" />
		*		<property name="IMAGE_OPERATION" type="ENUM" value="IMAGE_NONE" />
		*</property>
		*
		*<property name="BGIMAGE" type="IMAGE">
		*		<property name="COLOR" type="COLOR" value="0.5,0.0,0.6,1.0"/>
		*</property>
		*
		*<property name="BGIMAGE" type="IMAGE">
		*		<property name="IMAGE_NAME" type="STRING" value="btn_ok_hover"/>
		*</property>
		*
		*/
		//else if(pProperty->GetType() == "IMAGE_DATA")
		//{
		//	CGUIImage* pImg = CGUIImageManager::Instance()->CreateImage(*pProperty);

		//	SetImage( pProperty->GetName(), pImg);
		//}
		/*
		*<property name="BGIMAGE" type="IMAGE" value="btn_ok_hover" />
		*/
		else if( pProperty->GetType() == "IMAGE" )
		{
			if(!pProperty->GetValue().empty())
			{
				CGUIImage* pImg = CGUIImageManager::Instance()->FindResource(pProperty->GetValue());
				SetImage( pProperty->GetName(), pImg);
			}
			else{
				//clear image
				SetImage( pProperty->GetName(), NULL);
			}
			//if(!pProperty->GetValue().empty())
			//{
			//	CGUIImage* pImg = CGUIImageManager::Instance()->CreateImage(*pProperty);
			//	SetImage( pProperty->GetName(), pImg);
			//}
			//else if( pProperty->GetPropertyNum() > 0)
			//{
			//	CGUIImage* pImg = CGUIImageManager::Instance()->CreateImage(*pProperty);
			//	SetImage( pProperty->GetName(), pImg);
			//}
			//else
			//{
			//	//clear image
			//	SetImage( pProperty->GetName(), NULL);
			//}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text
		/*
		*<property name="TEXT_INFO" type="STRING_INFO">
		*		<property name="SIZE"	type="NUMBER" value="22"/>
		*		<property name="FONT_INDEX" type="NUMBER" value="0"/>
		*		<property name="COLOR" type="COLOR" value="0.5,0.0,0.6,1.0"/>
		*</property>
		*<property name="TEXT" type="STRING">
		*/
		else if( pProperty->GetName() == "TEXT_INFO" && pProperty->GetType()=="STRING_INFO")
		{
			const CGUIProperty* pPropertySize = pProperty->GetProperty("SIZE");
			const CGUIProperty* pPropertyIdx = pProperty->GetProperty("FONT_INDEX");
			const CGUIProperty* pPropertyColor = pProperty->GetProperty("COLOR");
			CGUIStringExInfo aInfo;
			aInfo.m_aColor = CGUIStringConvertor::StringToColor(pPropertyColor->GetValue());
			aInfo.m_nFontIdx = CGUIStringConvertor::StringToInt(pPropertyIdx->GetValue());
			aInfo.m_nFontSize = CGUIStringConvertor::StringToUInt(pPropertySize->GetValue());
			SetTextInfo(aInfo);
		}
		else if( pProperty->GetName() == "TEXT" && pProperty->GetType()=="STRING")
		{
			CGUIStringEx aStrText;
			CGUIStringConvertor::MultiByteToWideChar(pProperty->GetValue(), aStrText);
			SetTextContent(aStrText.GetContent());
		}
		else if( pProperty->GetName() == "TEXT_ALIGNMENT_H" && pProperty->GetType()=="ENUM")
		{
			if( pProperty->GetValue() == "GUI_TA_H_LEFT" )
			{
				m_uTextAlignment = (m_uTextAlignment & GUI_TA_VERTICAL_MASK) + GUI_TA_H_LEFT;
			}
			else if( pProperty->GetValue() == "GUI_TA_H_RIGHT" )
			{
				m_uTextAlignment = (m_uTextAlignment & GUI_TA_VERTICAL_MASK) + GUI_TA_H_RIGHT;
			}
			else if( pProperty->GetValue() == "GUI_TA_H_CENTER" )
			{
				m_uTextAlignment = (m_uTextAlignment & GUI_TA_VERTICAL_MASK) + GUI_TA_H_CENTER;
			}
			else
			{
				throw CGUIException("[CGUIWidget::ProcessProperty]: unknown text alignment format, <%s>", pProperty->GetValue().c_str() );
			}
		}
		else if( pProperty->GetName() == "TEXT_ALIGNMENT_V" && pProperty->GetType()=="ENUM")
		{
			if( pProperty->GetValue() == "GUI_TA_V_UP" )
			{
				m_uTextAlignment = (m_uTextAlignment & GUI_TA_HORIZON_MASK) + GUI_TA_V_UP;
			}
			else if( pProperty->GetValue() == "GUI_TA_V_DOWN" )
			{
				m_uTextAlignment = (m_uTextAlignment & GUI_TA_HORIZON_MASK) + GUI_TA_V_DOWN;
			}
			else if( pProperty->GetValue() == "GUI_TA_V_CENTER" )
			{
				m_uTextAlignment = (m_uTextAlignment & GUI_TA_HORIZON_MASK) + GUI_TA_V_CENTER;
			}
			else
			{
				throw CGUIException("[CGUIWidget::ProcessProperty]: unknown text alignment format, <%s>", pProperty->GetValue().c_str() );
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for sound
		/*
		*<property name="OnOpen" type="SOUND" value="3" />
		*/
		else if(  pProperty->GetType()=="SOUND")
		{
			RegisterSound( pProperty->GetName(), CGUIStringConvertor::StringToInt(pProperty->GetValue()));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for script event
		/*
		*<property name="OnOpen" type="EVENT" value="btn_ok_onopen" />
		*/
		else if(  pProperty->GetType()=="EVENT")
		{
			if( pProperty->GetValue().empty())
			{
				UnregisterScriptCallbackFunc(pProperty->GetName());
			}
			else
			{
				RegisterScriptCallbackFunc( pProperty->GetName(), pProperty->GetValue());
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for relative size
		/*
		*<property name="RELATIVE_SIZE" type="BOOL" value="true"/>
		*/
		else if( pProperty->GetName() == "RELATIVE_SIZE" && pProperty->GetType()=="BOOL")
		{
			SetSizeRelative(CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for tag point
		/*
		*<property name="TAG_POINT" type="VECTOR2" value="0.5,0.5"/>
		*/
		else if( pProperty->GetName() == "TAG_POINT" && pProperty->GetType()=="VECTOR2")
		{
			SetTagPoint(CGUIStringConvertor::StringToVector2(pProperty->GetValue()));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for position
		/*
		*<property name="WIDGET_POSITION" type="WIDGET_POSITION">
		*	<property name="RELATIVE" type="BOOL" value="true" />
		*	<property name="POSITION"	type="VECTOR2" value="0,0" />
		*</property>
		*/
		else if( pProperty->GetName() == "WIDGET_POSITION" && pProperty->GetType()=="WIDGET_POSITION")
		{
			const CGUIProperty* pPropertyISRelative = pProperty->GetProperty("RELATIVE");
			if( pPropertyISRelative && CGUIStringConvertor::StringToBool(pPropertyISRelative->GetValue()))
			{
				SetPositionRelative( true );
			}
			else
			{
				SetPositionRelative( false );
			}

			const CGUIProperty* pPropertyPos = pProperty->GetProperty("POSITION");
			if( IsPositionRelative())
			{
				SetPositionRatio( CGUIStringConvertor::StringToVector2(pPropertyPos->GetValue()));
			}
			else
			{
				SetLocalPosition(CGUIStringConvertor::StringToVector2(pPropertyPos->GetValue()));
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for relative pos
		/*
		*<property name="RELATIVE_POS" type="BOOL" value="true"/>
		*/
		else if( pProperty->GetName() == "RELATIVE_POS" && pProperty->GetType()=="BOOL")
		{
			SetPositionRelative(CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for position
		/*
		*<property name="LOCAL_POSITION" type="VECTOR2" value="200,100"/>
		*/
		else if( pProperty->GetName() == "LOCAL_POSITION" && pProperty->GetType()=="VECTOR2")
		{
			this->SetLocalPosition(CGUIStringConvertor::StringToVector2(pProperty->GetValue()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for position
		/*
		*<property name="GLOBAL_POSITION" type="VECTOR2" value="200,100"/>
		*/
		else if( pProperty->GetName() == "GLOBAL_POSITION" && pProperty->GetType()=="VECTOR2")
		{
			this->SetGlobalPosition(CGUIStringConvertor::StringToVector2(pProperty->GetValue()));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for position ration
		/*
		*<property name="POSITION_RATIO" type="VECTOR2" value="0.5,0.5"/>
		*/
		else if( pProperty->GetName() == "POSITION_RATIO" && pProperty->GetType()=="VECTOR2")
		{
			this->SetPositionRatio(CGUIStringConvertor::StringToVector2(pProperty->GetValue()));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for size
		/*
		*<property name="SIZE" type="SIZE" value="800,600" />
		*/
		else if( pProperty->GetName() == "SIZE" && pProperty->GetType()=="SIZE")
		{
			CGUISize aSize = CGUIStringConvertor::StringToSize(pProperty->GetValue());
			if( GUI_REAL_EQUAL(0.0f,aSize.GetWidth()) || GUI_REAL_EQUAL(0.0f,aSize.GetHeight()))
			{
				return;
			}
			this->SetSize(aSize);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for size
		/*
		*<property name="WIDGET_SIZE" type="WIDGET_SIZE">
		*	<property name="RELATIVE" type="BOOL" value="false" />
		*	<property name="SIZE"	type="VECTOR2" value="100,100" />
        *   <property name="MAX_SIZE" type="SIZE" value="0,0"/>
        *   <property name="MIN_SIZE" type="SIZE" value="0,0"/>
		*</property>
		*/
		else if( pProperty->GetName() == "WIDGET_SIZE" && pProperty->GetType()=="WIDGET_SIZE")
		{
			const CGUIProperty* pPropertyISRelative = pProperty->GetProperty("RELATIVE");
			if( pPropertyISRelative && CGUIStringConvertor::StringToBool(pPropertyISRelative->GetValue()))
			{
				SetSizeRelative( true );
			}
			else
			{
				SetSizeRelative( false );
			}

			const CGUIProperty* pPropertySize = pProperty->GetProperty("SIZE");
			if( IsSizeRelative())
			{
				SetSizeRatio( CGUIStringConvertor::StringToSize(pPropertySize->GetValue()));
			}
			else
			{
				SetSize(CGUIStringConvertor::StringToSize(pPropertySize->GetValue()));
			}

			const CGUIProperty* pPropertyMaxSize = pProperty->GetProperty("MAX_SIZE");
			SetMaximumSize(CGUIStringConvertor::StringToSize(pPropertyMaxSize->GetValue()));
			const CGUIProperty* pPropertyMinSize = pProperty->GetProperty("MIN_SIZE");
			SetMinimumSize(CGUIStringConvertor::StringToSize(pPropertyMinSize->GetValue()));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for size ratio
		/*
		*<property name="SIZE_RATIO" type="SIZE" value="0.5,0.5" />
		*/
		else if( pProperty->GetName() == "SIZE_RATIO" && pProperty->GetType()=="SIZE")
		{
			this->SetSizeRatio(CGUIStringConvertor::StringToSize(pProperty->GetValue()));
		}

		/*
		*<property name="MAX_SIZE" type="SIZE" value="80,80" />
		*/
		else if( pProperty->GetName() == "MAX_SIZE" && pProperty->GetType()=="SIZE")
		{
			this->SetMaximumSize(CGUIStringConvertor::StringToSize(pProperty->GetValue()));
		}

		/*
		*<property name="MIN_SIZE" type="SIZE" value="80,80" />
		*/
		else if( pProperty->GetName() == "MIN_SIZE" && pProperty->GetType()=="SIZE")
		{
			this->SetMinimumSize(CGUIStringConvertor::StringToSize(pProperty->GetValue()));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for alpha
		/*
		*<property name="ALPHA" type="REAL" value="0.5" />
		*/
		else if(  pProperty->GetName()=="ALPHA" && pProperty->GetType()=="REAL")
		{
			this->SetSelfAlpha(CGUIStringConvertor::StringToReal(pProperty->GetValue()));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for flag
		/*
		*<property name="MOVABLE" type="BOOL" value="true" />
		*<property name="ACTIVABLE" type="BOOL" value="true" />
		*<property name="EXCLUSIVE" type="BOOL" value="true" />
		*<property name="OPEN_WITH_PARENT" type="BOOL" value="true" />
		*<property name="EVENT_UPDATE" type="BOOL" value="true" />
		*<property name="EVENT_PARENTSIZECHANGE" type="BOOL" value="true" />
		*<property name="EVENT_CLICK" type="BOOL" value="true" />
		*<property name="EVENT_DBCLICK" type="BOOL" value="true" />
		*/
		else if(  pProperty->GetName()=="MOVABLE" && pProperty->GetType()=="BOOL")
		{
			this->SetFlag( eFLAG_MOVABLE, CGUIStringConvertor::StringToBool(pProperty->GetValue()) );
		}
		else if(  pProperty->GetName()=="HITABLE" && pProperty->GetType()=="BOOL")
		{
			this->SetFlag( eFLAG_HITABLE, CGUIStringConvertor::StringToBool(pProperty->GetValue()) );
		}
		else if(  pProperty->GetName()=="ACTIVABLE" && pProperty->GetType()=="BOOL")
		{
			SetSelfActivable( CGUIStringConvertor::StringToBool(pProperty->GetValue()) );
		}
		else if(  pProperty->GetName()=="EXCLUSIVE" && pProperty->GetType()=="BOOL")
		{
			this->SetFlag( eFLAG_EXCLUSIVE, CGUIStringConvertor::StringToBool(pProperty->GetValue()) );
		}
		else if(  pProperty->GetName()=="OPEN_WITH_PARENT" && pProperty->GetType()=="BOOL")
		{
			this->SetFlag( eFLAG_OPEN_WITH_PARENT, CGUIStringConvertor::StringToBool(pProperty->GetValue()) );
		}
		else if(  pProperty->GetName()=="EVENT_UPDATE" && pProperty->GetType()=="BOOL")
		{
			this->SetFlag( eFLAG_EVENT_UPDATE, CGUIStringConvertor::StringToBool(pProperty->GetValue()) );
		}
		else if(  pProperty->GetName()=="EVENT_PARENTSIZECHANGE" && pProperty->GetType()=="BOOL")
		{
			this->SetFlag( eFLAG_EVENT_PARENTSIZECHANGE, CGUIStringConvertor::StringToBool(pProperty->GetValue()) );
		}		
		else if(  pProperty->GetName()=="EVENT_CLICK" && pProperty->GetType()=="BOOL")
		{
			this->SetFlag( eFLAG_EVENT_CLICK, CGUIStringConvertor::StringToBool(pProperty->GetValue()) );
		}		
		else if(  pProperty->GetName()=="EVENT_DBCLICK" && pProperty->GetType()=="BOOL")
		{
			this->SetFlag( eFLAG_EVENT_DBCLICK, CGUIStringConvertor::StringToBool(pProperty->GetValue()) );
		}
		else if( pProperty->GetName() == "DISABLE" && pProperty->GetType() == "BOOL" )
		{
			this->SetSelfDisable(CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
	}
	//------------------------------------------------------------------------------
	void			CGUIWidget::LoadProperty()
	{
		//set parent first
		CGUIProperty* pPropertyParent = m_aPropertySet.GetProperty( "PARENT" );
		if( pPropertyParent )
		{
			ProcessProperty( pPropertyParent );
		}

		uint32 nSize = m_aPropertySet.GetSize();
		for( uint32 nIdx = 0; nIdx<nSize;++nIdx)
		{
			const CGUIProperty* pProperty = m_aPropertySet.GetProperty(nIdx);
			ProcessProperty( pProperty );
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::Render(IGUIInterfaceRender* pRender)
	{
		// don't do anything if window is not visible or closed
		if (IsOpen()==false || !this->IsVisible()) 
		{
			return;
		}

		// perform render for 'this' Window
		RenderSelf(pRender);

		// render any child windows
		CGUIWidget* pWidget = m_pChild;
		while( pWidget )
		{
			pWidget->Render(pRender);
			pWidget = pWidget->m_pNextSibling;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::Update()
	{
		// don't do anything if window is closed
		if (IsOpen()==false ) 
		{
			return;
		}

		// perform update for 'this' Window
		UpdateSelf();

		// render any child windows
		CGUIWidget* pWidget = m_pChild;
		while( pWidget )
		{
			pWidget->Update();
			pWidget = pWidget->m_pNextSibling;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::AddAs( CGUIAs* pAs)
	{
		GUI_ASSERT(pAs->IsCreateByFactory(), "the as isn't created by factory");
		m_listAs.push_back(pAs);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UpdateAs()
	{
		TListAs	listAsBuffer;

		while( !m_listAs.empty())
		{
			//pop as
			CGUIAs *pAs = m_listAs.back();
			m_listAs.pop_back();

			//process event
			if( CGUIWidgetSystem::Instance()->GetGlobalTimer() - pAs->GetTimer() >= static_cast<int32>(pAs->GetDelayTime()))
			{
				pAs->UpdateTimer();
				pAs->Process();
			}
			else
			{
				listAsBuffer.push_back(pAs);
				continue;
			}

			if( pAs->IsRetired())
			{
				//the as has retired, push his successor
				CGUIAs* pSuccessor = NULL;
				while( pSuccessor = pAs->PopSuccessor())
				{
					pSuccessor->GetReceiver()->AddAs(pSuccessor);
				}
				CGUIAsFactory::Instance()->DestroyAs(pAs);
			}
			else
			{
				//process this event again
				listAsBuffer.push_back(pAs);
			}
		}

		//swap list
		m_listAs.swap(listAsBuffer);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RenderSelf(IGUIInterfaceRender* pRender)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UpdateSelf()
	{
		//update action sequence
		UpdateAs();

		//call update event
		if( GetFlag(eFLAG_EVENT_UPDATE))
		{
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_UPDATE);
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}

		//for timer event
		if( !m_aMapTimer.empty())
		{
			TMapTimer::iterator itor = m_aMapTimer.begin();
			TMapTimer::iterator itorEnd = m_aMapTimer.end();
			const CGUITimer& aTimer = CGUIWidgetSystem::Instance()->GetGlobalTimer();
			for( ; itor!=itorEnd; itor++ )
			{
				if( aTimer - itor->second.m_aPrevTime >= static_cast<int32>(itor->second.m_uTimeGap))
				{
					itor->second.m_aPrevTime = aTimer;

					//call function
					CGUIEventTimer aEvent;
					aEvent.SetEventId(eEVENT_TIMER);
					aEvent.SetReceiver(this);
					aEvent.SetTimerName(itor->first);
					CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
				}
			}
		}

	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DrawString(
		IGUIInterfaceRender* pRender, 
		const CGUIStringEx& strText, 
		const CGUIRect& rDrawRect,
		uint8 uTextAlignment,
		const CGUIRect* pClipRect,
		int32 nStartPos,
		int32 nEndPos)
	{
		pRender->GetFontRender()->DrawString(pRender, strText, rDrawRect, uTextAlignment, GetScale(), GetAlpha(), pClipRect, nStartPos, nEndPos);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DrawString(
		IGUIInterfaceRender* pRender, 
		const CGUIStringEx& strText, 
		const CGUIVector2& rPos,
		const CGUIRect* pClipRect,
		int32 nStartPos,
		int32 nEndPos)
	{
		pRender->GetFontRender()->DrawString(pRender, strText,rPos, GetScale(), GetAlpha(), pClipRect, nStartPos,nEndPos);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DrawImage(
		IGUIInterfaceRender* pRender, 
		CGUIImage* pImage, 
		const CGUIRect& rDestRect, 
		real z, 
		const CGUIRect* pClipRect)
	{
		if( pImage )
		{
			pRender->AddScissor(pClipRect?*pClipRect:CGUIWidgetSystem::Instance()->GetScreenRect());
			pImage->Draw( pRender,rDestRect,z,m_aColor,GetAlpha() );
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DrawImage(
		IGUIInterfaceRender* pRender, 
		const CGUIString& rName, 
		const CGUIRect& rDestRect, 
		real z, 
		const CGUIRect* pClipRect)
	{
		TMapImage::iterator itor= m_aMapImage.find(rName);
		if( itor == m_aMapImage.end())
		{
			throw CGUIException("[CGUIWidget::RenderImage]: failed to render image <%s>!", rName.c_str());
			return;
		}
		else
		{
			DrawImage(pRender, itor->second, rDestRect,  z,  pClipRect);
		}
	}
	//------------------------------------------------------------------------------




	//------------------------------------------------------------------------------
	const CGUIRect&		CGUIWidget::GetRect()
	{
		UpdateWidgetRect();
		return m_aWidgetRect;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetRect(const CGUIRect& rRect)
	{
		const CGUISize& rScale = GetScale();

		SetSize( CGUISize(rRect.GetWidth()*rScale.GetWidth(),rRect.GetHeight()*rScale.GetHeight()));
		SetGlobalPosition(CGUIVector2(rRect.m_fLeft+rRect.GetWidth()*GetTagPoint().x, rRect.m_fTop+rRect.GetHeight()*GetTagPoint().y));
	}
	//------------------------------------------------------------------------------
	const CGUIRect&		CGUIWidget::GetClientRect()
	{
		UpdateWidgetRect();
		return m_aClientRect;
	}
	//------------------------------------------------------------------------------
	const CGUIRect&		CGUIWidget::GetClipRect()
	{
		UpdateWidgetRect();
		return m_aClipRect;
	}
	//------------------------------------------------------------------------------
	const CGUIRect&	CGUIWidget::GetClientClipRect()
	{
		UpdateWidgetRect();
		return m_aClientClipRect;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2&	CGUIWidget::GetGlobalPosition()
	{
		UpdateWidgetRect();
		return m_aGlobalPos;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetGlobalPosition(const CGUIVector2&rPos)
	{
		//for local position
		CGUIVector2 aLocalPos = m_pParent?rPos - m_pParent->GetClientRect().GetPosition():rPos;
		SetLocalPosition(aLocalPos);
	}
	//------------------------------------------------------------------------------
	const CGUIVector2&	CGUIWidget::GetLocalPosition()
	{
		UpdateWidgetRect();
		return m_aLocalPos;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetLocalPosition(real x, real y)
	{
		SetLocalPosition(CGUIVector2(x,y));
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetLocalPosition(const CGUIVector2&rPos)
	{
		if( GetFlag(eFLAG_PARENT_CLIENTRECT))
		{	
			if( m_aLocalPos != rPos )
			{
				m_aLocalPos = rPos;
				SetRectDirty();
			}

			//for relative pos
			const CGUISize rParentSize = m_pParent?m_pParent->GetClientRect().GetSize()/m_pParent->GetScale() : CGUIWidgetSystem::Instance()->GetScreenSize();
			CGUIVector2 aRelPos(m_aLocalPos.x/rParentSize.GetWidth(), m_aLocalPos.y/rParentSize.GetHeight());
			if( m_aPosRatio != aRelPos )
			{
				m_aPosRatio = aRelPos;
				SetRectDirty();
			}
		}
		else
		{		
			if( m_aLocalPos != rPos )
			{
				m_aLocalPos = rPos;
				SetRectDirty();
			}

			//for relative pos
			const CGUISize& rParentSize = m_pParent?m_pParent->GetSize() : CGUIWidgetSystem::Instance()->GetScreenSize();
			CGUIVector2 aRelPos(m_aLocalPos.x/rParentSize.GetWidth(), m_aLocalPos.y/rParentSize.GetHeight());
			if( m_aPosRatio != aRelPos )
			{
				m_aPosRatio = aRelPos;
				SetRectDirty();
			}
		}
	}
	//------------------------------------------------------------------------------
	const CGUIVector2&	CGUIWidget::GetPositionRatio()
	{
		return m_aPosRatio;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetPositionRatio(real x, real y)
	{
		SetPositionRatio(CGUIVector2(x,y));
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetPositionRatio(const CGUIVector2&rPos)
	{
		//for relative pos
		if( m_aPosRatio != rPos )
		{
			m_aPosRatio = rPos;
			SetRectDirty();
		}

		//for local position
		CGUIVector2	aLocalPos;
		if( GetFlag(eFLAG_PARENT_CLIENTRECT))
		{
			const CGUISize rParentSize = m_pParent?m_pParent->GetClientRect().GetSize()/m_pParent->GetScale() : CGUIWidgetSystem::Instance()->GetScreenSize();
			aLocalPos.x = rParentSize.GetWidth() * rPos.x; 
			aLocalPos.y = rParentSize.GetHeight() * rPos.y;	
		}
		else
		{
			const CGUISize& rParentSize = m_pParent?m_pParent->GetSize() : CGUIWidgetSystem::Instance()->GetScreenSize();
			aLocalPos.x = rParentSize.GetWidth() * rPos.x; 
			aLocalPos.y = rParentSize.GetHeight() * rPos.y;	
		}

		if( m_aLocalPos != aLocalPos )
		{
			m_aLocalPos = aLocalPos;
			SetRectDirty();
		}
	}
	//------------------------------------------------------------------------------
	const CGUIVector2&	CGUIWidget::GetTagPoint()
	{
		return m_aWidgetTagPoint;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetTagPoint(const CGUIVector2&rTagPoint)
	{
		if( m_aWidgetTagPoint != rTagPoint)
		{
			m_aWidgetTagPoint = rTagPoint;
			SetRectDirty();
		}
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetTagPoint(real x, real y)
	{
		if( m_aWidgetTagPoint.x != x || m_aWidgetTagPoint.y != y )
		{
			m_aWidgetTagPoint.x = x;
			m_aWidgetTagPoint.y = y;
			SetRectDirty();
		}
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetMaximumSize(const CGUISize& rSize)
	{
		m_aMaxSize = rSize;
		SetSize( GetSize());
	}
	//------------------------------------------------------------------------------
	const CGUISize&		CGUIWidget::GetMaximumSize() const			
	{
		return m_aMaxSize;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetMinimumSize(const CGUISize& rSize)
	{
		m_aMinSize = rSize;
		SetSize(GetSize());
	}
	//------------------------------------------------------------------------------
	const CGUISize&		CGUIWidget::GetMinimumSize() const
	{
		return m_aMinSize;
	}
	//------------------------------------------------------------------------------
	const CGUISize&		CGUIWidget::GetSize()
	{
		//update global rect
		if( IsSizeRelative())
		{
			UpdateWidgetRect();
		}
		return m_aWidgetSize;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetSize(const CGUISize&rSize)
	{
		bool bSet = false;

		CGUISize aSize = rSize;
		//check maximum size
		if( !GUI_REAL_EQUAL(0, m_aMaxSize.m_fWidth) && !GUI_REAL_EQUAL(0, m_aMaxSize.m_fHeight))
		{
			if( aSize.m_fWidth > m_aMaxSize.m_fWidth )
			{
				aSize.m_fWidth = m_aMaxSize.m_fWidth;
			}
			if( aSize.m_fHeight > m_aMaxSize.m_fHeight )
			{
				aSize.m_fHeight = m_aMaxSize.m_fHeight;
			}
		}

		//check minimum size
		if( aSize.m_fWidth < m_aMinSize.m_fWidth )
		{
			aSize.m_fWidth = m_aMinSize.m_fWidth;
		}
		if( aSize.m_fHeight < m_aMinSize.m_fHeight )
		{
			aSize.m_fHeight = m_aMinSize.m_fHeight;
		}

		if( aSize.IsEqualZero())
		{
			//ignore it
			return;
		}

		//set size
		if( m_aWidgetSize != aSize )
		{
			m_aWidgetSize = aSize;
			bSet = true;
		}

		//set size ratio
		const CGUISize&	rParentSize = m_pParent ? m_pParent->GetSize():CGUIWidgetSystem::Instance()->GetScreenSize();
		CGUISize aRelSize(1.f,1.f);
		if( GUI_REAL_EQUAL(0,rParentSize.m_fWidth) || GUI_REAL_EQUAL(0,rParentSize.m_fHeight) )
		{
			//parent size is zero, ignore it
			//throw CGUIException("[CGUIWidget::SetSize]: the widget's parent's size is zero. TYPE<%s>  NAME<%s>",GetType().c_str(), GetName().c_str());
		}
		else
		{
			aRelSize.m_fWidth = aSize.m_fWidth / rParentSize.m_fWidth;
			aRelSize.m_fHeight = aSize.m_fHeight / rParentSize.m_fHeight;

		}
		if( m_aSizeRatio != aRelSize )
		{
			m_aSizeRatio = aRelSize;
			bSet = true;
		}

		//send event
		if( bSet )
		{
			SetRectDirty();

			CGUIEventSize aEvent;
			aEvent.SetEventId(eEVENT_CHANGE_SIZE);
			aEvent.SetSize(GetSize());
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent );
		}
	}
	//------------------------------------------------------------------------------
	void		CGUIWidget::SetSize(real width, real height)
	{
		SetSize(CGUISize(width, height));
	}
	//------------------------------------------------------------------------------
	void		CGUIWidget::SetRectSize(const CGUISize&rSize)
	{
		SetSize(rSize / GetScale());
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetRectSize(real width, real height)
	{
		SetRectSize(CGUISize(width, height));
	}
	//------------------------------------------------------------------------------
	const CGUISize&	CGUIWidget::GetSizeRatio()
	{
		return m_aSizeRatio;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetSizeRatio(const CGUISize&rSize)
	{
		bool bSet = false;

		//set size ratio
		if( m_aSizeRatio != rSize )
		{
			m_aSizeRatio = rSize;
			SetRectDirty();
			bSet = true;
		}

		//set size
		CGUISize	aSize = (m_pParent?m_pParent->GetSize() : CGUIWidgetSystem::Instance()->GetScreenSize()) * m_aSizeRatio;
		if( m_aWidgetSize != aSize )
		{
			m_aWidgetSize = aSize;
			SetRectDirty();
			bSet = true;
		}

		//send event
		if( bSet )
		{
			SetRectDirty();

			CGUIEventSize aEvent;
			aEvent.SetEventId(eEVENT_CHANGE_SIZE);
			aEvent.SetSize(GetSize());
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent );
		}
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::SetRectDirty()
	{
		if( !m_bRectDirtyFlag )
		{
			m_bRectDirtyFlag = true;

			CGUIWidget* pWidget = m_pChild;
			while( pWidget )
			{
				pWidget->SetRectDirty();
				pWidget = pWidget->m_pNextSibling;
			}
		}
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::PreUpdateDirtyRect_Imp(const CGUIRect& rParentRect, const CGUISize& rParentSize  )
	{

		//update global rect
		if( IsSizeRelative() && IsPositionRelative())
		{
			//size
			m_aWidgetSize = rParentSize * m_aSizeRatio;

			//pos
			m_aLocalPos.x = rParentSize.GetWidth() * m_aPosRatio.x;
			m_aLocalPos.y = rParentSize.GetHeight() * m_aPosRatio.y;
		}
		else if(!IsSizeRelative() && IsPositionRelative())
		{
			//size
			m_aSizeRatio.m_fWidth = GUI_REAL_EQUAL(0,rParentSize.m_fWidth) ? 0.f : m_aWidgetSize.m_fWidth / rParentSize.m_fWidth;
			m_aSizeRatio.m_fHeight = GUI_REAL_EQUAL(0,rParentSize.m_fHeight) ? 0.f : m_aWidgetSize.m_fHeight / rParentSize.m_fHeight;

			//pos
			m_aLocalPos.x = rParentSize.GetWidth() * m_aPosRatio.x;
			m_aLocalPos.y = rParentSize.GetHeight() * m_aPosRatio.y;
		}
		else //if(!IsSizeRelative() && !IsPositionRelative())
		{
			//size
			//if( GUI_REAL_EQUAL(0,rParentSize.m_fWidth) || GUI_REAL_EQUAL(0,rParentSize.m_fHeight) )
			//{
			//	//parent size is zero, ignore it
			//	throw CGUIException("[CGUIWidget::SetSize]: the widget's parent's size is zero. TYPE<%s>  NAME<%s>",GetType().c_str(), GetName().c_str());
			//}
			//else
			//{
			//	m_aSizeRatio.m_fWidth = m_aWidgetSize.m_fWidth / rParentSize.m_fWidth;
			//	m_aSizeRatio.m_fHeight = m_aWidgetSize.m_fHeight / rParentSize.m_fHeight;
			//}
			m_aSizeRatio.m_fWidth =  GUI_REAL_EQUAL(0,rParentSize.m_fWidth) ? 0.f : m_aWidgetSize.m_fWidth / rParentSize.m_fWidth;
			m_aSizeRatio.m_fHeight = GUI_REAL_EQUAL(0,rParentSize.m_fHeight) ? 0.f : m_aWidgetSize.m_fHeight / rParentSize.m_fHeight;

			//if( GUI_REAL_EQUAL(0,rParentSize.GetWidth()) || GUI_REAL_EQUAL(0,rParentSize.GetHeight()) )
			//{
			//	//parent size is zero, ignore it
			//	throw CGUIException("[CGUIWidget::SetSize]: the widget's parent's size is zero. TYPE<%s>  NAME<%s>",GetType().c_str(), GetName().c_str());
			//}
			//else
			//{
			//	//pos
			//	m_aPosRatio.x = m_aLocalPos.x/rParentSize.GetWidth();
			//	m_aPosRatio.y = m_aLocalPos.y/rParentSize.GetHeight();
			//}
			m_aSizeRatio.m_fWidth =  GUI_REAL_EQUAL(0,rParentSize.m_fWidth) ? 0.f : m_aLocalPos.x/rParentSize.GetWidth();
			m_aSizeRatio.m_fHeight = GUI_REAL_EQUAL(0,rParentSize.m_fHeight) ? 0.f : m_aLocalPos.y/rParentSize.GetHeight();

		}

		//global pos
		m_aGlobalPos = rParentRect.GetPosition() + m_aLocalPos*(m_pParent?m_pParent->GetScale():CGUISize(1.0f,1.0f));

		//global rect
		m_aWidgetRect.SetSize(m_aWidgetSize * GetScale());
		m_aWidgetRect.SetPosition(CGUIVector2(
			m_aGlobalPos.x - m_aWidgetRect.GetSize().m_fWidth*m_aWidgetTagPoint.x,
			m_aGlobalPos.y - m_aWidgetRect.GetSize().m_fHeight*m_aWidgetTagPoint.y));
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::PreUpdateDirtyRect()
	{
		if( GetFlag(eFLAG_PARENT_CLIENTRECT))
		{
			const CGUIRect& rParentRect = m_pParent? m_pParent->GetClientRect() : CGUIWidgetSystem::Instance()->GetScreenRect();
			const CGUISize rParentSize = m_pParent?m_pParent->GetClientRect().GetSize()/*/m_pParent->GetScale()*/ : CGUIWidgetSystem::Instance()->GetScreenSize();
			PreUpdateDirtyRect_Imp(rParentRect, rParentSize);
		}
		else
		{
			const CGUIRect& rParentRect = m_pParent? m_pParent->GetRect() : CGUIWidgetSystem::Instance()->GetScreenRect();
			const CGUISize& rParentSize = m_pParent?m_pParent->GetSize() : CGUIWidgetSystem::Instance()->GetScreenSize();
			PreUpdateDirtyRect_Imp(rParentRect,rParentSize);
		}
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::UpdateDirtyRect()
	{
		//client rect
		m_aClientRect = m_aWidgetRect;

		//clip rect for client
		m_aClientClipRect = m_aClientRect;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidget::PostUpdateDirtyRect()
	{
		if( GetFlag(eFLAG_PARENT_CLIENTRECT))
		{
			const CGUIRect& rParentRect = m_pParent? m_pParent->GetClientRect() : CGUIWidgetSystem::Instance()->GetScreenRect();

			//update clip rect
			const CGUIRect& rParentClipRect = m_pParent?m_pParent->GetClientClipRect():CGUIWidgetSystem::Instance()->GetScreenRect();

			m_aClipRect = rParentClipRect.GetIntersection(m_aWidgetRect);
			m_aClientClipRect = rParentClipRect.GetIntersection(m_aClientClipRect);
		}
		else
		{
			const CGUIRect& rParentRect = m_pParent? m_pParent->GetRect() : CGUIWidgetSystem::Instance()->GetScreenRect();

			//update clip rect
			const CGUIRect& rParentClipRect = m_pParent?m_pParent->GetClipRect():CGUIWidgetSystem::Instance()->GetScreenRect();

			m_aClipRect = rParentClipRect.GetIntersection(m_aWidgetRect);
			m_aClientClipRect = rParentClipRect.GetIntersection(m_aClientClipRect);
		}
	}
	//------------------------------------------------------------------------------
	void		CGUIWidget::UpdateWidgetRect()
	{
		if( IsRectDirty())
		{	
			CGUISize oldSize = m_aWidgetSize;

			//update
			PreUpdateDirtyRect();
			UpdateDirtyRect();
			PostUpdateDirtyRect();

			if( oldSize != m_aWidgetSize)
			{
				CGUIEventSize aEvent;
				aEvent.SetEventId(eEVENT_CHANGE_SIZE);
				aEvent.SetSize(m_aWidgetSize);
				aEvent.SetReceiver(this);
				CGUIWidgetSystem::Instance()->SendEvent( &aEvent );
			}

			ResetRectDirty();
		}
	}
	//------------------------------------------------------------------------------


}//namespace guiex
