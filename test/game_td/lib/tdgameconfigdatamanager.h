/** 
* @file tdgameconfigdatamanager.h
* @brief config data manager
* @author ken
* @date 2011-08-22
*/

#ifndef __TD_GAMECONFIGDATAMANAGER_20110822_H__
#define __TD_GAMECONFIGDATAMANAGER_20110822_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiex.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameConfigDataManager
	{
	public:
		CTDGameConfigDataManager();
		~CTDGameConfigDataManager();

		///< rpath is relative to game path
		void LoadData( const CGUIString& rPath );

	protected:
		typedef std::map<CGUIString, CGUIProperty> TMapProperty;
		TMapProperty m_aMapProperty;
	};
}

#endif //__TD_GAMECONFIGDATAMANAGER_20110822_H__
