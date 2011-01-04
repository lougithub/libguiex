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
	class GUIEXPORT CGUIFontManager : public CGUIResourceManager <CGUIFontData>
	{
	public:
		CGUIFontManager();
		virtual ~CGUIFontManager();

		static CGUIFontManager* Instance(); 

		int32 RegisterFont( const CGUIString& rSceneName, const CGUIProperty& rProperty);

	protected:
		virtual	void DestroyResourceImp( CGUIFontData* pRes ); 
		
		CGUIFontData* DoCreateFont(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rPath, 
			uint32 nFontID );

	private:
		static CGUIFontManager* m_pSingleton;
	};

}//namespace guiex

#endif		//__GUI_FONTMANAGER_20091027_H__

