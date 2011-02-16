/** 
 * @file resourcelist.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2010-12-24
 */


//============================================================================//
// include
//============================================================================//
#include "resourcelist.h"
#include "editorutility.h"
#include <wx/filename.h> 



//============================================================================//
// function
//============================================================================//
//------------------------------------------------------------------------------
CResourceList::CResourceList()
{
}
//------------------------------------------------------------------------------
CResourceList::~CResourceList()
{
	ResetImageList();
	ResetAsList();
	ResetParticle2DList();
	ResetSoundList();
	ResetTileMapList();
}
//------------------------------------------------------------------------------
CResourceList* CResourceList::Instance()
{
	static CResourceList sInstance;
	return &sInstance;
}
//------------------------------------------------------------------------------
void CResourceList::UpdateResourceList()
{
	UpdateImageList();
	UpdateAsList();
	UpdateParticle2DList();
	UpdateSoundList();
	UpdateTileMapList();
}
//------------------------------------------------------------------------------
void CResourceList::ResetImageList()
{
	m_arrayImageArray.Clear();

	for( std::map<wxString, wxBitmap*>::iterator itor = m_mapImageThumbnails.begin();
		itor != m_mapImageThumbnails.end();
		++itor )
	{
		delete itor->second;
	}
	m_mapImageThumbnails.clear();

	for( std::map<wxString, wxBitmap*>::iterator itor = m_mapOriginalImageThumbnails.begin();
		itor != m_mapOriginalImageThumbnails.end();
		++itor )
	{
		delete itor->second;
	}
	m_mapOriginalImageThumbnails.clear();
}
//------------------------------------------------------------------------------
void CResourceList::UpdateImageList()
{
	ResetImageList();

	m_arrayImageArray.Add(_T(""));
	const std::map<CGUIString, CGUIImage*>& rMapImageList = CGUIImageManager::Instance()->GetRegisterResourceMap();
	for( std::map<CGUIString,CGUIImage*>::const_iterator itor = rMapImageList.begin();
		itor != rMapImageList.end();
		++itor)
	{
		CGUIImage* pGuiImage = itor->second;
		const CGUIString& rGuiImageName = itor->first;
		wxString rWxImageName = Gui2wxString( rGuiImageName );

		m_arrayImageArray.Add( rWxImageName );

		// Create the image thumbnail
		wxString rImagePath = Gui2wxString( GSystem->GetDataPath() + CGUISceneManager::Instance()->GetScenePath( pGuiImage->GetSceneName() ) + pGuiImage->GetFilePath() );
		wxFileName filename( rImagePath );
		if ( filename.FileExists() )
		{
			wxImage* pWxImage = new wxImage( filename.GetFullPath(), wxBITMAP_TYPE_TGA );
			if ( pWxImage && pWxImage->Ok() )
			{
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

				wxBitmap* pOriginalBitmap = new wxBitmap( *pWxImage );
				m_mapOriginalImageThumbnails[rWxImageName] = pOriginalBitmap;

				int im_wid = pWxImage->GetWidth();
				int im_hei = pWxImage->GetHeight();
				if ( pWxImage->GetWidth() > pWxImage->GetHeight() )
				{
					// TNW = (TNH*IW)/IH
					im_wid = PREF_THUMBNAIL_HEIGHT;
					im_hei = PREF_THUMBNAIL_HEIGHT * pWxImage->GetHeight() / pWxImage->GetWidth();
				}
				else
				{
					im_hei = PREF_THUMBNAIL_HEIGHT;
					im_wid = (PREF_THUMBNAIL_HEIGHT*pWxImage->GetWidth())/pWxImage->GetHeight();
				}
				pWxImage->Rescale( im_wid, im_hei );

				wxBitmap* pBitmap = new wxBitmap( *pWxImage );
				m_mapImageThumbnails[rWxImageName] = pBitmap;

				delete pWxImage;
			}
		}
	}
	m_arrayImageArray.Sort();
}
//------------------------------------------------------------------------------
const wxBitmap* CResourceList::GetImageThumbnail( const wxString& rImageName )
{
	std::map<wxString, wxBitmap*>::iterator itor = m_mapImageThumbnails.find( rImageName );
	if( itor != m_mapImageThumbnails.end() )
	{
		return itor->second;
	}
	return NULL;
}
//------------------------------------------------------------------------------
const wxBitmap* CResourceList::GetOriginalImageThumbnail( const wxString& rImageName )
{
	std::map<wxString, wxBitmap*>::iterator itor = m_mapOriginalImageThumbnails.find( rImageName );
	if( itor != m_mapOriginalImageThumbnails.end() )
	{
		return itor->second;
	}
	return NULL;
}
//------------------------------------------------------------------------------
void CResourceList::ResetAsList()
{
	m_arrayAsArray.Clear();
}
//------------------------------------------------------------------------------
void CResourceList::UpdateTileMapList()
{
	ResetTileMapList();

	m_arrayTileMapArray.Add(_T(""));
	const std::map<CGUIString, CGUITileMapData*>& rMapTileMapDataList = CGUITileMapManager::Instance()->GetRegisterResourceMap();
	for( std::map<CGUIString,CGUITileMapData*>::const_iterator itor = rMapTileMapDataList.begin();
		itor != rMapTileMapDataList.end();
		++itor)
	{
		m_arrayTileMapArray.Add(Gui2wxString( itor->first));
	}
	m_arrayTileMapArray.Sort();
}
//------------------------------------------------------------------------------
void CResourceList::UpdateParticle2DList()
{
	ResetParticle2DList();

	m_arrayParticle2DArray.Add(_T(""));
	const std::map<CGUIString, CGUIParticle2DData*>& rMapParticle2DDataList = CGUIParticle2DManager::Instance()->GetRegisterResourceMap();
	for( std::map<CGUIString,CGUIParticle2DData*>::const_iterator itor = rMapParticle2DDataList.begin();
		itor != rMapParticle2DDataList.end();
		++itor)
	{
		m_arrayParticle2DArray.Add(Gui2wxString( itor->first));
	}
	m_arrayParticle2DArray.Sort();
}
//------------------------------------------------------------------------------
void CResourceList::ResetTileMapList()
{
	m_arrayTileMapArray.Clear();
}
//------------------------------------------------------------------------------
void CResourceList::ResetParticle2DList()
{
	m_arrayParticle2DArray.Clear();
}
//------------------------------------------------------------------------------
void CResourceList::UpdateAsList()
{
	ResetAsList();

	m_arrayAsArray.Add(_T(""));
	const std::map<CGUIString, CGUIAsData*>& rMapAsList = CGUIAsManager::Instance()->GetRegisterResourceMap();
	for( std::map<CGUIString,CGUIAsData*>::const_iterator itor = rMapAsList.begin();
		itor != rMapAsList.end();
		++itor)
	{
		m_arrayAsArray.Add(Gui2wxString( itor->first));
	}
	m_arrayAsArray.Sort();
}
//------------------------------------------------------------------------------
void CResourceList::ResetSoundList()
{
	m_arraySoundArray.Clear();
}
//------------------------------------------------------------------------------
void CResourceList::UpdateSoundList()
{
	ResetSoundList();

	m_arraySoundArray.Add(_T(""));
	const std::map<CGUIString, CGUISoundData*>& rMapSoundList = CGUISoundManager::Instance()->GetRegisterResourceMap();
	for( std::map<CGUIString,CGUISoundData*>::const_iterator itor = rMapSoundList.begin();
		itor != rMapSoundList.end();
		++itor)
	{
		m_arraySoundArray.Add(Gui2wxString( itor->first));
	}
	m_arraySoundArray.Sort();
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetImageList()
{
	return m_arrayImageArray;
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetAsList()
{
	return m_arrayAsArray;
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetTileMapList()
{
	return m_arrayTileMapArray;
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetParticle2DList()
{
	return m_arrayParticle2DArray;
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetSoundList()
{
	return m_arraySoundArray;
}
//------------------------------------------------------------------------------
