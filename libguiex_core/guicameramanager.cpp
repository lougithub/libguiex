/** 
 * @file guicameramanager.cpp
 * @brief camera manager used in system
 * @author ken
 * @date 2010-12-22
 */


//============================================================================//
// include
//============================================================================// 
#include "guicameramanager.h"
#include "guisystem.h"
#include "guimath.h"
#include "guiintsize.h"
#include "guiexception.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUICameraManager * CGUICameraManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUICameraManager::CGUICameraManager()
	:m_vDefaultEye( 0.0f,0.0f, -1.0f )
	,m_vDefaultCenter( 0.0f,0.0f, 0.0f )
	,m_vDefaultUp( 0.0f,-1.0f,0.0f )
	,m_fDefaultFov(60.0f)
	,m_fDefaultAspectRatio(1.0f)
	,m_fDefaultNearPlane(0.1f)
	,m_fDefaultFarPlane(10000.0f)
	{
		GUI_ASSERT( !m_pSingleton, "[CGUICameraManager::CGUICameraManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUICameraManager::~CGUICameraManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUICameraManager* CGUICameraManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	void CGUICameraManager::SetCameraDefaultValue( const CGUIIntSize& rRawScreenSize, EScreenOrientation eScreenOrientation )
	{
		m_fDefaultFov = 60.0f;
		if( rRawScreenSize.GetHeight() == 0 )
		{
			m_fDefaultAspectRatio = 1;
		}
		else
		{
			m_fDefaultAspectRatio = rRawScreenSize.GetWidth() / real( rRawScreenSize.GetHeight());
		}
		
		real fZDistance = -(rRawScreenSize.GetHeight()/2.0f) / CGUIMath::Tan( CGUIDegree(m_fDefaultFov/2));
		
		switch( eScreenOrientation )
		{
			case eScreenOrientation_Portrait:
				m_vDefaultCenter.x = rRawScreenSize.GetWidth() / 2.0f;
				m_vDefaultCenter.y = rRawScreenSize.GetHeight() / 2.0f;
				m_vDefaultCenter.z = 0.0f;
				
				m_vDefaultEye.x = m_vDefaultCenter.x;
				m_vDefaultEye.y = m_vDefaultCenter.y;
				m_vDefaultEye.z = fZDistance;
				
				m_vDefaultUp.x = 0.0f;
				m_vDefaultUp.y = -1.0f;
				m_vDefaultUp.z = 0.0f;
				break;
			case eScreenOrientation_PortraitUpsideDown:
				m_vDefaultCenter.x = rRawScreenSize.GetWidth() / 2.0f;
				m_vDefaultCenter.y = rRawScreenSize.GetHeight() / 2.0f;
				m_vDefaultCenter.z = 0.0f;
				
				m_vDefaultEye.x = m_vDefaultCenter.x;
				m_vDefaultEye.y = m_vDefaultCenter.y;
				m_vDefaultEye.z = fZDistance;
				
				m_vDefaultUp.x = 0.0f;
				m_vDefaultUp.y = 1.0f;
				m_vDefaultUp.z = 0.0f;
				break;
			case eScreenOrientation_LandscapeLeft:
				m_vDefaultCenter.x = rRawScreenSize.GetHeight() / 2.0f;
				m_vDefaultCenter.y = rRawScreenSize.GetWidth() / 2.0f;
				m_vDefaultCenter.z = 0.0f;
				
				m_vDefaultEye.x = m_vDefaultCenter.x;
				m_vDefaultEye.y = m_vDefaultCenter.y;
				m_vDefaultEye.z = fZDistance;
				
				m_vDefaultUp.x = -1.0f;
				m_vDefaultUp.y = 0.0f;
				m_vDefaultUp.z = 0.0f;
				break;
				
			case eScreenOrientation_LandscapeRight:
				m_vDefaultCenter.x = rRawScreenSize.GetHeight() / 2.0f;
				m_vDefaultCenter.y = rRawScreenSize.GetWidth() / 2.0f;
				m_vDefaultCenter.z = 0.0f;
				
				m_vDefaultEye.x = m_vDefaultCenter.x;
				m_vDefaultEye.y = m_vDefaultCenter.y;
				m_vDefaultEye.z = fZDistance;
				
				m_vDefaultUp.x = 1.0f;
				m_vDefaultUp.y = 0.0f;
				m_vDefaultUp.z = 0.0f;
				
				break;
				
			default:
				throw CGUIException("CGUICamera::SetDefaultValue: unknown screen orientation.");
				break;
		}
		
		m_fDefaultNearPlane = 0.1f;
		m_fDefaultFarPlane = 10000.0f;
		
		OnDefaultValueChanged();
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICameraManager::GetDefaultEye() const
	{
		return m_vDefaultEye;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICameraManager::GetDefaultCenter() const
	{
		return m_vDefaultCenter;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICameraManager::GetDefaultUp() const
	{
		return m_vDefaultUp;
	}
	//------------------------------------------------------------------------------
	real CGUICameraManager::GetDefaultFov() const
	{
		return m_fDefaultFov;
	}
	//------------------------------------------------------------------------------
	real CGUICameraManager::GetDefaultAspectRatio() const
	{
		return m_fDefaultAspectRatio;
	}
	//------------------------------------------------------------------------------
	real CGUICameraManager::GetDefaultNearPlane() const
	{
		return m_fDefaultNearPlane;
	}
	//------------------------------------------------------------------------------
	real CGUICameraManager::GetDefaultFarPlane() const
	{
		return m_fDefaultFarPlane;
	}
	//------------------------------------------------------------------------------
	sigslot::signal0<>& CGUICameraManager::GetOnDefaultValueChangedSignal()
	{
		return OnDefaultValueChanged;
	}
	//------------------------------------------------------------------------------
	
}
