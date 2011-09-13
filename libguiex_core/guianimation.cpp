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
		,m_fElapsedTime(0.0f)
		,m_nCurrentFrame(0)
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

		GUI_ASSERT( !m_vecImages.empty(),"CGUIAnimation::DoLoad: no animation data")

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::DoUnload()
	{
		for( TImageArray::iterator itor = m_vecImages.begin(); itor != m_vecImages.end(); ++itor )
		{
			(*itor)->Unload();
		}
		m_nCurrentFrame = 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::Reset()
	{
		m_nCurrentFrame = 0;
		m_fElapsedTime = 0.0f;
	}
	//------------------------------------------------------------------------------
	bool CGUIAnimation::IsFinished() const
	{
		if( !m_bLooping && !m_vecImages.empty() && m_nCurrentFrame == m_vecImages.size()-1 )
		{
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------------------
	/** 
	* @return the time which is not used to update this animatin. 
	return 0.0f if it's looping animation.
	**/
	real CGUIAnimation::Update( real fDeltaTime )
	{
		Load();

		if( fDeltaTime < 0.0f )
		{
			fDeltaTime = 0.0f;
		}

		if( m_bLooping )
		{
			m_fElapsedTime += fDeltaTime;
			m_nCurrentFrame = GUI_FLOAT2UINT_ROUND( floor( m_fElapsedTime / m_fInterval ));
			uint32 uRatio = m_nCurrentFrame / m_vecImages.size();
			m_nCurrentFrame %= m_vecImages.size();
			m_fElapsedTime -= ( uRatio * GetLength() );

			return 0.0f;
		}
		else
		{
			if( IsFinished() )
			{
				return fDeltaTime;
			}
			else
			{
				m_fElapsedTime += fDeltaTime;
				m_nCurrentFrame = GUI_FLOAT2UINT_ROUND( floor( m_fElapsedTime / m_fInterval ));
				real fLeftTime = 0.0f;
				if( m_nCurrentFrame >= m_vecImages.size()-1 )
				{
					m_nCurrentFrame = m_vecImages.size() - 1;
					real fTotalTime = GetLength();
					fLeftTime = m_fElapsedTime - fTotalTime;
					m_fElapsedTime = fTotalTime;
				}
				return fLeftTime;
			}
		}
	}
	//------------------------------------------------------------------------------
	real CGUIAnimation::GetLength() const
	{
		return m_fInterval * m_vecImages.size();
	}
	//------------------------------------------------------------------------------
	real CGUIAnimation::GetPercent() const
	{
		return m_fElapsedTime / GetLength();
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::SetPercent( real fPercent )
	{
		m_fElapsedTime = fPercent * GetLength();
		m_nCurrentFrame = GUI_FLOAT2UINT_ROUND( floor( m_fElapsedTime / m_fInterval ));
		if( IsLooping() )
		{
			uint32 uRatio = m_nCurrentFrame / m_vecImages.size();
			m_nCurrentFrame %= m_vecImages.size();
			m_fElapsedTime -= ( uRatio * GetLength() );
		}
		else
		{
			if( m_nCurrentFrame >= m_vecImages.size()-1 )
			{
				m_nCurrentFrame = m_vecImages.size() - 1;
				m_fElapsedTime = GetLength();
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
		m_vecImages[m_nCurrentFrame]->Draw( pRender, rDestRect, z, fAlpha );
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::Draw( IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect,
		real z, 
		const CGUIColor& rColor,
		real fAlpha	)
	{
		if( m_vecImages.empty() )
		{
			return;
		}
		Load();
		m_vecImages[m_nCurrentFrame]->Draw( pRender, rDestRect, z, rColor, fAlpha );
	}
	//------------------------------------------------------------------------------
	void CGUIAnimation::SetLooping( bool bLoop )	
	{
		m_bLooping = bLoop;
	}
	//------------------------------------------------------------------------------
	bool CGUIAnimation::IsLooping() const
	{
		return m_bLooping;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIAnimation::GetImageNum() const
	{
		return static_cast<uint32>(m_vecImages.size());
	}
	//------------------------------------------------------------------------------
	const CGUIImage* CGUIAnimation::GetImage( uint32 uIndex )
	{
		GUI_ASSERT( uIndex < GetImageNum(), "[CGUIAnimation::GetImage]: invalid index");
		return m_vecImages[uIndex];
	}
	//------------------------------------------------------------------------------

}//namespace guiex
