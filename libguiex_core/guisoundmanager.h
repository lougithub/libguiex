/** 
* @file guisoundmanager.h
* @brief sound manager.
* @author ken
* @date 2011-01-04
*/

#ifndef __GUI_SOUNDMANAGER_20110104_H__
#define __GUI_SOUNDMANAGER_20110104_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guisounddata.h"
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
	* @class CGUISoundManager
	* @brief sound manager
	* 
	*/
	class GUIEXPORT CGUISoundManager : public CGUIResourceManager <CGUISoundData, CGUISoundData>
	{
	public:
		CGUISoundManager();
		virtual ~CGUISoundManager();

		static CGUISoundManager* Instance(); 

		int32 RegisterSound( const CGUIString& rSceneName, const CGUIProperty& rProperty);
		CGUISoundData* AllocateResource( const CGUIString& rSoundName ) const;
		int32 DeallocateResource( CGUISoundData* pSound );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 
		
		CGUISoundData* DoCreateSound(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rPath);

	private:
		static CGUISoundManager* m_pSingleton;
	};

}//namespace guiex

#endif //__GUI_SOUNDMANAGER_20110104_H__

