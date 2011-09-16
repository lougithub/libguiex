/** 
* @file propertyconvertor.cpp
* @brief 
* @author ken
* @date 2011-01-08
*/

//============================================================================//
// include
//============================================================================// 
#include "propertyconvertor.h"
#include "toolsmisc.h"
#include "guiresourcepool.h"

#include "wxpgpropertyextend.h"
#include "propertyconfigmgr.h"

//============================================================================//
// declare
//============================================================================// 


//============================================================================//
// function
//============================================================================// 


class CEditorPropertyConvertorBase
{
public:
	virtual ~CEditorPropertyConvertorBase()
	{
	}

	int32 GetType() const
	{
		return m_nType;
	}

	wxPGProperty* GetPGProperty( wxPropertyGridManager* pSheetMgr, const std::string& rName, const std::string& rType )
	{
		for ( wxPGVIterator it = pSheetMgr->GetVIterator( wxPG_ITERATE_PROPERTIES ); !it.AtEnd(); it.Next() )
		{
			wxPGProperty* pProp = (wxPGProperty*) it.GetProperty();
			std::string* pType = (std::string*)pSheetMgr->GetPropertyClientData( pProp );
			if( !pType )
			{
				continue;
			}
			if( *pType == rType && 
				wx2GuiString( pProp->GetLabel()) == rName )
			{
				return pProp;
			}
		}
		return NULL;
	}

	void GuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, const CGUIProperty& aProp, bool bEnableIdenticalName)
	{
		wxPGProperty* pPGTop  = NULL;
		if( !bEnableIdenticalName )
		{
			pPGTop = GetPGProperty( pSheetMgr, aProp.GetName(), aProp.GetTypeAsString() );
		}
		DoGuiProperty2GridProperty( pSheetMgr, pPGCategory, pPGTop, aProp);
		//set type as client data
		if( pPGTop)
		{
			pSheetMgr->SetPropertyClientData( pPGTop, (void*)CPropertyConfigMgr::Instance()->GetTypePtr(aProp.GetTypeAsString()));

			if( CPropertyData::IsPropertyMustExist(aProp))
			{
				pSheetMgr->SetPropertyTextColour( pPGTop, wxColour(255,0,0));
			}	
			if( CPropertyData::IsPropertyReadOnly(aProp))
			{
				pSheetMgr->SetPropertyReadOnly( pPGTop, true);
			}
		}
	}
	void GridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		std::string  strName = pSheetMgr->GetPropertyName(pPGProperty).char_str(wxConvUTF8).data();
		std::string* pType = (std::string*)pSheetMgr->GetPropertyClientData( pPGProperty );
		wxASSERT( pType );

		rProperty.SetName( strName );
		rProperty.SetType( *pType );
		DoGridProperty2GuiProperty( pSheetMgr, pPGProperty, rProperty );
	}

protected:
	CEditorPropertyConvertorBase( int32 nPropertyType )
		:m_nType( nPropertyType )
	{
	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp) = 0;
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty ) = 0;
	void AddPgProperty(  wxPropertyGridManager* pSheetMgr,wxPGProperty* pPGCategory, wxPGProperty* pPGProperty )
	{
		if( !pPGCategory )
		{
			pPGCategory = pSheetMgr->Append( new wxPropertyCategory(L"Default") );
		}
		pSheetMgr->Insert( pPGCategory, -1, pPGProperty );
	}

private:
	int32 m_nType;
};


class CEditorPropertyConvertor_Vector2 : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Vector2()
		:CEditorPropertyConvertorBase( ePropertyType_Vector2 )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		CGUIVector2 aValue;
		PropertyToValue(aProp, aValue);
		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = new WxGUIVector2Property( Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIVector2 aValue = CGUIVector2RefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
};

class CEditorPropertyConvertor_Vector3 : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Vector3()
		:CEditorPropertyConvertorBase( ePropertyType_Vector3 )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		CGUIVector3 aValue;
		PropertyToValue(aProp, aValue);

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = new WxGUIVector3Property( Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIVector3 aValue = CGUIVector3RefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
};

class CEditorPropertyConvertor_Rotator : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Rotator()
		:CEditorPropertyConvertorBase( ePropertyType_Rotator )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		CGUIRotator aValue;
		PropertyToValue(aProp, aValue);

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = new WxGUIRotatorProperty( Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIRotator aValue = CGUIRotatorRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
};

