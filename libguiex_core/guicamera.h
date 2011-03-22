/** 
 * @file guicamera.h
 * @brief camera used in system
 * @author ken
 * @date 2010-12-22
 */

#ifndef __GUI_CAMERA_20101222_H__
#define __GUI_CAMERA_20101222_H__

//============================================================================//
// include
//============================================================================// 
#include "guivector3.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUICamera
	{
	public:
		CGUICamera();
		CGUICamera( const CGUICamera& rOther );
		const CGUICamera& operator=( const CGUICamera& rOther );

	public:
		void Restore();

		void SetEye( real eyeX, real eyeY, real eyeZ );
		void SetEye( const CGUIVector3& rEye );
		void SetCenter( real centerX, real centerY, real centerZ );
		void SetCenter( const CGUIVector3& rCenter );
		void SetUp( real upX, real upY, real upZ );
		void SetUp( const CGUIVector3& rUp );

		const CGUIVector3& GetEye() const;
		const CGUIVector3& GetCenter() const;
		const CGUIVector3& GetUp() const;

		real GetFov() const;
		void SetFov( real rFov );
		void SetAspectRatio( real rRatio );
		real GetAspectRatio() const;
		real GetNearPlane() const;
		real GetFarPlane() const;
		void SetNearPlane( real fNearPlane );
		void SetFarPlane( real fFarPlane );

		bool IsDirty();
		void SetDirty();
		void ClearDirty();

	protected:
		CGUIVector3 m_vEye;
		CGUIVector3 m_vCenter;
		CGUIVector3 m_vUp;
		
		real m_fFov;
		real m_fAspectRatio;
		real m_fNearPlane;
		real m_fFarPlane;

		bool m_bDirty;
	};

}//namespace guiex

#endif //__GUI_CAMERA_20101222_H__
