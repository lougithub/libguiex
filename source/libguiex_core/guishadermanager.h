/** 
* @file guishadermanager.h
* @brief shader manager.
* @author ken
* @date 2011-09-23
*/

#ifndef __GUI_SHADERMANAGER_20110923_H__
#define __GUI_SHADERMANAGER_20110923_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guishader.h"
#include "guiresourcemanager.h"
#include <set>
#include <map>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIProperty;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIShaderManager
	* @brief image manager
	* 
	*/
	class GUIEXPORT CGUIShaderManager : public CGUIResourceManager <CGUIShader, CGUIShader>
	{
	public:
		CGUIShaderManager();
		virtual ~CGUIShaderManager();

		static CGUIShaderManager* Instance(); 

		virtual int32 RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty);
		CGUIShader* AllocateResource( const CGUIString& rImageName ) const;
		virtual void DeallocateResource( CGUIResource* pRes );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 

	private:
		static CGUIShaderManager* m_pSingleton;
	};

}//namespace guiex

#endif		//__GUI_SHADERMANAGER_20110923_H__

