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
#include "guibase.h"
#include "guivector3.h"
#include "sigslot.h"


//============================================================================//
// include
//============================================================================// 
namespace guiex
{
	class CGUIIntSize;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUICamera : public sigslot::has_slots<>
	{
	public:
		CGUICamera();
		CGUICamera( const CGUICamera& rOther );
		virtual ~CGUICamera();
		const CGUICamera& operator=( const CGUICamera& rOther );

	public:
		void Reset();

		void SetBaseEye( const CGUIVector3& rEye );
		void SetOffsetEye( const CGUIVector3& rEye );
		const CGUIVector3& GetBaseEye() const;
		const CGUIVector3& GetOffsetEye() const;
		const CGUIVector3& GetEye() const;

		void SetBaseCenter( const CGUIVector3& rCenter );
		void SetOffsetCenter( const CGUIVector3& rCenter );
		const CGUIVector3& GetBaseCenter() const;
		const CGUIVector3& GetOffsetCenter() const;
		const CGUIVector3& GetCenter() const;

		void SetBaseUp( const CGUIVector3& rUp );
		void SetOffsetUp( const CGUIVector3& rUp );
		const CGUIVector3& GetBaseUp() const;
		const CGUIVector3& GetOffsetUp() const;
		const CGUIVector3& GetUp() const;

		void SetBaseFov( real rFov );
		void SetOffsetFov( real rFov );
		real GetBaseFov() const;
		real GetOffsetFov() const;
		real GetFov() const;

		void SetBaseAspectRatio( real rAspectRatio );
		void SetOffsetAspectRatio( real rAspectRatio );
		real GetBaseAspectRatio() const;
		real GetOffsetAspectRatio() const;
		real GetAspectRatio() const;

		real GetNearPlane() const;
		real GetFarPlane() const;

		//dirty flag
		bool IsDirty();
		void SetDirty();
		void ClearDirty();

	protected:
		virtual void OnCameraDefaultValueChanged( );
		void Restore();

		void UpdateEye();
		void UpdateCenter();
		void UpdateUp();
		void UpdateFov();
		void UpdateAspectRatio();

	private:
		//base value
		CGUIVector3 m_vBaseEye;
		CGUIVector3 m_vBaseCenter;
		CGUIVector3 m_vBaseUp;
		real m_fBaseFov;
		real m_fBaseAspectRatio;

		//offset value
		CGUIVector3 m_vOffsetEye;
		CGUIVector3 m_vOffsetCenter;
		CGUIVector3 m_vOffsetUp;
		real m_fOffsetFov;
		real m_fOffsetAspectRatio;

		//final value
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
