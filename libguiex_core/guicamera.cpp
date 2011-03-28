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
#include "guisystem.h"
#include "guimath.h"
#include "guiintsize.h"
#include "guiexception.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	CGUIVector3 CGUICamera::ms_vDefaultEye = CGUIVector3( 0.0f,0.0f, -1.0f );
	CGUIVector3 CGUICamera::ms_vDefaultCenter = CGUIVector3( 0.0f,0.0f, 0.0f );
	CGUIVector3 CGUICamera::ms_vDefaultUp = CGUIVector3( 0.0f,-1.0f,0.0f );
	real CGUICamera::ms_fDefaultFov = 60.0f;
	real CGUICamera::ms_fDefaultAspectRatio = 1.0f;
	real CGUICamera::ms_fDefaultNearPlane = 0.1f;
	real CGUICamera::ms_fDefaultFarPlane = 10000;
	//------------------------------------------------------------------------------
	CGUICamera::CGUICamera()
		:m_bDirty(true)
		,m_vEye( 0,0,-1 )
		,m_vCenter( 0,0,0 )
		,m_vUp( 0,-1,0 )
		,m_fFov( 60.0f )
		,m_fAspectRatio( 1.0f )
		,m_fNearPlane( 0.1f )
		,m_fFarPlane( 10000 )
	{

	}
	//------------------------------------------------------------------------------
	CGUICamera::CGUICamera( const CGUICamera& rOther )
	{
		*this = rOther;
	}
	//------------------------------------------------------------------------------
	const CGUICamera& CGUICamera::operator=( const CGUICamera& rOther )
	{
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
	void CGUICamera::SetDefaultValue( const CGUIIntSize& rRawScreenSize, EScreenOrientation eScreenOrientation )
	{
		ms_fDefaultFov = 60.0f;
		if( rRawScreenSize.GetHeight() == 0 )
		{
			ms_fDefaultAspectRatio = 1;
		}
		else
		{
			ms_fDefaultAspectRatio = rRawScreenSize.GetWidth() / real( rRawScreenSize.GetHeight());
		}

		real fZDistance = -(rRawScreenSize.GetHeight()/2.0f) / CGUIMath::Tan( CGUIDegree(ms_fDefaultFov/2));

		switch( eScreenOrientation )
		{
		case eDeviceOrientation_Portrait:
			ms_vDefaultCenter.x = rRawScreenSize.GetWidth() / 2.0f;
			ms_vDefaultCenter.y = rRawScreenSize.GetHeight() / 2.0f;
			ms_vDefaultCenter.z = 0.0f;

			ms_vDefaultEye.x = ms_vDefaultCenter.x;
			ms_vDefaultEye.y = ms_vDefaultCenter.y;
			ms_vDefaultEye.z = fZDistance;

			ms_vDefaultUp.x = 0.0f;
			ms_vDefaultUp.y = -1.0f;
			ms_vDefaultUp.z = 0.0f;
			break;
		case eDeviceOrientation_PortraitUpsideDown:
			ms_vDefaultCenter.x = rRawScreenSize.GetWidth() / 2.0f;
			ms_vDefaultCenter.y = rRawScreenSize.GetHeight() / 2.0f;
			ms_vDefaultCenter.z = 0.0f;

			ms_vDefaultEye.x = ms_vDefaultCenter.x;
			ms_vDefaultEye.y = ms_vDefaultCenter.y;
			ms_vDefaultEye.z = fZDistance;

			ms_vDefaultUp.x = 0.0f;
			ms_vDefaultUp.y = 1.0f;
			ms_vDefaultUp.z = 0.0f;
			break;
		case eDeviceOrientation_LandscapeLeft:
			ms_vDefaultCenter.x = rRawScreenSize.GetHeight() / 2.0f;
			ms_vDefaultCenter.y = rRawScreenSize.GetWidth() / 2.0f;
			ms_vDefaultCenter.z = 0.0f;

			ms_vDefaultEye.x = ms_vDefaultCenter.x;
			ms_vDefaultEye.y = ms_vDefaultCenter.y;
			ms_vDefaultEye.z = fZDistance;

			ms_vDefaultUp.x = 1.0f;
			ms_vDefaultUp.y = 0.0f;
			ms_vDefaultUp.z = 0.0f;
			break;
		case eDeviceOrientation_LandscapeRight:
			ms_vDefaultCenter.x = rRawScreenSize.GetHeight() / 2.0f;
			ms_vDefaultCenter.y = rRawScreenSize.GetWidth() / 2.0f;
			ms_vDefaultCenter.z = 0.0f;

			ms_vDefaultEye.x = ms_vDefaultCenter.x;
			ms_vDefaultEye.y = ms_vDefaultCenter.y;
			ms_vDefaultEye.z = fZDistance;

			ms_vDefaultUp.x = -1.0f;
			ms_vDefaultUp.y = 0.0f;
			ms_vDefaultUp.z = 0.0f;
			break;

		default:
			throw CGUIException("CGUICamera::SetDefaultValue: unknown screen orientation.");
			break;
		}

		ms_fDefaultNearPlane = 0.1f;
		ms_fDefaultFarPlane = 10000.0f;
	}
	//------------------------------------------------------------------------------
	/** sets the camera in the default position */
	void CGUICamera::Restore()
	{
		m_vEye = ms_vDefaultEye;
		m_vCenter = ms_vDefaultCenter;
		m_vUp = ms_vDefaultUp;

		m_fFov = ms_fDefaultFov;
		m_fAspectRatio = ms_fDefaultAspectRatio;
		m_fNearPlane = ms_fDefaultNearPlane;
		m_fFarPlane = ms_fDefaultFarPlane;

		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	/** sets the eye values in points */
	void CGUICamera::SetEye( const CGUIVector3& rEye )
	{
		SetEye( rEye.x, rEye.y, rEye.z );
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetEye( real eyeX, real eyeY, real eyeZ )
	{
		m_vEye.x = eyeX;
		m_vEye.y = eyeY;
		m_vEye.z = eyeZ;

		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	/** sets the center values in points */
	void CGUICamera::SetCenter( const CGUIVector3& rCenter )
	{
		SetCenter( rCenter.x, rCenter.y, rCenter.z );
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetCenter( real centerX, real centerY, real centerZ )
	{
		m_vCenter.x = centerX;
		m_vCenter.y = centerY;
		m_vCenter.z = centerZ;

		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	/** sets the up values */
	void CGUICamera::SetUp( const CGUIVector3& rUp )
	{
		SetEye( rUp.x, rUp.y, rUp.z );
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetUp( real upX, real upY, real upZ )
	{
		m_vUp.x = upX;
		m_vUp.y = upY;
		m_vUp.z = upZ;

		m_bDirty = true;
	}
	//------------------------------------------------------------------------------
	/** get the eye vector values in points */
	const CGUIVector3& CGUICamera::GetEye() const
	{
		return m_vEye;
	}
	//------------------------------------------------------------------------------
	/** get the center vector values in points */
	const CGUIVector3& CGUICamera::GetCenter() const
	{
		return m_vCenter;
	}
	//------------------------------------------------------------------------------
	/** get the up vector values */
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
	void CGUICamera::SetFov( real rFov )
	{
		m_fFov = rFov;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetAspectRatio( real rRatio )
	{
		m_fAspectRatio = rRatio;
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
	void CGUICamera::SetNearPlane( real fNearPlane )
	{
		m_fNearPlane = fNearPlane;
	}
	//------------------------------------------------------------------------------
	void CGUICamera::SetFarPlane( real fFarPlane )
	{
		m_fFarPlane = fFarPlane;
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
}
