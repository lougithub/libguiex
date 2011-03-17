/** 
* @file guiastiledgrid3d.h
* @brief
* @author ken
* @date 2011-03-16
*/

#ifndef __GUI_ASTILEDGRID3D_20110316_H__
#define __GUI_ASTILEDGRID3D_20110316_H__

//============================================================================//
// include
//============================================================================// 
#include "guiasgridbase.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	//*****************************************************************************
	//	CGUIAsTiledGrid3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsTiledGrid3D : public CGUIAsGridBase
	{
	protected:
		CGUIAsTiledGrid3D( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsTiledGrid3D( );

	public:
		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

	protected:
		virtual CGUISceneEffectGridBase* GenerateGrid( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize );
		
		/*
		// returns the tile that belongs to a certain position of the grid
		-(ccQuad3)tile:(ccGridSize)pos;
		// returns the non-transformed tile that belongs to a certain position of the grid
		-(ccQuad3)originalTile:(ccGridSize)pos;
		// sets a new tile to a certain position of the grid
		-(void)setTile:(ccGridSize)pos coords:(ccQuad3)coords;
		*/
	};

}//namespace guiex


#endif //__GUI_ASTILEDGRID3D_20110316_H__



