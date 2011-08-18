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
#include "guiexception.h"
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guiinterfacerender.h"
#include "guisystem.h"
#include "guianimationmanager.h"
#include "guiimagemanager.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	CGUIAnimation::CGUIAnimation(
		const CGUIString& rName, 
		const CGUIString& rSceneName, 
		const std::vector<CGUIString>& rImageNames,  
		real fInterval,
		bool bLooping,
		const CGUISize& rSize )
		:CGUIResource( rName, rSceneName, "ANIMATION", GSystem->GetAnimationManager() )
		,m_fInterval( fInterval )
		,m_fDeltaTime(0.0f)
		,m_nFrame(0)
		,m_aAnimationSize(rSize)
		,m_bLooping(bLooping)
	{
		m_vecImages.resize( rImageNames.size(), NULL );
		for( uint32 i = 0; i<rImageNames.size(); ++i )
		{
			CGUIImage* pImage = CGUIImageManager::Instance()->AllocateResource( rImageNames[i] );
			m_vecImages[i] = pImage;
		}
	}
	//------------------------------------------------------------------------------
	CGUIAnimation::~CGUIAnimation()
	{
		for( TImageArray::iterator itor = m_vecImages.begin(); itor != m_vecImages.end(); ++itor )
		{
			(*itor)->RefRelease();
		}
		m_vecImages.clear();
	}
	//------------------------------------------------------------------------------
	int32 CGUIAnimation::DoLoad()
	{
		for( TImageArray::iterator itor = m_vecImages.begin(); itor != m_vecImages.end(); ++itor )
		{
			(*itor)->Load();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::DoUnload()
	{
		for( TImageArray::iterator itor = m_vecImages.begin(); itor != m_vecImages.end(); ++itor )
		{
			(*itor)->Unload();
		}
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
		if( !m_bLooping && !m_vecImages.empty() && m_nFrame == m_vecImages.size()-1 )
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
				m_nFrame = (m_nFrame+1) % m_vecImages.size();
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
				if( m_nFrame >= m_vecImages.size() )
				{
					m_nFrame = m_vecImages.size() - 1;
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

			if( m_vecImages.empty())
			{
				m_aAnimationSize.SetValue( 0.0f, 0.0f );
			}
			else
			{
				m_aAnimationSize = m_vecImages[0]->GetSize();
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
		if( m_vecImages.empty() )
		{
			return;
		}
		Load();
		m_vecImages[m_nFrame]->Draw( pRender, rDestRect, z, fAlpha );
	}
	//------------------------------------------------------------------------------
	bool CGUIAnimation::IsLooping() const
	{
		return m_bLooping;
	}
	//------------------------------------------------------------------------------


}//namespace guiex
