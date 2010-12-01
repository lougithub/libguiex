/** 
* @file guiinterfacecommand.h
* @brief command from out of application, use script
* to control ui system
* @author ken
* @date 2006-09-26
*/


#ifndef __GUI_INTERFACE_COMMAND_H_20060926__
#define __GUI_INTERFACE_COMMAND_H_20060926__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiinterface.h"
#include "guistring.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class IGUIInterfaceCommand
	* @brief script command from out of gui system
	*/
	class GUIEXPORT IGUIInterfaceCommand : public IGUIInterface
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIInterfaceCommand();

		/** 
		* @brief destructor
		*/
		virtual ~IGUIInterfaceCommand();


	public:
		/**
		* @brief receive command
		*/
		virtual	const char* Receive( ) = 0;

		/**
		* @brief send output
		*/
		virtual	void Send( const CGUIString& rOutput ) = 0;

	};
}//namespace guiex


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	/**
	* @brief get command interface
	*/
	GUIEXPORT IGUIInterfaceCommand*	GetInterface_Command( );

}//namespace guiex

#endif //__GUI_INTERFACE_COMMAND_H_20060926__

