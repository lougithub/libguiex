/** 
* @file guianimation.cpp
* @brief animation
* @author ken
* @date 2006-08-10
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guianimation.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiresource.h>
#include <libguiex_core\guitexture.h>
#include <libguiex_core\guitexturemanager.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guiprojectinfo.h>
#include <libguiex_core\guiprojectinfomanager.h>
#include <libguiex_core\guicolor.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guirenderrect.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIAnimation::CGUIAnimation( 
		const CGUIString& rName, 
		const CGUIString& rProjectName, 
		const CGUIString& rFileName, 
		const std::vector<CGUIRect>& rUVRects,
		real fInterval )
		:CGUIResource( rName, rProjectName, "ANIMATION" )
		,m_vecUVRects( rUVRects )
		,m_vecFileNames( rUVRects.size(), rFileName )
		,m_nFrame(0)
		,m_fInterval( fInterval )
		,m_fDeltaTime(0.0f)
	{
	}
	//------------------------------------------------------------------------------
	CGUIAnimation::CGUIAnimation(
		const CGUIString& rName, 
		const CGUIString& rProjectName, 
		const std::vector<CGUIString>& rFileNames,  
		real fInterval)
		:CGUIResource( rName, rProjectName, "ANIMATION" )
		,m_vecFileNames( rFileNames )
		,m_vecUVRects( rFileNames.size(), CGUIRect(0.f,0.f,1.f,1.f))
		,m_nFrame(0)
		,m_fInterval( fInterval )
		,m_fDeltaTime(0.0f)
	{

	}
	//------------------------------------------------------------------------------
	CGUIAnimation::~CGUIAnimation()
	{
		Unload();
	}
	//------------------------------------------------------------------------------
	int32	CGUIAnimation::DoLoad()
	{
		//get full path
		CGUIString strPrePath = CGUIProjectInfoManager::Instance()->GetProjectFilePath( m_strProjectName );

		//load texture
		for( uint32 i=0; i<m_vecFileNames.size(); ++i )
		{
			CGUIString strFullPath = strPrePath+m_vecFileNames[i];
			CGUITexture* pTexture = CGUITextureManager::Instance()->CreateTexture(strFullPath);
			if( !pTexture )
			{
				throw CGUIException("[CGUIAnimation::DoLoad]: failed to create texture from path <%s>",strFullPath.c_str());
				return -1;
			}
			m_vecTextures.push_back( pTexture );
		}
		m_nFrame = 0;

		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUIAnimation::DoUnload()
	{
		for( uint32 i=0; i<m_vecTextures.size(); ++i )
		{
			CGUITextureManager::Instance()->DestroyTexture( m_vecTextures[i] );
		}
		m_vecTextures.clear( );
		m_nFrame = 0;
	}
	//------------------------------------------------------------------------------
	void	CGUIAnimation::Update( real fDeltaTime )
	{
		if( fDeltaTime < 0.0f )
		{
			fDeltaTime = 0.0f;
		}
		m_fDeltaTime += fDeltaTime;
		while( m_fDeltaTime >= m_fInterval )
		{
			m_fDeltaTime -= m_fInterval;
			m_nFrame = (m_nFrame+1) % m_vecTextures.size();
		}
	}
	//------------------------------------------------------------------------------
	CGUISize	CGUIAnimation::GetSize()
	{
		if( !IsLoaded())
		{
			Load();
		}

		if( m_vecTextures.empty())
		{
			return CGUISize();
		}
		else
		{
			CGUISize aTexSize = m_vecUVRects[0].GetSize();
			return CGUISize( m_vecTextures[0]->GetWidth() * aTexSize.m_fWidth, m_vecTextures[0]->GetHeight() * aTexSize.m_fHeight);
		}
	}
	//------------------------------------------------------------------------------
	//void	CGUIAnimation::Draw(IGUIInterfaceRender* pRender,
	//	const CGUIRenderRect& rRenderRect,
	//	real z, 
	//	real fAlpha	)
	//{
	//	if( !IsLoaded())
	//	{
	//		Load();
	//	}

	//	CGUIColor aColor(0xFFFFFFFF);
	//	aColor.SetAlpha(fAlpha);
	//	pRender->AddRenderTexture( 
	//		rRenderRect, 
	//		z,
	//		m_vecTextures[m_nFrame]->GetTextureImplement(),
	//		m_vecUVRects[m_nFrame], 
	//		IMAGE_NONE,
	//		aColor.GetARGB(),
	//		aColor.GetARGB(),
	//		aColor.GetARGB(),
	//		aColor.GetARGB());
	//}
	//------------------------------------------------------------------------------
	void	CGUIAnimation::Draw( IGUIInterfaceRender* pRender,
		const CGUIMatrix4& rWorldMatrix,
		const CGUIRect& rDestRect,
		real z, 
		real fAlpha	)
	{
		if( !IsLoaded())
		{
			Load();
		}

		CGUIColor aColor(0xFFFFFFFF);
		aColor.SetAlpha(fAlpha);
		pRender->AddRenderTexture( rWorldMatrix,
			rDestRect, 
			z,
			m_vecTextures[m_nFrame]->GetTextureImplement(),
			m_vecUVRects[m_nFrame], 
			IMAGE_NONE,
			aColor.GetARGB(),
			aColor.GetARGB(),
			aColor.GetARGB(),
			aColor.GetARGB());
	}
	//------------------------------------------------------------------------------


}//namespace guiex
