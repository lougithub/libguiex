/** 
 * @file resourcelist.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2010-12-24
 */

#ifndef __KEN_RESOURCELIST_20101224_H__
#define	__KEN_RESOURCELIST_20101224_H__


//============================================================================//
// include
//============================================================================//
#include <wx/wxprec.h>
#include <libguiex_core/guiex.h>
#include <string>
#include <map>
#include <vector>

using namespace guiex;

// Preferred thumbnail height.
#define PREF_THUMBNAIL_HEIGHT       32

//============================================================================//
// class
//============================================================================//
class CResourceList
{
public:
	static CResourceList* Instance();

	void UpdateResourceList();

	const wxArrayString& GetAsList();
	const wxArrayString& GetParticle2DList();
	const wxArrayString& GetSoundList();
	const wxArrayString& GetLocalizationList();
	const wxArrayString& GetTiledMapList();

	const wxArrayString& GetFontList();
	const wxString* GetFontDesc( const wxString& rFont );

	const wxArrayString& GetImageList();
	wxBitmap* GetImageThumbnail( const wxString& rImageName );
	wxBitmap* GetOriginalImageThumbnail( const wxString& rImageName );

protected:
	//update image used in the system
	void UpdateImageList();
	void ResetImageList();

	//update as used in the system
	void UpdateAsList();
	void ResetAsList();

	//update particle 2d used in the system
	void UpdateParticle2DList();
	void ResetParticle2DList();

	//update tiledmap used in the system
	void UpdateTiledMapList();
	void ResetTiledMapList();

	//update sound used in the system
	void UpdateSoundList();
	void ResetSoundList();

	//update font used in the system
	void UpdateFontList();
	void ResetFontList();

	//update localization list used in the system
	void UpdateLocalizationList();
	void ResetLocalizationList();

protected:
	CResourceList();
	~CResourceList();

private:
	wxArrayString m_arrayImageArray;
	std::map<wxString, wxBitmap*> m_mapImageThumbnails;
	std::map<wxString, wxBitmap*> m_mapOriginalImageThumbnails;
	wxArrayString m_arrayAsArray;
	wxArrayString m_arrayParticle2DArray;
	wxArrayString m_arrayTiledMapArray;
	wxArrayString m_arraySoundArray;
	wxArrayString m_arrayFontArray;
	std::map<wxString,wxString> m_mapFontDesc;
	wxArrayString m_arrayLocalizationArray;
};


#endif //__KEN_RESOURCELIST_20101224_H__
