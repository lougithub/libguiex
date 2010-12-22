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


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUICamera::CGUICamera()
	{
		m_fFov = 45.0f;
		m_fAspectRatio = 1.0f;

		m_vEye.x = 0;
		m_vEye.y = 0;
		m_vEye.z = -1;

		m_vCenter.x = 0;
		m_vCenter.y = 0;
		m_vCenter.z = 0;

		m_vUp.x = 0;
		m_vUp.y = -1;
		m_vUp.z = 0;
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

		return *this;
	}
	//------------------------------------------------------------------------------
	/** sets the camera in the default position */
	void CGUICamera::Restore()
	{
		const CGUISize& rSize = GSystem->GetScreenSize();

		m_fFov = 45.0f;
		if( GUI_REAL_EQUAL( rSize.GetHeight(), 0.0f ))
		{
			m_fAspectRatio = 1;
		}
		else
		{
			m_fAspectRatio = rSize.GetWidth() / rSize.GetHeight();
		}

		real fZDistance = -(rSize.m_fHeight/2) / CGUIMath::Tan( CGUIDegree(m_fFov/2));

		m_vEye.x = rSize.GetWidth() / 2;
		m_vEye.y = rSize.GetHeight() / 2;
		m_vEye.z = fZDistance;

		m_vCenter.x = rSize.GetWidth() / 2;
		m_vCenter.y = rSize.GetHeight() / 2;
		m_vCenter.z = 0;

		m_vUp.x = 0;
		m_vUp.y = -1;
		m_vUp.z = 0;
	}
	//------------------------------------------------------------------------------
	/** sets the eye values in points */
	void CGUICamera::SetEye( real eyeX, real eyeY, real eyeZ )
	{
		m_vEye.x = eyeX;
		m_vEye.y = eyeY;
		m_vEye.z = eyeZ;
	}
	//------------------------------------------------------------------------------
	/** sets the center values in points */
	void CGUICamera::SetCenter( real centerX, real centerY, real centerZ )
	{
		m_vCenter.x = centerX;
		m_vCenter.y = centerY;
		m_vCenter.z = centerZ;
	}
	//------------------------------------------------------------------------------
	/** sets the up values */
	void CGUICamera::SetUp( real upX, real upY, real upZ )
	{
		m_vUp.x = upX;
		m_vUp.y = upY;
		m_vUp.z = upZ;
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
}
