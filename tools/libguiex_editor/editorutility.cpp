/** 
* @file editorutility.cpp
* @author ken
*/

//============================================================================//
// include
//============================================================================//
#include "editorutility.h"
#include <wx/filename.h> 


//============================================================================//
// function
//============================================================================//
//------------------------------------------------------------------------------
wxString Gui2wxString( const CGUIString& rString )
{
	return wxConvUTF8.cMB2WC(rString.c_str());
}
//------------------------------------------------------------------------------
CGUIString wx2GuiString( const wxString& rString )
{
	return wxConvUTF8.cWC2MB(rString.c_str()).data();
}
//------------------------------------------------------------------------------
wxImage* LoadwxImageByGuiImage( const CGUIImage* pGuiImage )
{
	wxString strImagePath = Gui2wxString( GSystem->GetDataPath() + pGuiImage->GetFullFilePath() );
	wxFileName filename( strImagePath );
	wxImage* pWxImage = NULL;
	if( filename.GetExt().CmpNoCase(L"tga") == 0)
	{
		pWxImage = new wxImage( filename.GetFullPath(), wxBITMAP_TYPE_TGA );
	}
	else if( filename.GetExt().CmpNoCase(L"png") == 0)
	{
		pWxImage = new wxImage( filename.GetFullPath(), wxBITMAP_TYPE_PNG );
	}

	if( !pWxImage )
	{
		return NULL;
	}

	if( !pWxImage->Ok())
	{
		delete pWxImage;
		return NULL;
	}

	const CGUIRect& rRect = pGuiImage->GetUVRect();
	wxSize newSize( 
		pWxImage->GetSize().GetWidth() * rRect.GetWidth(), 
		pWxImage->GetSize().GetHeight()* rRect.GetHeight() );
	wxPoint newPoint( 
		rRect.GetPosition().x * pWxImage->GetSize().GetWidth(), 
		rRect.GetPosition().y * pWxImage->GetSize().GetHeight() );
	pWxImage->Resize( newSize, -newPoint );
	switch( pGuiImage->GetOrientation() )
	{
	case eImageOrientation_90CW: ///!< rotate image for 90 CW
		*pWxImage = pWxImage->Rotate90(true);
		break;
	case eImageOrientation_90CCW: ///!< rotate image for 90 CCW
		*pWxImage = pWxImage->Rotate90(false);
		break;
	case eImageOrientation_FlipHorizon:	///!< flip image horizon
		*pWxImage = pWxImage->Mirror( true );
		break;
	case eImageOrientation_FlipVertical: ///!< flip image vertical
		*pWxImage = pWxImage->Mirror( false );
		break;
	}
	switch( pGuiImage->GetOrientation() )
	{
	case eImageOrientation_90CW:
		pWxImage->Rotate90( true );
		break;
	case eImageOrientation_90CCW:
		pWxImage->Rotate90( false );
		break;
	case eImageOrientation_FlipHorizon:
		pWxImage->Mirror( true );
		break;
	case eImageOrientation_FlipVertical:
		pWxImage->Mirror( false );
		break;
	}

	return pWxImage;
}
//------------------------------------------------------------------------------
