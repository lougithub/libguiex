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
		void SetCenter( real centerX, real centerY, real centerZ );
		void SetUp( real upX, real upY, real upZ );

		const CGUIVector3& GetEye() const;
		const CGUIVector3& GetCenter() const;
		const CGUIVector3& GetUp() const;

	public:
		CGUIVector3 m_vEye;
		CGUIVector3 m_vCenter;
		CGUIVector3 m_vUp;
		
		real m_fFov;
		real m_fAspectRatio;
	};

}//namespace guiex

#endif //__GUI_CAMERA_20101222_H__
