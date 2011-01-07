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
wxPGProperty* WxToolsPGManager::ToolsGetProperty( const std::string& rName, const std::string& rType ) const
{
	for ( size_t i=0; i<GetPageCount(); i++ )
	{
		wxPropertyGridPageState* pState = m_arrPages[i]->GetStatePtr();
		wxPGProperty* pRootProp = pState->DoGetRoot();

		if( pState->IsInNonCatMode() )
		{
			//no category
			for( unsigned long j=0; j<pRootProp->GetChildCount(); ++j )
			{
				wxPGProperty* pProp = pRootProp->Item( j );
				if( pProp )
				{
					std::string* pType = (std::string*)GetPropertyClientData( pProp );
					if( !pType )
					{
						continue;
					}
					if( *pType == rType && 
						wx2GuiString( pProp->GetName()) == rName )
					{
						return pProp;
					}
				}
			}
		}
		else
		{
			//no category
			for( unsigned long j=0; j<pRootProp->GetChildCount(); ++j )
			{
				//has category
				wxPGProperty* pPropCategory = pRootProp->Item( j );
				for( unsigned long k=0; k<pPropCategory->GetChildCount(); ++k )
				{
					wxPGProperty* pProp = pPropCategory->Item( k );
					if( pProp )
					{
						std::string* pType = (std::string*)GetPropertyClientData( pProp );
						if( !pType )
						{
							continue;
						}
						if( *pType == rType && 
							wx2GuiString( pProp->GetName()) == rName )
						{
							return pProp;
						}
					}
				}
			}
		}
	}

	return NULL;
}
//------------------------------------------------------------------------------
