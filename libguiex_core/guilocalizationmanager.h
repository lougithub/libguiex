/** 
* @file guilocalizationmanager.h
* @brief localization manager
* @author ken
* @date 2011-04-01
*/

#ifndef __GUI_LOCALIZATIONMANAGER_20110401_H__
#define __GUI_LOCALIZATIONMANAGER_20110401_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"

#include <map>


//============================================================================//
// declare
//============================================================================//
namespace guiex
{
}

//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT CGUILocalizationManager
	{
	public:
		static CGUILocalizationManager* Instance();

		int32 LoadLocalization( const CGUIString& rFileName, const CGUIString& rSceneName);
		void UnloadLocalization( const CGUIString& rSceneName);

		const CGUIString& GetCurrentLocalConfig() const;
		void SetCurrentLocalConfig( const CGUIString& rLocalConfig );

		CGUIStringW Localize( const CGUIString& rSceneName, const CGUIString& rKey ) const;

		///< full_key should be "@scenename/key"
		CGUIStringW Localize( const CGUIStringW& rFullKey ) const;
		CGUIStringW Localize( const CGUIString& rFullKey ) const;

	protected:
		friend class CGUISystem;
		CGUILocalizationManager();
		~CGUILocalizationManager();

	private:
		typedef std::map<CGUIString, CGUIStringW> TMapKeys;
		typedef std::map<CGUIString, TMapKeys> TMapScenes;

		TMapScenes m_mapScenes;

		CGUIString m_strCurrentLocalConfig;

	private:
		static CGUILocalizationManager* m_pSingleton;
	};
}

#endif //__GUI_LOCALIZATIONMANAGER_20110401_H__
