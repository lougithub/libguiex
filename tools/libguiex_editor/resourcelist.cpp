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
	ResetAnimationList();
	ResetAsList();
	ResetParticle2DList();
	ResetSoundList();
	ResetFontList();
	ResetLocalizationList();
	ResetTiledMapList();
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
	UpdateAnimationList();
	UpdateAsList();
	UpdateParticle2DList();
	UpdateSoundList();
	UpdateFontList();
	UpdateLocalizationList();
	UpdateTiledMapList();
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
void CResourceList::ResetAnimationList()
{
	m_arrayAnimationArray.Clear();

	for( std::map<wxString, wxBitmap*>::iterator itor = m_mapAnimationThumbnails.begin();
		itor != m_mapAnimationThumbnails.end();
		++itor )
	{
		delete itor->second;
	}
	m_mapAnimationThumbnails.clear();
	m_mapFullAnimationThumbnails.clear();
}
//------------------------------------------------------------------------------
void CResourceList::UpdateAnimationList()
{
	ResetAnimationList();

	m_arrayAnimationArray.Add(_T(""));
	const std::map<CGUIString, CGUIAnimationData*>& rMapAnimationList = CGUIAnimationManager::Instance()->GetRegisterResourceMap();
	for( std::map<CGUIString,CGUIAnimationData*>::const_iterator itor = rMapAnimationList.begin();
		itor != rMapAnimationList.end();
		++itor)
	{
		CGUIAnimationData* pGuiAnimationData = itor->second;
		const CGUIString& rGuiAnimationName = itor->first;
		wxString rWxAnimationName = Gui2wxString( rGuiAnimationName );

		m_arrayAnimationArray.Add( rWxAnimationName );

		// Create the image thumbnail
		CGUIImage* pGuiImage = CGUIImageManager::Instance()->AllocateResource(pGuiAnimationData->GetImages()[0]);
		wxImage* pWxImage = LoadwxImageByGuiImage( pGuiImage );
		if( pWxImage )
		{	
			m_mapFullAnimationThumbnails[rWxAnimationName] = pGuiAnimationData;

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
			m_mapAnimationThumbnails[rWxAnimationName] = pBitmap;

			delete pWxImage;
		}
		pGuiImage->RefRelease();
	}
	m_arrayAnimationArray.Sort();
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
		wxImage* pWxImage = LoadwxImageByGuiImage( pGuiImage );
		if( pWxImage )
		{
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
	m_arrayImageArray.Sort();
}
//------------------------------------------------------------------------------
wxBitmap* CResourceList::GetAnimationThumbnail( const wxString& rImageName ) const
{
	std::map<wxString, wxBitmap*>::const_iterator itor = m_mapAnimationThumbnails.find( rImageName );
	if( itor != m_mapAnimationThumbnails.end() )
	{
		return itor->second;
	}
	return NULL;
}
//------------------------------------------------------------------------------
CGUIAnimationData* CResourceList::GetFullAnimationThumbnail( const wxString& rImageName ) const
{
	std::map<wxString, CGUIAnimationData* >::const_iterator itor = m_mapFullAnimationThumbnails.find( rImageName );
	if( itor != m_mapFullAnimationThumbnails.end() )
	{
		return itor->second;
	}
	return NULL;
}
//------------------------------------------------------------------------------
wxBitmap* CResourceList::GetImageThumbnail( const wxString& rImageName )
{
	std::map<wxString, wxBitmap*>::iterator itor = m_mapImageThumbnails.find( rImageName );
	if( itor != m_mapImageThumbnails.end() )
	{
		return itor->second;
	}
	return NULL;
}
//------------------------------------------------------------------------------
wxBitmap* CResourceList::GetOriginalImageThumbnail( const wxString& rImageName )
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
void CResourceList::UpdateTiledMapList()
{
	ResetTiledMapList();

	m_arrayTiledMapArray.Add(_T(""));
	const std::map<CGUIString, CGUITiledMapData*>& rMapTiledMapDataList = CGUITiledMapManager::Instance()->GetRegisterResourceMap();
	for( std::map<CGUIString,CGUITiledMapData*>::const_iterator itor = rMapTiledMapDataList.begin();
		itor != rMapTiledMapDataList.end();
		++itor)
	{
		m_arrayTiledMapArray.Add(Gui2wxString( itor->first));
	}
	m_arrayTiledMapArray.Sort();
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
void CResourceList::ResetTiledMapList()
{
	m_arrayTiledMapArray.Clear();
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
void CResourceList::UpdateLocalizationList()
{
	ResetLocalizationList();

	m_arrayLocalizationArray.Add(_T(""));

	std::vector<CGUIString> m_tempArrays;
	CGUILocalizationManager::Instance()->GetAllLocalizations( m_tempArrays );

	for( std::vector<CGUIString>::const_iterator itor = m_tempArrays.begin();
		itor != m_tempArrays.end();
		++itor)
	{
		m_arrayLocalizationArray.Add(Gui2wxString( *itor));
	}
	m_arrayLocalizationArray.Sort();
}
//------------------------------------------------------------------------------
void CResourceList::ResetLocalizationList()
{
	m_arrayLocalizationArray.Clear();
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
void CResourceList::ResetFontList()
{
	m_arrayFontArray.Clear();
	m_mapFontDesc.clear();
}
//------------------------------------------------------------------------------
void CResourceList::UpdateFontList()
{
	ResetFontList();

	const std::map<CGUIString, CGUIFontData*>& rMapFontList = CGUIFontManager::Instance()->GetRegisterResourceMap();
	for( std::map<CGUIString,CGUIFontData*>::const_iterator itor = rMapFontList.begin();
		itor != rMapFontList.end();
		++itor)
	{
		wxString strFontID;
		strFontID<<itor->second->GetFontID();
		m_arrayFontArray.Add(strFontID);
		m_mapFontDesc.insert( std::make_pair(strFontID, Gui2wxString( itor->second->GetFontDesc())));
	}
	m_arrayFontArray.Sort();
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetImageList()
{
	return m_arrayImageArray;
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetAnimationList()
{
	return m_arrayAnimationArray;
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetAsList()
{
	return m_arrayAsArray;
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetTiledMapList()
{
	return m_arrayTiledMapArray;
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
const wxArrayString& CResourceList::GetFontList()
{
	return m_arrayFontArray;
}
//------------------------------------------------------------------------------
const wxString* CResourceList::GetFontDesc( const wxString& rFont )
{
	std::map<wxString, wxString>::iterator itor = m_mapFontDesc.find( rFont );
	if( itor != m_mapFontDesc.end() )
	{
		return &itor->second;
	}
	return NULL;
}
//------------------------------------------------------------------------------
const wxArrayString& CResourceList::GetLocalizationList()
{
	return m_arrayLocalizationArray;
}
//------------------------------------------------------------------------------
