/** 
* @file guisceneutility.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/

#ifndef	__KEN_GUISCENEUTILITY_20091022_H__
#define __KEN_GUISCENEUTILITY_20091022_H__

//============================================================================//
// include
//============================================================================//
#include "guibase.h"


//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CGUIWidget;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUISceneUtility
	{
	public:
		static int32 LoadResource(const CGUIString& strSceneName);
		static int32 UnloadResource(const CGUIString& strSceneName);

		static int32 LoadPages( const CGUIString& strSceneName);
	};
}		//namespace guiex

#endif //__KEN_GUISCENEUTILITY_20091022_H__

