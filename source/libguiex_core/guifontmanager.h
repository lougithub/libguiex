/** 
* @file guifontmanager.h
* @brief font manager.
* @author ken
* @date 2009-10-27
*/

#ifndef __GUI_FONTMANAGER_20091027_H__
#define __GUI_FONTMANAGER_20091027_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guifontdata.h"
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
	* @class CGUIFontManager
	* @brief font manager
	* 
	*/
	class GUIEXPORT CGUIFontManager : public CGUIResourceManager <CGUIFontData, CGUIFontData>
	{
	public:
		CGUIFontManager();
		virtual ~CGUIFontManager();

		static CGUIFontManager* Instance(); 

		virtual int32 RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty );
		virtual void DeallocateResource( CGUIResource* pRes );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 
		
		CGUIFontData* DoCreateFont(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const SFontInfo& rFontInfo );

	private:
		static CGUIFontManager* m_pSingleton;
	};

}//namespace guiex

#endif		//__GUI_FONTMANAGER_20091027_H__