class CEditorPropertyConvertor_Rect : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Rect()
		:CEditorPropertyConvertorBase( ePropertyType_Rect )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		CGUIRect aValue;
		PropertyToValue(aProp, aValue);
			
		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = new WxGUIRectProperty( Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIRect aValue = CGUIRectRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
};


class CEditorPropertyConvertor_Color : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Color()
		:CEditorPropertyConvertorBase( ePropertyType_Color )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aProp) );
		}
		else
		{
			pPGTop = new WxGuiColorProperty( Gui2wxString(aProp.GetName()), aProp );
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		rProperty = CGUIPropertyRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
	}
};


class CEditorPropertyConvertor_Size : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Size()
		:CEditorPropertyConvertorBase( ePropertyType_Size )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aProp));
		}
		else
		{
			pPGTop = new WxGUISizeProperty(Gui2wxString(aProp.GetName()), aProp);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		rProperty = CGUIPropertyRefFromVariant(pSheetMgr->GetPropertyValue( pPGProperty ));
	}
};


class CEditorPropertyConvertor_String : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_String()
		:CEditorPropertyConvertorBase( ePropertyType_String )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue( aValue );
		}
		else
		{
			pPGTop = new wxStringProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};

class CEditorPropertyConvertor_LocalizedString : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_LocalizedString()
		:CEditorPropertyConvertorBase( ePropertyType_LocalizedString )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue( aValue );
		}
		else
		{
			pPGTop = new WxGUILocalizedStringProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};


class CEditorPropertyConvertor_Event : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Event()
		:CEditorPropertyConvertorBase( ePropertyType_Event )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue( aValue );
		}
		else
		{
			pPGTop = new wxStringProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};


class CEditorPropertyConvertor_StringInfo : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_StringInfo()
		:CEditorPropertyConvertorBase( ePropertyType_StringRenderInfo )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aProp ));
		}
		else
		{
			pPGTop = new WxGUIStringInfoProperty(Gui2wxString(aProp.GetName()), aProp);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		rProperty = CGUIPropertyRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
	}
};



class CEditorPropertyConvertor_WidgetPosition : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_WidgetPosition()
		:CEditorPropertyConvertorBase( ePropertyType_WidgetPosition )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aProp ));
		}
		else
		{
			pPGTop = new WxGUIWidgetPositionProperty(Gui2wxString(aProp.GetName()), aProp);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		rProperty = CGUIPropertyRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
	}
};


class CEditorPropertyConvertor_WidgetSize : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_WidgetSize()
		:CEditorPropertyConvertorBase( ePropertyType_WidgetSize )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aProp ));
		}
		else
		{
			pPGTop = new WxGUIWidgetSizeProperty(Gui2wxString(aProp.GetName()), aProp);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		rProperty = CGUIPropertyRefFromVariant(pSheetMgr->GetPropertyValue( pPGProperty ));
	}
};


class CEditorPropertyConvertor_Animation : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Animation()
		:CEditorPropertyConvertorBase( ePropertyType_Animation )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		wxString aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aValue ));
		}
		else
		{
			pPGTop = new WxGUIAnimationProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};



class CEditorPropertyConvertor_Image : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Image()
		:CEditorPropertyConvertorBase( ePropertyType_Image )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		wxString aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aValue ));
		}
		else
		{
			pPGTop = new WxGUIImageProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};



class CEditorPropertyConvertor_As : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_As()
		:CEditorPropertyConvertorBase( ePropertyType_As )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			pPGTop = new wxEnumProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()),CGUIResourcePool::Instance()->GetAsList());
			pPGTop->SetValue(aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};

class CEditorPropertyConvertor_Particle2D : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Particle2D()
		:CEditorPropertyConvertorBase( ePropertyType_Particle2D )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			pPGTop = new wxEnumProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()),CGUIResourcePool::Instance()->GetParticle2DList());
			pPGTop->SetValue(aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};


class CEditorPropertyConvertor_TiledMap : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_TiledMap()
		:CEditorPropertyConvertorBase( ePropertyType_TiledMap )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			pPGTop = new wxEnumProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()),CGUIResourcePool::Instance()->GetTiledMapList());
			pPGTop->SetValue(aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};

class CEditorPropertyConvertor_Sound : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Sound()
		:CEditorPropertyConvertorBase( ePropertyType_Sound )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			pPGTop = new WxGUISoundProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};



class CEditorPropertyConvertor_ScreenValue : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_ScreenValue()
		:CEditorPropertyConvertorBase( ePropertyType_ScreenValue )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		EScreenValue aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = new wxEnumProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), arrEnums);
			pPGTop->SetValue(aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};


