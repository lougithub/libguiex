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
#define GUI_AS_REGISTER(asname)	\
	::guiex::CGUIAsFactory::Instance()->RegisterGenerator( #asname, ::guiex::asname##_Generator::GenerateSelf())

#define GUI_AS_GENERATE( asname )	\
	(::guiex::asname*)( ::guiex::CGUIAsFactory::Instance()->GenerateAs(#asname))

#define GUI_USER_AS_REGISTER(asname)	\
	::guiex::CGUIAsFactory::Instance()->RegisterGenerator( #asname, asname##_Generator::GenerateSelf())

#define GUI_USER_AS_GENERATE( asname )	\
	static_cast<asname*>( ::guiex::CGUIAsFactory::Instance()->GenerateEvent(#asname))



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
		CGUIAs*	GenerateAs(const CGUIString& rName);

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

