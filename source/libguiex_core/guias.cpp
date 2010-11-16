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
		if( 0 != GUI_AS_REGISTER(CGUIAsColor)) {return -1;}
		if( 0 != GUI_AS_REGISTER(CGUIAsContainer)) {return -1;}

		return 0;
	} 
 

	//*****************************************************************************
	//	CGUIAs
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAs::CGUIAs( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIResource( rAsName, rSceneName, "AS" )
		,m_pReceiver(NULL)
		,m_bRetired(false)
		,m_fTotalTime(0.1f)
		,m_fElapsedTime(0.0f)
		,m_strAsType(rAsType)
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

		Unload();
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIAs::GetType() const
	{
		return m_strAsType;
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
	int32 CGUIAs::DoLoad() const
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::DoUnload()
	{

	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsAlpha
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsAlpha );
	
	
	//------------------------------------------------------------------------------
	CGUIAsAlpha::CGUIAsAlpha( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsLinearBase<real>("CGUIAsAlpha" , rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsAlpha::Update( real fDeltaTime )
	{
		CGUIAsLinearBase<real>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetAlpha(GetCurrentValue());
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsRotation
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsRotation );
	//------------------------------------------------------------------------------
	CGUIAsRotation::CGUIAsRotation(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsLinearBase<CGUIVector3>("CGUIAsRotation", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsRotation::Update( real fDeltaTime )
	{
		CGUIAsLinearBase<CGUIVector3>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetRotation(GetCurrentValue());
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsScale
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsScale );
	//------------------------------------------------------------------------------
	CGUIAsScale::CGUIAsScale(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsLinearBase<CGUISize>("CGUIAsScale", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsScale::Update( real fDeltaTime )
	{
		CGUIAsLinearBase<CGUISize>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetScale(GetCurrentValue());
	}
	//------------------------------------------------------------------------------

	//*****************************************************************************
	//	CGUIAsPosition
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsPosition );
	//------------------------------------------------------------------------------
	CGUIAsPosition::CGUIAsPosition(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsLinearBase<CGUIVector2>("CGUIAsPosition", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsPosition::Update( real fDeltaTime )
	{
		CGUIAsLinearBase<CGUIVector2>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetPixelPosition(GetCurrentValue());
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsColor
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsColor );
	//------------------------------------------------------------------------------
	CGUIAsColor::CGUIAsColor(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsLinearBase<CGUIColor>("CGUIAsColor", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsColor::Update( real fDeltaTime )
	{
		CGUIAsLinearBase<CGUIColor>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetColor(GetCurrentValue());
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsContainer
	//*****************************************************************************
	//------------------------------------------------------------------------------
	GUI_AS_GENERATOR_IMPLEMENT( CGUIAsContainer );
	//------------------------------------------------------------------------------
	CGUIAsContainer::CGUIAsContainer(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAs("CGUIAsContainer", rAsName, rSceneName)
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsContainer::SetReceiver(CGUIWidget* pReceiver)
	{
		CGUIAs::SetReceiver( pReceiver );

		for( std::vector<SAsInfo>::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			SAsInfo& rInfo = *itor;
			rInfo.m_pAs->SetReceiver( GetReceiver() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAsContainer::Update( real fDeltaTime )
	{
		CGUIAs::Update( fDeltaTime );

		for( std::vector<SAsInfo>::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			const SAsInfo& rInfo = *itor;
			if( rInfo.m_fBeginTime > m_fElapsedTime )
			{
				//update done
				break;
			}

			if( rInfo.m_fBeginTime + rInfo.m_pAs->GetTotalTime() < m_fElapsedTime )
			{
				continue;
			}

			rInfo.m_pAs->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAsContainer::AddAs( CGUIAs* pAs, real fBeginTime )
	{
		SAsInfo aAsInfo;
		aAsInfo.m_fBeginTime = fBeginTime;
		aAsInfo.m_pAs = pAs;
		aAsInfo.m_pAs->SetReceiver( GetReceiver() );


		//insert
		bool bInserted = false;
		for( std::vector<SAsInfo>::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			const SAsInfo& rInfo = *itor;
			if( rInfo.m_fBeginTime >= fBeginTime )
			{
				m_vAsList.insert( itor, rInfo );
				bInserted = true;
				break;
			}
		}
		if( !bInserted )
		{
			//last one
			m_vAsList.push_back( aAsInfo );
		}

		//update time
		real fTotalTime = 0.0f;
		for( std::vector<SAsInfo>::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			const SAsInfo& rInfo = *itor;
			real fAsEndTime = rInfo.m_fBeginTime + rInfo.m_pAs->GetTotalTime();
			if( fAsEndTime > fTotalTime )
			{
				fTotalTime = fAsEndTime;
			}
		}
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
}//namespace guiex


