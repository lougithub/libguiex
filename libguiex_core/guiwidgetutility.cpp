/** 
* @file guiwidgetutility.cpp
* @brief utility function for widget.
* @author ken
* @date 2006-09-27
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guiwidgetutility.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiwidgetmanager.h>
#include <sstream>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIString CGUIWidgetUtility::GetWidgetInfo( const CGUIWidget* pWidget ) 
	{
		if( !pWidget )
		{
			throw CGUIException("[CGUIWidgetUtility::GetWidgetInfo]: invalid parameter!");
		}

		//NAME<> TYPE<>	STATE<>
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << "NAME<"<<pWidget->GetName().c_str()<<">\tTYPE<"<<pWidget->GetType().c_str()<<">\tSTATE<";
		if( pWidget->IsOpen())
		{
			stream<<"WS_OPENED>";
		}
		else
		{
			stream<<"WS_CLOSED>";
		}

		return stream.str();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get widget's information, the format of it is:
	* NAME<> TYPE<>	STATE<>
	*/
	CGUIString CGUIWidgetUtility::GetWidgetInfo( const CGUIString& rWidgetName, const CGUIString& rSceneName ) 
	{
		return GetWidgetInfo( CGUIWidgetManager::Instance()->GetWidget(rWidgetName, rSceneName));
	}
	//------------------------------------------------------------------------------
	static void	GetWidgetTreeInfo_Imp( 
		const CGUIWidget* pWidget, 
		std::stringstream& stream,  
		const std::string& rPrefix )
	{
		stream<<rPrefix<<CGUIWidgetUtility::GetWidgetInfo(pWidget).c_str()<<"\n";
		std::string nChildPrefix = rPrefix+"    ";
		CGUIWidget* pChild = pWidget->GetChild();
		while( pChild )
		{
			GetWidgetTreeInfo_Imp(pChild, stream, nChildPrefix);
			pChild = pChild->GetNextSibling();
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get widget's and it's child's information, the format of it is:
	* NAME<> TYPE<>	STATE<>
	*		NAME<> TYPE<>	STATE<>
	*			NAME<> TYPE<>	STATE<>
	*			NAME<> TYPE<>	STATE<>
	*		NAME<> TYPE<>	STATE<>
	*/
	CGUIString	CGUIWidgetUtility::GetWidgetTreeInfo( const CGUIWidget* pWidget ) 
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		GetWidgetTreeInfo_Imp(pWidget, stream, "%");
		return stream.str();
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWidgetUtility::GetWidgetTreeInfo( const CGUIString& rWidgetName, const CGUIString& rSceneName ) 
	{
		return GetWidgetTreeInfo(CGUIWidgetManager::Instance()->GetWidget(rWidgetName, rSceneName));
	}
	//------------------------------------------------------------------------------

}//namespace guiex