class CEditorPropertyConvertor_TextAlignmentHorz : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_TextAlignmentHorz()
		:CEditorPropertyConvertorBase( ePropertyType_TextAlignmentHorz )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		ETextAlignmentHorz aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = new wxEnumProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), arrEnums);
			pPGTop->SetValue(aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};


class CEditorPropertyConvertor_TextAlignmentVert : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_TextAlignmentVert()
		:CEditorPropertyConvertorBase( ePropertyType_TextAlignmentVert )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		ETextAlignmentVert aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = new wxEnumProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), arrEnums);
			pPGTop->SetValue(aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};



class CEditorPropertyConvertor_ImageOrientation : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_ImageOrientation()
		:CEditorPropertyConvertorBase( ePropertyType_ImageOrientation )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		EImageOrientation aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = new wxEnumProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), arrEnums);
			pPGTop->SetValue(aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};



class CEditorPropertyConvertor_Orientation : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Orientation()
		:CEditorPropertyConvertorBase( ePropertyType_Orientation )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		EOrientation aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = new wxEnumProperty(Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()), arrEnums);
			pPGTop->SetValue(aValue);
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		CGUIString aValue = wx2GuiString(pSheetMgr->GetPropertyValueAsString( pPGProperty ));
		rProperty.SetValue( aValue );
	}
};

class CEditorPropertyConvertor_Real : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Real()
		:CEditorPropertyConvertorBase( ePropertyType_Real )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		real aValue = 0.f;
		PropertyToValue( aProp, aValue );
		
		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
		else
		{
			pPGTop = new wxFloatProperty( Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()),aValue );
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}		
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		real aValue = pSheetMgr->GetPropertyValueAsDouble(pPGProperty);
		ValueToProperty( aValue, rProperty );
	}
};

class CEditorPropertyConvertor_UInt32 : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_UInt32()
		:CEditorPropertyConvertorBase( ePropertyType_UInt32 )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		uint32 aValue = 0;
		PropertyToValue( aProp, aValue );
		
		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, int(aValue));
		}
		else
		{
			pPGTop = new wxIntProperty( Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()),aValue );
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}			
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		uint32 aValue = pSheetMgr->GetPropertyValueAsInt(pPGProperty);
		ValueToProperty( aValue, rProperty );
	}
};


class CEditorPropertyConvertor_Bool : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Bool()
		:CEditorPropertyConvertorBase( ePropertyType_Bool )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		bool aValue = false;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
		else
		{
			pPGTop = new wxBoolProperty( Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()),aValue );
			pPGTop->SetAttribute( wxPG_BOOL_USE_CHECKBOX,(long)1 );
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}				
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		bool aValue = pSheetMgr->GetPropertyValueAsBool(pPGProperty);
		ValueToProperty( aValue, rProperty );
	}
};


class CEditorPropertyConvertor_Int32 : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Int32()
		:CEditorPropertyConvertorBase( ePropertyType_Int32 )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		int32 aValue = 0;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( int(aValue));
		}
		else
		{
			pPGTop = new wxIntProperty( Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()),aValue );
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}					
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		int32 aValue = pSheetMgr->GetPropertyValueAsInt(pPGProperty);
		ValueToProperty( aValue, rProperty );
	}
};

//------------------------------------------------------------------------------
class CEditorPropertyConvertor_Int16 : public CEditorPropertyConvertorBase
{
public:
	CEditorPropertyConvertor_Int16()
		:CEditorPropertyConvertorBase( ePropertyType_Int16 )
	{

	}
	virtual void DoGuiProperty2GridProperty(wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, wxPGProperty*& pPGTop, const CGUIProperty& aProp)
	{
		int16 aValue = 0;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( int(aValue));
		}
		else
		{
			pPGTop = new wxIntProperty( Gui2wxString(aProp.GetName()), Gui2wxString(aProp.GetName()),aValue );
			AddPgProperty( pSheetMgr, pPGCategory, pPGTop);
		}					
	}
	virtual void DoGridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
	{
		int16 aValue = pSheetMgr->GetPropertyValueAsInt(pPGProperty);
		ValueToProperty( aValue, rProperty );
	}
};

