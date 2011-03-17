/** 
* @file guiasgridbase.h
* @brief
* @author ken
* @date 2011-03-16
*/

#ifndef __GUI_ASGRIDBASE_20110316_H__
#define __GUI_ASGRIDBASE_20110316_H__

//============================================================================//
// include
//============================================================================// 
#include "guias.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUISceneEffectGridBase;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//*****************************************************************************
	//	CGUIAsGridBase
	//*****************************************************************************
	class GUIEXPORT CGUIAsGridBase : public CGUIAs
	{
	protected:
		CGUIAsGridBase( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsGridBase( );

		void SetGridSize( const CGUIIntSize& rGridSize );
		const CGUIIntSize& GetGridSize() const;


	public:
		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

		virtual int32 InitGrid( );
		virtual void OnDestory();

	protected:
		virtual CGUISceneEffectGridBase* GenerateGrid( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize ) = 0;

	protected:
		CGUIIntSize m_aGridSize;
		CGUISceneEffectGridBase* m_pEffectGrid;
	};


	


}//namespace guiex


#endif //__GUI_ASGRIDBASE_20110316_H__



