/** 
* @file guiasfactory.h
* @brief the factory which is used to create as(action sequence)
* @author ken
* @date 2007-11-21
*/


#ifndef __GUI_AS_FACTORY_20071121_H__
#define __GUI_AS_FACTORY_20071121_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guisingleton.h"
#include <map>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIAs;
	class CGUIAsGenerator;
	class CGUIWidget;
}//namespace guiex


//============================================================================//
// define
//============================================================================// 
#define GUI_AS_REGISTER(astype)	\
	::guiex::CGUIAsFactory::Instance()->RegisterGenerator( #astype, ::guiex::astype::astype##_Generator::GenerateSelf())


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIAsFactory
	* @brief manager of as,it is a singleton class and hold a queue
	* to store as generator
	* @exception CGUIException
	*/
	class GUIEXPORT CGUIAsFactory
	{
	public:
		CGUIAsFactory();
		~CGUIAsFactory();

		static CGUIAsFactory* Instance();

		int RegisterGenerator(const CGUIString& rName, CGUIAsGenerator* pGenerator );
		int UnregisterGenerator(const CGUIString& rName);
		void UnregisterAllGenerator();

	protected:
		CGUIAs*	GenerateAs(const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName);
		int	DestroyAs( CGUIAs* pEvent );

	protected:
		friend class CGUIAsManager;

		typedef std::map<CGUIString,CGUIAsGenerator *> TMapGenerator;
		TMapGenerator m_mapGenerator;

	private:
		static CGUIAsFactory* m_pSingleton;
	};


}//namespace guiex

#endif //__GUI_AS_FACTORY_20071121_H__

