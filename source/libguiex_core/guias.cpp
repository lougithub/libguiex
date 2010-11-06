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
	CGUIAs::CGUIAs(const char* pAsName, CGUIWidget* pReceiver)
		:m_pReceiver(pReceiver)
		,m_bRetired(false)
		,m_fTotalTime(0.1f)
		,m_fElapsedTime(0.0f)
		,m_strAsName(pAsName)
		,m_pAsGenerator(NULL)
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
	const CGUIString&		CGUIAs::GetAsName() const
	{
		return m_strAsName;
	}
	//------------------------------------------------------------------------------
	void	CGUIAs::Retire( bool bRetired )
	{
		m_bRetired = bRetired;
	}
	//------------------------------------------------------------------------------
	bool	CGUIAs::IsRetired()
	{
		return m_bRetired;
	}
	//------------------------------------------------------------------------------
	void	CGUIAs::SetReceiver(CGUIWidget* pReceiver)
	{
		m_pReceiver = pReceiver;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIAs::GetReceiver()
	{
		return m_pReceiver;
	}
	//------------------------------------------------------------------------------
	void	CGUIAs::SetTotalTime(real fTotalTime)
	{
		if( fTotalTime <= 0.0f )
		{
			fTotalTime = 0.1f;
		}
		m_fTotalTime = fTotalTime;
	}
	//------------------------------------------------------------------------------
	real	CGUIAs::GetTotalTime( ) const
	{
		return m_fTotalTime;
	}
	//------------------------------------------------------------------------------
	void	CGUIAs::Update( real fDeltaTime )
	{
		m_fElapsedTime += fDeltaTime;
		if( m_fElapsedTime >= m_fTotalTime )
		{
			m_fElapsedTime = m_fTotalTime;
			Retire( true );
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIAs::PushSuccessor( CGUIAs* pAs)
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
	void	CGUIAs::SetGenerator( const CGUIAsGenerator* pGenerator)
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
	CGUIAsAlpha::CGUIAsAlpha(CGUIWidget* pReceiver)
		:CGUIAs("CGUIAsAlpha", pReceiver)
		,m_fBeginValue(0.0f)
		,m_fEndValue(0.0f)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIAsAlpha::SetAlphaSequence(real fBeginValue, real fEndValue, real fTotalTime)
	{	
		m_fBeginValue = fBeginValue;
		m_fEndValue = fEndValue;
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
	void	CGUIAsAlpha::Update( real fDeltaTime )
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
	CGUIAsRotation::CGUIAsRotation(CGUIWidget* pReceiver)
		:CGUIAs("CGUIAsRotation", pReceiver)
		,m_fBeginValue(0.0f)
		,m_fEndValue(0.0f)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIAsRotation::SetRotationSequence(real fBeginValue, real fEndValue, real fTotalTime)
	{	
		m_fBeginValue = fBeginValue;
		m_fEndValue = fEndValue;
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
	void	CGUIAsRotation::Update( real fDeltaTime )
	{
		CGUIAs::Update( fDeltaTime );
		
		real fCurValue = CGUIMath::LinearTween( m_fElapsedTime / m_fTotalTime, m_fBeginValue, m_fEndValue );
		GetReceiver()->SetRotation(fCurValue);
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsScale
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsScale );
	//------------------------------------------------------------------------------
	CGUIAsScale::CGUIAsScale(CGUIWidget* pReceiver)
		:CGUIAs("CGUIAsScale", pReceiver)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIAsScale::SetScaleSequence(const CGUISize& aBeginValue, const CGUISize& aEndValue, real fTotalTime )
	{	
		m_aBeginValue = aBeginValue;
		m_aEndValue = aEndValue;
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
	void	CGUIAsScale::Update( real fDeltaTime )
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
	CGUIAsPosition::CGUIAsPosition(CGUIWidget* pReceiver)
		:CGUIAs("CGUIAsPosition", pReceiver)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIAsPosition::SetPositionSequence(const CGUIVector2& aBeginValue, const CGUIVector2& aEndValue, real fTotalTime )
	{	
		m_aBeginValue = aBeginValue;
		m_aEndValue = aEndValue;
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
	void	CGUIAsPosition::Update( real fDeltaTime )
	{
		CGUIAs::Update( fDeltaTime );

		CGUIVector2 aCurValue = CGUIMath::LinearTween( m_fElapsedTime / m_fTotalTime, m_aBeginValue, m_aEndValue );
		GetReceiver()->SetPixelPosition(aCurValue);
	}
	//------------------------------------------------------------------------------
}//namespace guiex


