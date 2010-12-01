/** 
* @file guiinterfacerender.cpp
* @brief interface for render module
* @author ken
* @date 2006-07-04
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacerender.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------	
	const real	IGUIInterfaceRender::ms_ZInitialValue = 1.0f;	//!< Initial value to use for 'z' each frame.
	//------------------------------------------------------------------------------
	const real	IGUIInterfaceRender::ms_ZElementStep = 0.0001f;	//!< Value to step 'z' for each GUI element.
	//------------------------------------------------------------------------------

	IGUIInterfaceRender::IGUIInterfaceRender( )
		:IGUIInterface("IGUIRender")
	{

	}
	//------------------------------------------------------------------------------ 
	IGUIInterfaceRender::~IGUIInterfaceRender()
	{

	}
	//------------------------------------------------------------------------------
	void	IGUIInterfaceRender::ResetZValue(void)
	{
		m_current_z = ms_ZInitialValue;
	}
	//------------------------------------------------------------------------------
	void	IGUIInterfaceRender::IncreaseZValue(void)
	{ 
		m_current_z -= ms_ZElementStep;
	}
	//------------------------------------------------------------------------------
	real IGUIInterfaceRender::GetAndIncZ() 
	{
		return 0.0f;
		//real oldZ = m_current_z ;
		//m_current_z -= ms_ZElementStep; 
		//return oldZ;
	}
	//------------------------------------------------------------------------------
}//namespace guiex


