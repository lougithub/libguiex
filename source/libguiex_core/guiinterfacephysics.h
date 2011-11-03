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
		IGUIInterfacePhysics( const char* szModuleName );
		virtual ~IGUIInterfacePhysics();

	public:
		/**
		* @brief update physics
		*/
		virtual	void Update( real fDeltaTime ) = 0;

		/**
		* @brief render extra info if there exist.
		*/
		virtual void RenderExtraInfo( class IGUIInterfaceRender* pRender ){}

	public: 
		static const char* StaticGetModuleType();
	};
}//namespace guiex

#endif //__GUI_INTERFACE_PHYSICS_H_20101221__

