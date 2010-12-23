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
#include "guicamera.h"

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

		const CGUICamera* GetDefaultUICamera() const;
		CGUICamera* GetDefaultUICamera();

	protected:
		friend class CGUISystem;
		CGUICameraManager();
		CGUICameraManager( const CGUICameraManager& );

	protected:
		CGUICamera m_aDefaultUICamera;
		CGUICamera m_aDefaultCanvasCamera;

	private:
		static CGUICameraManager* m_pSingleton;
	};

	//------------------------------------------------------------------------------
	inline const CGUICamera* CGUICameraManager::GetDefaultUICamera() const
	{
		return &m_aDefaultUICamera;
	}
	//------------------------------------------------------------------------------
	inline CGUICamera* CGUICameraManager::GetDefaultUICamera()
	{
		return &m_aDefaultUICamera;
	}
	//------------------------------------------------------------------------------

}//namespace guiex

#endif //__GUI_CAMERAMANAGER_20101222_H__
