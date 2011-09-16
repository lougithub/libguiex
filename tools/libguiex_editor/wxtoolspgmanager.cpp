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
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
WxToolsPGManager::WxToolsPGManager(  wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
:wxPropertyGridManager( parent, id, pos, size, style)
{

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
