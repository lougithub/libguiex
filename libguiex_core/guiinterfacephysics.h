/** 
* @file guiinterfacephysics.h
* @brief physics interface
* @author ken
* @date 2010-12-21
*/


#ifndef __GUI_INTERFACE_PHYSICS_H_20101221__
#define __GUI_INTERFACE_PHYSICS_H_20101221__

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
	* @class IGUIInterfacePhysics
	* @brief physics interface used in system
	*/
	class GUIEXPORT IGUIInterfacePhysics : public IGUIInterface
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIInterfacePhysics( const char* szModuleName );

		/** 
		* @brief destructor
		*/
		virtual ~IGUIInterfacePhysics();

	public:
		/**
		* @brief update physics
		*/
		virtual	void Update( real fDeltaTime ) = 0;

	public: 
		static const char* StaticGetModuleType();
	};
}//namespace guiex

#endif //__GUI_INTERFACE_PHYSICS_H_20101221__