//------------------------------------------------------------------------------
CPropertyConvertorMgr::CPropertyConvertorMgr()
{
	RegisterConvertor( new CEditorPropertyConvertor_Vector2 );
	RegisterConvertor( new CEditorPropertyConvertor_Vector3 );
	RegisterConvertor( new CEditorPropertyConvertor_Rotator );
	RegisterConvertor( new CEditorPropertyConvertor_Rect );
	RegisterConvertor( new CEditorPropertyConvertor_Color );
	RegisterConvertor( new CEditorPropertyConvertor_Size );
	RegisterConvertor( new CEditorPropertyConvertor_String );
	RegisterConvertor( new CEditorPropertyConvertor_LocalizedString );
	RegisterConvertor( new CEditorPropertyConvertor_Event );
	RegisterConvertor( new CEditorPropertyConvertor_StringInfo );
	RegisterConvertor( new CEditorPropertyConvertor_WidgetPosition );
	RegisterConvertor( new CEditorPropertyConvertor_WidgetSize );
	RegisterConvertor( new CEditorPropertyConvertor_Image );
	RegisterConvertor( new CEditorPropertyConvertor_Animation );
	RegisterConvertor( new CEditorPropertyConvertor_As );
	RegisterConvertor( new CEditorPropertyConvertor_Particle2D );
	RegisterConvertor( new CEditorPropertyConvertor_TiledMap );
	RegisterConvertor( new CEditorPropertyConvertor_Sound );
	RegisterConvertor( new CEditorPropertyConvertor_ScreenValue );
	RegisterConvertor( new CEditorPropertyConvertor_TextAlignmentHorz );
	RegisterConvertor( new CEditorPropertyConvertor_TextAlignmentVert );
	RegisterConvertor( new CEditorPropertyConvertor_ImageOrientation );
	RegisterConvertor( new CEditorPropertyConvertor_Orientation );
	RegisterConvertor( new CEditorPropertyConvertor_Real );
	RegisterConvertor( new CEditorPropertyConvertor_UInt32 );
	RegisterConvertor( new CEditorPropertyConvertor_Bool );
	RegisterConvertor( new CEditorPropertyConvertor_Int32 );
	RegisterConvertor( new CEditorPropertyConvertor_Int16 );
}
//------------------------------------------------------------------------------
CPropertyConvertorMgr::~CPropertyConvertorMgr()
{
	for( TMapConvertor::iterator itor = m_mapConvertor.begin();
		itor != m_mapConvertor.end();
		++itor )
	{
		delete itor->second;
	}
	m_mapConvertor.clear();
}
//------------------------------------------------------------------------------
CPropertyConvertorMgr* CPropertyConvertorMgr::Instance()
{
	static CPropertyConvertorMgr sInstance;
	return &sInstance;
}
//------------------------------------------------------------------------------
void CPropertyConvertorMgr::RegisterConvertor( class CEditorPropertyConvertorBase* pConvertor )
{
	assert( pConvertor );
	if( m_mapConvertor.find(pConvertor->GetType()) != m_mapConvertor.end() )
	{
		throw CGUIException("failed to register convertor!");
		return;
	}
	m_mapConvertor[pConvertor->GetType()] = pConvertor;
}
//------------------------------------------------------------------------------
void CPropertyConvertorMgr::GuiProperty2GridProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, const CGUIProperty& aProp, bool bEnableIdenticalName )
{
	TMapConvertor::iterator itor = m_mapConvertor.find( aProp.GetType() );
	if( itor == m_mapConvertor.end() )
	{
		std::string strError = std::string( "unsupported property type:  " )+aProp.GetTypeAsString();
		wxMessageBox( Gui2wxString(strError), _T("Error") );
		throw CGUIException( "unsupported property type: <%s>", aProp.GetType() );
		return;
	}
	itor->second->GuiProperty2GridProperty( pSheetMgr, pPGCategory, aProp, bEnableIdenticalName );
}
//------------------------------------------------------------------------------
void CPropertyConvertorMgr::GridProperty2GuiProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
{
	std::string* pType = (std::string*)pSheetMgr->GetPropertyClientData( pPGProperty );
	uint32 nType = CGUIPropertyManager::Instance()->StringToPropertyType( *pType );
	TMapConvertor::iterator itor = m_mapConvertor.find( nType );
	if( itor == m_mapConvertor.end() )
	{
		std::string strError = std::string("unsupported property type:  ")+pType->c_str();
		wxMessageBox(Gui2wxString(strError), _T("Error") );
		throw CGUIException("unsupported property type: <%s>", pType->c_str());
		return;
	}
	itor->second->GridProperty2GuiProperty( pSheetMgr, pPGProperty, rProperty );
}
//------------------------------------------------------------------------------

