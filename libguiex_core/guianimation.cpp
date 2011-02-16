/** 
* @file guianimation.cpp
* @brief animation
* @author ken
* @date 2006-08-10
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guianimation.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiresource.h>
#include <libguiex_core/guitexture.h>
#include <libguiex_core/guitexturemanager.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiscene.h>
#include <libguiex_core/guiscenemanager.h>
#include <libguiex_core/guicolor.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guirenderrect.h>


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
		const CGUISize& rSize )
		:CGUIResource( rName, rSceneName, "ANIMATION" )
		,m_vecUVRects( rUVRects )
		,m_vecFileNames( rUVRects.size(), rFileName )
		,m_fInterval( fInterval )
		,m_fDeltaTime(0.0f)
		,m_nFrame(0)
		,eUVAnimType(eUVAnimType_SingleFile)
	{

	}
	//------------------------------------------------------------------------------
	CGUIAnimation::CGUIAnimation(
		const CGUIString& rName, 
		const CGUIString& rSceneName, 
		const std::vector<CGUIString>& rFileNames,  
		real fInterval,
		const CGUISize& rSize )
		:CGUIResource( rName, rSceneName, "ANIMATION" )
		,m_vecUVRects( rFileNames.size(), CGUIRect(0.f,0.f,1.f,1.f))
		,m_vecFileNames( rFileNames )
		,m_fInterval( fInterval )
		,m_fDeltaTime(0.0f)
		,m_nFrame(0)
		,eUVAnimType(eUVAnimType_MultiFile)
	{

	}
	//------------------------------------------------------------------------------
	CGUIAnimation::~CGUIAnimation()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIAnimation::DoLoad() const
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
				throw CGUIException("[CGUIAnimation::DoLoad]: failed to create texture from path <%s>",strFullPath.c_str());
				return -1;
			}
			m_vecTextures.push_back( pTexture );
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
	void CGUIAnimation::Update( real fDeltaTime )
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
	const CGUISize& CGUIAnimation::GetSize() const
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
		const CGUIMatrix4& rWorldMatrix,
		const CGUIRect& rDestRect,
		real z, 
		real fAlpha	) const
	{
		Load();

		CGUIColor aColor(1.0f,1.0f,1.0f,1.0f);
		aColor.SetAlpha(fAlpha);
		pRender->DrawTile( rWorldMatrix,
			rDestRect, 
			z,
			m_vecTextures[m_nFrame]->GetTextureImplement(),
			m_vecUVRects[m_nFrame], 
			eImageOrientation_Normal,
			aColor,
			aColor,
			aColor,
			aColor);
	}
	//------------------------------------------------------------------------------


}//namespace guiex
