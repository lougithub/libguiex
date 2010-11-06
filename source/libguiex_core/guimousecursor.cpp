/** 
* @file guimousecursor.cpp
* @brief mouse cursor, used to draw mouse
* @author ken
* @date 2006-07-18
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guimousecursor.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guiimagemanager.h>
#include <libguiex_core\guiimage.h>
#include <libguiex_core\guianimation.h>
#include <libguiex_core\guiwidgetsystem.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUIEXPORT	CGUIMouseCursor*	GetMouseCursor()
	{
		return CGUIMouseCursor::Instance();
	}
	//------------------------------------------------------------------------------
	GUI_SINGLETON_IMPLEMENT_EX(CGUIMouseCursor);
	//------------------------------------------------------------------------------
	CGUIMouseCursor::~CGUIMouseCursor()
	{
		//RemoveAllCursor();
	}
	//------------------------------------------------------------------------------
	CGUIMouseCursor::CGUIMouseCursor()
		:m_pAnimation(NULL)
		,m_bVisible(false)
	{
	}
	//------------------------------------------------------------------------------
	//int32	CGUIMouseCursor::AddCursor(const CGUIString& rCursorName, 
	//								  const CGUIString& rImageName, 
	//								  const CGUIRect& rTexRect,
	//								  const CGUIVector2& rAnchorPoint )
	//{
	//	TMapCursor::iterator itor = m_aMapCursor.find(rCursorName);
	//
	//	CGUIAnimation* pAnimation = NULL;
	//
	//	if( itor != m_aMapCursor.end())
	//	{
	//		//remove old one
	//		itor->second.first->Unload();
	//		pAnimation = itor->second.first;
	//		m_aMapCursor.erase(itor);
	//	}
	//	else
	//	{
	//		pAnimation = new CGUIAnimation;
	//	}
	//
	//	if( 0 != pAnimation->Load(rImageName,rTexRect))
	//	{
	//		//failed
	//		return -1;
	//	}
	//	
	//	m_aMapCursor.insert(std::make_pair(rCursorName, std::make_pair(pAnimation, rAnchorPoint)));
	//	return 0;
	//}
	////------------------------------------------------------------------------------
	//int32	CGUIMouseCursor::AddCursor( const CGUIString& rCursorName, 
	//									const CGUIString& rImageName, 
	//									const std::vector<CGUIRect>& rListTexRect, 
	//									uint32 nInterval,
	//									const CGUIVector2& rAnchorPoint  )
	//{
	//	TMapCursor::iterator itor = m_aMapCursor.find(rCursorName);
	//
	//	CGUIAnimation* pAnimation = NULL;
	//
	//	if( itor != m_aMapCursor.end())
	//	{
	//		//remove old one
	//		itor->second.first->Unload();
	//		pAnimation = itor->second.first;
	//		m_aMapCursor.erase(itor);
	//	}
	//	else
	//	{
	//		pAnimation = new CGUIAnimation;
	//	}
	//
	//	if( 0 != pAnimation->Load(rImageName,rListTexRect, nInterval))
	//	{
	//		//failed
	//		return -1;
	//	}
	//	
	//	m_aMapCursor.insert(std::make_pair(rCursorName, std::make_pair(pAnimation, rAnchorPoint)));
	//	return 0;
	//}
	////------------------------------------------------------------------------------
	//int32	CGUIMouseCursor::AddCursor( const CGUIString& rCursorName, 
	//									  const std::vector<CGUIString>& rListImageName,  
	//									  uint32 nInterval,
	//									  const CGUIVector2& rAnchorPoint)
	//{
	//	TMapCursor::iterator itor = m_aMapCursor.find(rCursorName);
	//
	//	CGUIAnimation* pAnimation = NULL;
	//
	//	if( itor != m_aMapCursor.end())
	//	{
	//		//remove old one
	//		itor->second.first->Unload();
	//		pAnimation = itor->second.first;
	//		m_aMapCursor.erase(itor);
	//	}
	//	else
	//	{
	//		pAnimation = new CGUIAnimation;
	//	}
	//
	//	if( 0 != pAnimation->Load(rListImageName,nInterval))
	//	{
	//		//failed
	//		return -1;
	//	}
	//	
	//	m_aMapCursor.insert(std::make_pair(rCursorName, std::make_pair(pAnimation, rAnchorPoint)));
	//	return 0;
	//}
	//------------------------------------------------------------------------------
	//int32	CGUIMouseCursor::AddCursor( 
	//								   const CGUIString& rCursorName, 
	//								   const CGUIAnimation& aAnimation )
	//{
	//	TMapCursor::iterator itor = m_aMapCursor.find(rCursorName);
	//	if( itor != m_aMapCursor.end())
	//	{
	//		//remove old one
	//		itor->second.first->Unload();
	//		delete itor->second.first;
	//		m_aMapCursor.erase(itor);
	//	}
	//
	//	if( 0 != pAnimation->Load(rListImageName,nInterval))
	//	{
	//		//failed
	//		return -1;
	//	}
	//	
	//	m_aMapCursor.insert(std::make_pair(rCursorName, std::make_pair(pAnimation, rAnchorPoint)));
	//	return 0;	
	//}
	//------------------------------------------------------------------------------
	//void	CGUIMouseCursor::RemoveCursor(const CGUIString& rCursorName)
	//{
	//	TMapCursor::iterator itor = m_aMapCursor.find(rCursorName);
	//	if( itor == m_aMapCursor.end() )
	//	{
	//		//failed to find cursor
	//		throw CGUIException("[CGUIMouseCursor::RemoveCursor]: failed to find cursor <%s>!", rCursorName.c_str());
	//	}
	//	else
	//	{
	//		if( m_pAnimation == itor->second.first)
	//		{
	//			//the cursor which will be removed is current show cursor, so reset it
	//			m_pAnimation = NULL;
	//		}
	//
	//		itor->second.first->Unload();
	//		delete itor->second.first;
	//		m_aMapCursor.erase(itor);
	//	}
	//}
	////------------------------------------------------------------------------------
	//void	CGUIMouseCursor::RemoveAllCursor()
	//{
	//	while(!m_aMapCursor.empty())
	//	{
	//		m_aMapCursor.begin()->second.first->Unload();
	//		delete m_aMapCursor.begin()->second.first;
	//		m_aMapCursor.erase(m_aMapCursor.begin());
	//	}
	//}
	//------------------------------------------------------------------------------
	void	CGUIMouseCursor::SetSelfVisible( bool bVisible)
	{
		m_bVisible = bVisible;
	}
	//------------------------------------------------------------------------------
	bool	CGUIMouseCursor::IsVisible( ) const
	{
		return m_bVisible;
	}
	//------------------------------------------------------------------------------
	void	CGUIMouseCursor::Render(IGUIInterfaceRender* pRender)
	{
		//if( IsVisible())
		//{
		//	if( m_pAnimation)
		//	{
		//		m_pAnimation->Update();
		//		CGUIImage* pImage = m_pAnimation->GetCurrentImageInfo();

		//		CGUISize aSize(pImage->GetSize());
		//		CGUIRect aRenderArea(
		//			CGUIVector2(m_aMousePoint.x-aSize.m_fWidth*m_aCurrentTagPoint.x,m_aMousePoint.y-aSize.m_fHeight*m_aCurrentTagPoint.y),
		//			CGUISize( aSize.m_fWidth, aSize.m_fHeight));

		//		pRender->AddScissor(CGUIWidgetSystem::Instance()->GetScreenRect());
		//		pImage->Draw(pRender, 
		//			aRenderArea,
		//			1.0f,
		//			1.0f);
		//	}
		//}
	}
	//------------------------------------------------------------------------------
	void	CGUIMouseCursor::SetCursor(const CGUIString& rCursorName)
	{
		if( rCursorName == m_strCurrentCursor)
		{
			return;
		}

		//TMapCursor::iterator itor = m_aMapCursor.find(rCursorName);
		//if( itor == m_aMapCursor.end() )
		//{
		//	return;

		//	//failed to find cursor
		//	throw CGUIException("[CGUIMouseCursor::SetCursor]: failed to find cursor <%s>!", rCursorName.c_str());
		//}
		//else
		//{
			m_strCurrentCursor = rCursorName;
		//	m_pAnimation = itor->second.first;
		//	m_aCurrentTagPoint = itor->second.second;
		//}
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIMouseCursor::GetCursor() const
	{
		return m_strCurrentCursor;
	}
	//------------------------------------------------------------------------------
	void	CGUIMouseCursor::SetPosition(const CGUIVector2& rPoint)
	{
		m_aMousePoint = rPoint;
	}
	//------------------------------------------------------------------------------


}//namespace guiex

