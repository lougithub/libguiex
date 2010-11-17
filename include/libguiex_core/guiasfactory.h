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

#define GUI_AS_GENERATE( astype, asname, asscenetype )	\
	(::guiex::astype*)( ::guiex::CGUIAsFactory::Instance()->GenerateAs(#astype, #asname, #asscenetype))



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
		/**
		* @brief register generator
		* @return 0 for successful, vice versa
		*/
		int RegisterGenerator(const CGUIString& rName, CGUIAsGenerator* pGenerator );

		/**
		* @brief unregister generator
		* @return 0 for successful, vice versa
		*/
		int UnregisterGenerator(const CGUIString& rName);

		/**
		* @brief unregister all generator
		*/
		void UnregisterAllGenerator();


		/**
		* @brief create a as by name
		*/
		CGUIAs*	GenerateAs(const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName);

		/**
		* @brief destroy as
		*/
		int	DestroyAs(CGUIAs*	pEvent);


	protected:
		CGUIAsFactory();
		~CGUIAsFactory();

	protected:
		typedef std::map<CGUIString,CGUIAsGenerator *> TMapGenerator;
		TMapGenerator m_mapGenerator;


		//for use singleton
		GUI_SINGLETON_DECLARE_EX(CGUIAsFactory);
	};


}//namespace guiex

#endif //__GUI_AS_FACTORY_20071121_H__

