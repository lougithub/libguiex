/** 
* @file guianimation.cpp
* @brief animation
* @author ken
* @date 2006-08-10
*/

//============================================================================//
// include 
//============================================================================// 
#include "guianimation.h"
#include "guisystem.h"
#include "guiresource.h"
#include "guitexture.h"
#include "guitexturemanager.h"
#include "guiexception.h"
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guicolor.h"
#include "guiinterfacerender.h"
#include "guirenderrect.h"
#include "guisystem.h"
#include "guianimationmanager.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIAnimation::CGUIAnimation( 
		const CGUIString& rName, 
		const CGUIString& rSceneName, 
		const CGUIString& rFileName, 
		const std::vector<CGUIRect>& rUVRects,
		real fInterval,
		bool bLooping,
		const CGUISize& rSize )
		:CGUIResource( rName, rSceneName, "ANIMATION", GSystem->GetAnimationManager() )
		,m_vecUVRects( rUVRects )
		,m_vecFileNames( rUVRects.size(), rFileName )
		,m_fInterval( fInterval )
		,m_fDeltaTime(0.0f)
		,m_nFrame(0)
		,eUVAnimType(eUVAnimType_SingleFile)
		,m_aAnimationSize(rSize)
		,m_bLooping(bLooping)
	{

	}
	//------------------------------------------------------------------------------
	CGUIAnimation::CGUIAnimation(
		const CGUIString& rName, 
		const CGUIString& rSceneName, 
		const std::vector<CGUIString>& rFileNames,  
		real fInterval,
		bool bLooping,
		const CGUISize& rSize )
		:CGUIResource( rName, rSceneName, "ANIMATION", GSystem->GetAnimationManager() )
		,m_vecUVRects( rFileNames.size(), CGUIRect(0.f,0.f,1.f,1.f))
		,m_vecFileNames( rFileNames )
		,m_fInterval( fInterval )
		,m_fDeltaTime(0.0f)
		,m_nFrame(0)
		,eUVAnimType(eUVAnimType_MultiFile)
		,m_aAnimationSize(rSize)
		,m_bLooping(bLooping)
	{

	}
	//------------------------------------------------------------------------------
	CGUIAnimation::CGUIAnimation(
		const CGUIString& rName, 
		const CGUIString& rSceneName, 
		const std::vector<CGUIString>& rFileNames,  
		const std::vector<CGUIRect>& rUVRects,
		real fInterval,
		bool bLooping,
		const CGUISize& rSize )
		:CGUIResource( rName, rSceneName, "ANIMATION", GSystem->GetAnimationManager() )
		,m_vecUVRects( rUVRects )
		,m_vecFileNames( rFileNames )
		,m_fInterval( fInterval )
		,m_fDeltaTime(0.0f)
		,m_nFrame(0)
		,eUVAnimType(eUVAnimType_MultiFile)
		,m_aAnimationSize(rSize)
		,m_bLooping(bLooping)
	{
		GUI_ASSERT( m_vecFileNames.size() == m_vecUVRects.size(), "invalid param" );
	}
	//------------------------------------------------------------------------------
	CGUIAnimation::~CGUIAnimation()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIAnimation::DoLoad()
	{
		//get full path
		CGUIString strPrePath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName );

		//load texture
		for( uint32 i=0; i<m_vecFileNames.size(); ++i )
		{
			CGUIString strFullPath = strPrePath+m_vecFileNames[i];
			CGUITexture* pTexture = CGUITextureManager::Instance()->CreateTexture(strFullPath);
			if( !pTexture )
			{
				CGUIException::ThrowException("[CGUIAnimation::DoLoad]: failed to create texture from path <%s>",strFullPath.c_str());
				return -1;
			}
			m_vecTextures.push_back( pTexture );
		}

		if( m_vecTextures.empty() )
		{
			CGUIException::ThrowException("[CGUIAnimation::DoLoad]: failed to load texture" );
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::DoUnload()
	{
		for( uint32 i=0; i<m_vecTextures.size(); ++i )
		{
			CGUITextureManager::Instance()->DestroyTexture( m_vecTextures[i] );
		}
		m_vecTextures.clear( );
		m_nFrame = 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::Reset()
	{
		m_nFrame = 0;
		m_fDeltaTime = 0.0f;
	}
	//------------------------------------------------------------------------------
	bool CGUIAnimation::IsPlaying() const
	{
		if( !m_bLooping && !m_vecTextures.empty() && m_nFrame == m_vecTextures.size()-1 )
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::Update( real fDeltaTime )
	{
		Load();

		if( m_bLooping )
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
		else
		{
			if( IsPlaying() )
			{
				if( fDeltaTime < 0.0f )
				{
					fDeltaTime = 0.0f;
				}
				m_fDeltaTime += fDeltaTime;
				while( m_fDeltaTime >= m_fInterval )
				{
					m_fDeltaTime -= m_fInterval;
					m_nFrame ++;
				}
				if( m_nFrame >= m_vecTextures.size() )
				{
					m_nFrame = m_vecTextures.size() - 1;
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	const CGUISize& CGUIAnimation::GetSize()
	{
		if( m_aAnimationSize.IsEqualZero())
		{
			Load();

			if( m_vecTextures.empty())
			{
				m_aAnimationSize.SetValue( 0.0f, 0.0f );
			}
			else
			{
				m_aAnimationSize.SetValue(
					m_vecTextures[0]->GetWidth() * m_vecUVRects[0].GetWidth(),
					m_vecTextures[0]->GetHeight() * m_vecUVRects[0].GetHeight() );
			}
		}
		return m_aAnimationSize;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::Draw( IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect,
		real z, 
		real fAlpha	)
	{
		Load();

		CGUIColor aColor(1.0f,1.0f,1.0f,1.0f);
		aColor.SetAlpha(fAlpha);
		pRender->DrawTile( rDestRect, 
			z,
			m_vecTextures[m_nFrame],
			m_vecUVRects[m_nFrame], 
			eImageOrientation_Normal,
			aColor,
			aColor,
			aColor,
			aColor);
	}
	//------------------------------------------------------------------------------
	bool CGUIAnimation::IsLooping() const
	{
		return m_bLooping;
	}
	//------------------------------------------------------------------------------


}//namespace guiex
