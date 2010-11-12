/** 
* @file guias.cpp
* @brief widget action sequence
* @author ken
* @date 2007-11-21
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guias.h>
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guimath.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 


//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	int32 GUIRegisterAllAs()
	{
		if( 0 != GUI_AS_REGISTER(CGUIAsAlpha)) {return -1;}
		if( 0 != GUI_AS_REGISTER(CGUIAsScale)) {return -1;}
		if( 0 != GUI_AS_REGISTER(CGUIAsPosition)) {return -1;}
		if( 0 != GUI_AS_REGISTER(CGUIAsRotation)) {return -1;}

		return 0;
	} 
 

	//*****************************************************************************
	//	CGUIAs
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAs::CGUIAs(const char* pAsName)
		:m_pReceiver(NULL)
		,m_bRetired(false)
		,m_fTotalTime(0.1f)
		,m_fElapsedTime(0.0f)
		,m_strAsName(pAsName)
		,m_pAsGenerator(NULL)
		,m_bLooping(false)
	{
	}
	//------------------------------------------------------------------------------
	CGUIAs::~CGUIAs()
	{
		for(TListSuccessor::iterator itor = m_listSuccessor.begin();
			itor != m_listSuccessor.end();
			++itor)
		{
			CGUIAsFactory::Instance()->DestroyAs(*itor);
		}
		m_listSuccessor.clear();
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIAs::GetAsName() const
	{
		return m_strAsName;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::Retire( bool bRetired )
	{
		m_bRetired = bRetired;
	}
	//------------------------------------------------------------------------------
	bool CGUIAs::IsRetired()
	{
		return m_bRetired;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::SetLooping( bool bLooping )
	{
		m_bLooping = bLooping;
	}
	//------------------------------------------------------------------------------
	bool CGUIAs::IsLooping()
	{
		return m_bLooping;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::SetReceiver(CGUIWidget* pReceiver)
	{
		m_pReceiver = pReceiver;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIAs::GetReceiver()
	{
		return m_pReceiver;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::SetTotalTime(real fTotalTime)
	{
		if( fTotalTime <= 0.0f )
		{
			fTotalTime = 0.1f;
		}
		m_fTotalTime = fTotalTime;
	}
	//------------------------------------------------------------------------------
	real CGUIAs::GetTotalTime( ) const
	{
		return m_fTotalTime;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::Update( real fDeltaTime )
	{
		m_fElapsedTime += fDeltaTime;
		if( m_fElapsedTime >= m_fTotalTime )
		{
			if( IsLooping() )
			{
				m_fElapsedTime = m_fElapsedTime - m_fTotalTime;
			}
			else
			{
				m_fElapsedTime = m_fTotalTime;
				Retire( true );
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAs::PushSuccessor( CGUIAs* pAs)
	{
		GUI_ASSERT(pAs, "wrong parameter");
		m_listSuccessor.push_back(pAs);
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAs::PopSuccessor()
	{
		if( m_listSuccessor.empty())
		{
			return NULL;
		}
		else
		{
			CGUIAs* pAs = m_listSuccessor.front();
			m_listSuccessor.pop_front();
			return pAs;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAs::SetGenerator( const CGUIAsGenerator* pGenerator)
	{
		m_pAsGenerator = pGenerator;
	}
	//------------------------------------------------------------------------------
	const CGUIAsGenerator* CGUIAs::GetGenerator() const
	{
		return m_pAsGenerator;
	}
	//------------------------------------------------------------------------------




	//*****************************************************************************
	//	CGUIAsAlpha
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsAlpha );
	
	
	//------------------------------------------------------------------------------
	CGUIAsAlpha::CGUIAsAlpha( )
		:CGUIAs("CGUIAsAlpha" )
		,m_fBeginValue(0.0f)
		,m_fEndValue(0.0f)
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsAlpha::SetAlphaSequence(real fBeginValue, real fEndValue, real fTotalTime)
	{	
		m_fBeginValue = fBeginValue;
		m_fEndValue = fEndValue;
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
	void CGUIAsAlpha::Update( real fDeltaTime )
	{
		CGUIAs::Update( fDeltaTime );
		
		real fCurValue = CGUIMath::LinearTween( m_fElapsedTime / m_fTotalTime, m_fBeginValue, m_fEndValue );
		GetReceiver()->SetAlpha(fCurValue);
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsRotation
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsRotation );
	//------------------------------------------------------------------------------
	CGUIAsRotation::CGUIAsRotation()
		:CGUIAs("CGUIAsRotation")
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsRotation::SetRotationSequence(const CGUIVector3& rBeginValue, const CGUIVector3& rEndValue, real fTotalTime)
	{	
		m_vBeginValue = rBeginValue;
		m_vEndValue = rEndValue;
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
	void CGUIAsRotation::Update( real fDeltaTime )
	{
		CGUIAs::Update( fDeltaTime );
		
		CGUIVector3 vCurValue = CGUIMath::LinearTween( m_fElapsedTime / m_fTotalTime, m_vBeginValue, m_vEndValue );
		GetReceiver()->SetRotation(vCurValue);
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsScale
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsScale );
	//------------------------------------------------------------------------------
	CGUIAsScale::CGUIAsScale()
		:CGUIAs("CGUIAsScale")
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsScale::SetScaleSequence(const CGUISize& aBeginValue, const CGUISize& aEndValue, real fTotalTime )
	{	
		m_aBeginValue = aBeginValue;
		m_aEndValue = aEndValue;
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
	void CGUIAsScale::Update( real fDeltaTime )
	{
		CGUIAs::Update( fDeltaTime );

		CGUISize aCurValue = CGUIMath::LinearTween( m_fElapsedTime / m_fTotalTime, m_aBeginValue, m_aEndValue );
		GetReceiver()->SetScale(aCurValue);
	}
	//------------------------------------------------------------------------------

	//*****************************************************************************
	//	CGUIAsPosition
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsPosition );
	//------------------------------------------------------------------------------
	CGUIAsPosition::CGUIAsPosition()
		:CGUIAs("CGUIAsPosition")
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsPosition::SetPositionSequence(const CGUIVector2& aBeginValue, const CGUIVector2& aEndValue, real fTotalTime )
	{	
		m_aBeginValue = aBeginValue;
		m_aEndValue = aEndValue;
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
	void CGUIAsPosition::Update( real fDeltaTime )
	{
		CGUIAs::Update( fDeltaTime );

		CGUIVector2 aCurValue = CGUIMath::LinearTween( m_fElapsedTime / m_fTotalTime, m_aBeginValue, m_aEndValue );
		GetReceiver()->SetPixelPosition(aCurValue);
	}
	//------------------------------------------------------------------------------
}//namespace guiex


