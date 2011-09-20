/** 
 * @file guicamera.cpp
 * @brief camera used in system
 * @author ken
 * @date 2010-12-22
 */

//============================================================================//
// include 
//============================================================================// 
#include "guicamera.h"
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
	CGUICamera::CGUICamera()
		:m_bDirty(true)
		,m_vBaseEye( 0,0,-1 )
		,m_vBaseCenter( 0,0,0 )
		,m_vBaseUp( 0,-1,0 )
		,m_fBaseFov( 60.0f )
		,m_fBaseAspectRatio( 1.0f )
		,m_vOffsetEye( 0,0,0 )
		,m_vOffsetCenter( 0,0,0 )
		,m_vOffsetUp( 0,0,0 )
		,m_fOffsetFov( 0.0f )
		,m_fOffsetAspectRatio( 0.0f )
		,m_vEye( 0,0,-1 )
		,m_vCenter( 0,0,0 )
		,m_vUp( 0,-1,0 )
		,m_fFov( 60.0f )
		,m_fAspectRatio( 1.0f )
		,m_fNearPlane( 0.1f )
		,m_fFarPlane( 10000 )
	{
		CGUICameraManager::Instance()->GetOnDefaultValueChangedSignal().connect( this, &CGUICamera::OnCameraDefaultValueChanged );
		Reset();
		Restore();
	}
	//------------------------------------------------------------------------------
	CGUICamera::CGUICamera( const CGUICamera& rOther )
	{
		*this = rOther;
		CGUICameraManager::Instance()->GetOnDefaultValueChangedSignal().connect( this, &CGUICamera::OnCameraDefaultValueChanged );
	}
	//------------------------------------------------------------------------------
	CGUICamera::~CGUICamera()
	{
		CGUICameraManager::Instance()->GetOnDefaultValueChangedSignal().disconnect( this );
	}
	//------------------------------------------------------------------------------
	const CGUICamera& CGUICamera::operator=( const CGUICamera& rOther )
	{
		m_vBaseEye = rOther.m_vBaseEye;
		m_vBaseCenter = rOther.m_vBaseCenter;
		m_vBaseUp = rOther.m_vBaseUp;
		m_fBaseFov = rOther.m_fBaseFov;
		m_fBaseAspectRatio = rOther.m_fBaseAspectRatio;

		m_vOffsetEye = rOther.m_vOffsetEye;
		m_vOffsetCenter = rOther.m_vOffsetCenter;
		m_vOffsetUp = rOther.m_vOffsetUp;
		m_fOffsetFov = rOther.m_fOffsetFov;
		m_fOffsetAspectRatio = rOther.m_fOffsetAspectRatio;

		m_vEye = rOther.m_vEye;
		m_vCenter = rOther.m_vCenter;
		m_vUp = rOther.m_vUp;
		m_fFov = rOther.m_fFov;
		m_fAspectRatio = rOther.m_fAspectRatio;
		m_fNearPlane = rOther.m_fNearPlane;
		m_fFarPlane = rOther.m_fFarPlane;

		m_bDirty = true;
		return *this;
	}
	//------------------------------------------------------------------------------
	/** sets the camera in the default position */
	void CGUICamera::Reset()
	{
		m_vOffsetEye = CGUIVector3::ZERO;
		m_vOffsetCenter = CGUIVector3::ZERO;
		m_vOffsetUp = CGUIVector3::ZERO;
		m_fOffsetFov = 0.0f;
		m_fOffsetAspectRatio = 0.0f;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::Restore()
	{
		CGUICameraManager* pCameraManager = GSystem->GetCameraManager();

		SetBaseEye( pCameraManager->GetDefaultEye() );
		SetBaseCenter( pCameraManager->GetDefaultCenter());
		SetBaseUp( pCameraManager->GetDefaultUp());
		SetBaseFov( pCameraManager->GetDefaultFov());
		SetBaseAspectRatio( pCameraManager->GetDefaultAspectRatio());
		
		m_fNearPlane = pCameraManager->GetDefaultNearPlane();
		m_fFarPlane = pCameraManager->GetDefaultFarPlane();

		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::UpdateEye()
	{
		m_vEye = m_vBaseEye + m_vOffsetEye;
		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::UpdateCenter()
	{
		m_vCenter = m_vBaseCenter + m_vOffsetCenter;
		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::UpdateUp()
	{
		m_vUp = m_vBaseUp + m_vOffsetUp;
		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::UpdateFov()
	{
		m_fFov = m_fBaseFov + m_fOffsetFov;
		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::UpdateAspectRatio()
	{
		m_fAspectRatio = m_fBaseAspectRatio + m_fOffsetAspectRatio;
		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetBaseEye( const CGUIVector3& rEye )
	{
		m_vBaseEye = rEye;
		UpdateEye();
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetBaseCenter( const CGUIVector3& rCenter )
	{
		m_vBaseCenter = rCenter;
		UpdateCenter();
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetBaseUp( const CGUIVector3& rUp )
	{
		m_vBaseUp = rUp;
		UpdateUp();
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetBaseFov( real rFov )
	{
		m_fBaseFov = rFov;
		UpdateFov();
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetBaseAspectRatio( real rRatio )
	{
		m_fBaseAspectRatio = rRatio;
		UpdateAspectRatio();
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetOffsetEye( const CGUIVector3& rEye )
	{
		m_vOffsetEye = rEye;
		UpdateEye();
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetOffsetCenter( const CGUIVector3& rCenter )
	{
		m_vOffsetCenter = rCenter;
		UpdateCenter();
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetOffsetUp( const CGUIVector3& rUp )
	{
		m_vOffsetUp = rUp;
		UpdateUp();
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetOffsetFov( real rFov )
	{
		m_fOffsetFov = rFov;
		UpdateFov();
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetOffsetAspectRatio( real rRatio )
	{
		m_fOffsetAspectRatio = rRatio;
		UpdateAspectRatio();
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICamera::GetBaseEye() const
	{
		return m_vBaseEye;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICamera::GetBaseCenter() const
	{
		return m_vBaseCenter;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICamera::GetBaseUp() const
	{
		return m_vBaseUp;
	}
	//------------------------------------------------------------------------------
	real CGUICamera::GetBaseFov() const
	{
		return m_fBaseFov;
	}
	//------------------------------------------------------------------------------
	real CGUICamera::GetBaseAspectRatio() const
	{
		return m_fBaseAspectRatio;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICamera::GetOffsetEye() const
	{
		return m_vOffsetEye;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICamera::GetOffsetCenter() const
	{
		return m_vOffsetCenter;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICamera::GetOffsetUp() const
	{
		return m_vOffsetUp;
	}
	//------------------------------------------------------------------------------
	real CGUICamera::GetOffsetFov() const
	{
		return m_fOffsetFov;
	}
	//------------------------------------------------------------------------------
	real CGUICamera::GetOffsetAspectRatio() const
	{
		return m_fOffsetAspectRatio;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICamera::GetEye() const
	{
		return m_vEye;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICamera::GetCenter() const
	{
		return m_vCenter;
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUICamera::GetUp() const
	{
		return m_vUp;
	}
	//------------------------------------------------------------------------------
	real CGUICamera::GetFov() const
	{
		return m_fFov;
	}
	//------------------------------------------------------------------------------
	real CGUICamera::GetAspectRatio() const
	{
		return m_fAspectRatio;
	}
	//------------------------------------------------------------------------------
	real CGUICamera::GetNearPlane() const
	{
		return m_fNearPlane;
	}
	//------------------------------------------------------------------------------
	real CGUICamera::GetFarPlane() const
	{
		return m_fFarPlane;
	}
	//------------------------------------------------------------------------------
	bool CGUICamera::IsDirty()
	{
		return m_bDirty;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetDirty()
	{
		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::ClearDirty()
	{
		m_bDirty = false;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::OnCameraDefaultValueChanged( )
	{
		Restore();
	}
	//------------------------------------------------------------------------------
}
