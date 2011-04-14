/** 
 * @file guicameramanager.h
 * @brief camera manager used in system
 * @author ken
 * @date 2010-12-22
 */

#ifndef __GUI_CAMERAMANAGER_20101222_H__
#define __GUI_CAMERAMANAGER_20101222_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guicamera.h"
#include "sigslot.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUICameraManager
	{
	public:
		~CGUICameraManager();

		static CGUICameraManager* Instance();

		const CGUIVector3& GetDefaultEye() const;
		const CGUIVector3& GetDefaultCenter() const;
		const CGUIVector3& GetDefaultUp() const;
		real GetDefaultFov() const;
		real GetDefaultAspectRatio() const;
		real GetDefaultNearPlane() const;
		real GetDefaultFarPlane() const;

		void SetCameraDefaultValue( const CGUIIntSize& rRawScreenSize, EScreenOrientation eScreenOrientation );

		sigslot::signal0<>& GetOnDefaultValueChangedSignal();

	protected:
		friend class CGUISystem;
		CGUICameraManager();
		CGUICameraManager( const CGUICameraManager& );

	private:
		//default value of camera
		CGUIVector3 m_vDefaultEye;
		CGUIVector3 m_vDefaultCenter;
		CGUIVector3 m_vDefaultUp;
		real m_fDefaultFov; //in degree
		real m_fDefaultAspectRatio;
		real m_fDefaultNearPlane;
		real m_fDefaultFarPlane;

		//sigslot
		sigslot::signal0<> OnDefaultValueChanged;

	private:
		static CGUICameraManager* m_pSingleton;
	};

}//namespace guiex

#endif //__GUI_CAMERAMANAGER_20101222_H__
