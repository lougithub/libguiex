/** 
* @file guiwidgetutility.cpp
* @brief utility function for widget.
* @author ken
* @date 2006-09-27
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guiwidgetutility.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiexception.h>
#include <sstream>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIString	CGUIWidgetUtility::GetWidgetInfo( const CGUIWidget* pWidget ) 
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
	CGUIString	CGUIWidgetUtility::GetWidgetInfo( const CGUIString& rWidgetName, const CGUIString& rProjectName ) 
	{
		return GetWidgetInfo(CGUIWidgetSystem::Instance()->GetWidget(rWidgetName, rProjectName));
	}
	//------------------------------------------------------------------------------
	static void		GetWidgetTreeInfo_Imp( 
		const CGUIWidget* pWidget, 
		std::stringstream& stream,  
		const std::string& rPrefix )
	{
		//stream<<rPrefix<<CGUIWidgetUtility::GetWidgetInfo(pWidget).c_str()<<"\n";
		//std::string nChildPrefix = rPrefix+"    ";
		//uint32 nSize = pWidget->GetChildNum(); 
		//for( uint32 i=0; i<nSize; ++i)
		//{
		//	GetWidgetTreeInfo_Imp(pWidget->GetChild(i), stream, nChildPrefix);
		//}
		stream<<rPrefix<<CGUIWidgetUtility::GetWidgetInfo(pWidget).c_str()<<"\n";
		std::string nChildPrefix = rPrefix+"    ";
		CGUIWidget* pChild = pWidget->GetChild();
		while( pChild )
		{
			GetWidgetTreeInfo_Imp(pChild, stream, nChildPrefix);
			pChild = pChild->GetNextSibling();
		}
	}
	CGUIString	CGUIWidgetUtility::GetWidgetTreeInfo( const CGUIWidget* pWidget ) 
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		GetWidgetTreeInfo_Imp(pWidget, stream, "%");
		return stream.str();
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWidgetUtility::GetWidgetTreeInfo( const CGUIString& rWidgetName, const CGUIString& rProjectName ) 
	{
		return GetWidgetTreeInfo(CGUIWidgetSystem::Instance()->GetWidget(rWidgetName, rProjectName));
	}
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------

}//namespace guiex
