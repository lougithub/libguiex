/** 
* @file guiastiledgrid3d.cpp
* @brief
* @author ken
* @date 2011-03-16
*/

//============================================================================//
// include
//============================================================================// 
#include "guiastiledgrid3d.h"
#include "guisceneeffecttiledgrid3d.h"

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//*****************************************************************************
	//	CGUIAsTiledGrid3D
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAsTiledGrid3D::CGUIAsTiledGrid3D( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsGridBase( rAsType, rAsName, rSceneName )
	{

	}
	//------------------------------------------------------------------------------
	CGUIAsTiledGrid3D::~CGUIAsTiledGrid3D( )
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIAsTiledGrid3D::ProcessProperty( const CGUIProperty& rProperty )
	{
		int32 ret = CGUIAsGridBase::ProcessProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsTiledGrid3D::GenerateProperty( CGUIProperty& rProperty )
	{
		int32 ret = CGUIAsGridBase::GenerateProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}



		return 0;
	}
	//------------------------------------------------------------------------------
}