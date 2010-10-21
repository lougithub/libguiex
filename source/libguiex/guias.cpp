/** 
* @file guias.cpp
* @brief widget action sequence
* @author ken
* @date 2007-11-21
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guias.h>
#include <libguiex_core\guiwidget.h>
#include <libguiex_core\guiwidgetsystem.h>

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
		
		return 0;
	} 
 

	//*****************************************************************************
	//	CGUIAs
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAs::CGUIAs(const char* pAsName, CGUIWidget* pReceiver)
		:m_pReceiver(pReceiver)
		,m_bRetired(false)
		,m_nDelayTime(0)
		,m_strAsName(pAsName)
		,m_pAsGenerator(NULL)
	{
		UpdateTimer();
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
	void		CGUIAs::UpdateTimer()
	{
		m_aTimer = CGUIWidgetSystem::Instance()->GetGlobalTimer();
	}
	//------------------------------------------------------------------------------
	const CGUITimer & CGUIAs::GetTimer() const
	{
		return m_aTimer;
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
	void	CGUIAs::SetDelayTime(uint32 nDelayTime)
	{
		m_nDelayTime = nDelayTime;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIAs::GetDelayTime( ) const
	{
		return m_nDelayTime;
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
		,m_fStepValue(0.0f)
		,m_fCurValue(0.0f)
		,m_nFrames(0)
		,m_nCurFrames(0)
		,m_nDelaytime(0)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIAsAlpha::SetAlphaSequence(real fStartValue, real fEndValue, uint32 nFrames, uint32 nDelayTime)
	{	
		GUI_ASSERT(nFrames>0, "invalid frame count" );

		m_nFrames = nFrames;
		m_nCurFrames = 0;

		m_fStepValue = (fEndValue - fStartValue)/nFrames;

		m_fCurValue = fStartValue-m_fStepValue;

		m_nDelaytime = nDelayTime;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIAsAlpha::Process()
	{
		SetDelayTime(m_nDelaytime);	
		if( m_nCurFrames <= m_nFrames)
		{
			m_fCurValue += m_fStepValue;
			++m_nCurFrames;
			GetReceiver()->SetSelfAlpha(m_fCurValue);
		}
		else
		{
			Retire(true);
		}

		return 0;
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
		,m_nFrames(0)
		,m_nCurFrames(0)
		,m_nDelaytime(0)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIAsScale::SetScaleSequence(const CGUISize& fStartScale, const CGUISize& fEndScale, uint32 nFrames, uint32 nDelayTime)
	{	
		GUI_ASSERT(nFrames>0, "invalid frame count" );

		m_nFrames = nFrames;
		m_nCurFrames = 0;

		m_fStepValue = (fEndScale - fStartScale)/static_cast<real>(nFrames);

		m_fCurValue = fStartScale;

		m_nDelaytime = nDelayTime;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIAsScale::Process()
	{
		SetDelayTime(m_nDelaytime);
		if( m_nCurFrames < m_nFrames)
		{
			m_fCurValue += m_fStepValue;
			++m_nCurFrames;
			GetReceiver()->SetSelfScale(m_fCurValue);
		}
		else
		{
			Retire(true);
		}

		return 0;
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
		,m_nFrames(0)
		,m_nCurFrames(0)
		,m_nDelaytime(0)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIAsPosition::SetPositionSequence(const CGUIVector2& fStartPos, const CGUIVector2& fEndPos, uint32 nFrames, uint32 nDelayTime)
	{	
		GUI_ASSERT(nFrames>0, "invalid frame count" );

		m_nFrames = nFrames;
		m_nCurFrames = 0;

		m_fStepValue = (fEndPos - fStartPos)/(guiex::real)nFrames;

		m_fCurValue = fStartPos;

		m_nDelaytime = nDelayTime;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIAsPosition::Process()
	{
		SetDelayTime(m_nDelaytime);
		if( m_nCurFrames < m_nFrames)
		{
			m_fCurValue += m_fStepValue;
			++m_nCurFrames;
			GetReceiver()->SetLocalPosition(m_fCurValue);
		}
		else
		{
			Retire(true);
		}

		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex


