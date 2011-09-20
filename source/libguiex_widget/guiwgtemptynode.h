/** 
* @file guiwgtemptynode.h
* @brief empty node, used to manage widget
* for example, used to group radio button
* @author ken
* @date 2006-08-03
*/



#ifndef __GUI_WGTEMPTYNODE_20060803_H__
#define __GUI_WGTEMPTYNODE_20060803_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIWgtEmptyNode
	* @brief used to manage widget, for example,
	* group radio button
	*/
	class GUIEXPORT CGUIWgtEmptyNode: public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtEmptyNode( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtEmptyNode( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/// initialize empty node
		void	InitEmptyNode();

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtEmptyNode);
	};


}//namespace guiex

#endif //__GUI_WGTEMPTYNODE_20060803_H__

