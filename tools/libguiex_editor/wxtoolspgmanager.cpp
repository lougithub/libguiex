/** 
* @file wxtoolspgmanager.cpp
* @brief 
* @author ken
* @date 2011-01-05
*/

//============================================================================//
// include
//============================================================================// 
#include "wxtoolspgmanager.h"
#include "editorutility.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
WxToolsPGManager::WxToolsPGManager(  wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
:wxPropertyGridManager( parent, id, pos, size, style)
{

}
//------------------------------------------------------------------------------
void WxToolsPGManager::ToolsClearPage( )
{
	m_vecPropertyPages.clear();
	Clear();
}
//------------------------------------------------------------------------------
void WxToolsPGManager::ToolsSelectPage( const std::string& rPageName )
{
	for( size_t i=0; i<m_vecPropertyPages.size(); ++i )
	{
		if( m_vecPropertyPages[i] == rPageName )
		{
			SelectPage( i );
			return;
		}
	}
	AddPage( Gui2wxString( rPageName ) );
	m_vecPropertyPages.push_back( rPageName );
	SelectPage( m_vecPropertyPages.size() - 1 );
}
//------------------------------------------------------------------------------
