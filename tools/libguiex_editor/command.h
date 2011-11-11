/** 
* @file command.h
* @brief command
* @author ken
* @date 2007-11-30
*/

#ifndef __KEN_COMMAND_20071130_H__
#define __KEN_COMMAND_20071130_H__


//============================================================================//
// include
//============================================================================// 
#include <map>
#include <vector>
#include <libguiex_core/guiex.h>
#include "propertysheetfunc.h"

//============================================================================//
// declare
//============================================================================// 
using namespace guiex;



//============================================================================//
// class
//============================================================================// 



//***********************************************
// CCommand
//*********************************************** 
class CCommand
{
public:
	CCommand();
	virtual ~CCommand();

	virtual void Execute() = 0;
	virtual void Undo() = 0;
};

//***********************************************
// CCommandWidgetBase
//*********************************************** 
class CCommandWidgetBase : public CCommand
{
public:
	CCommandWidgetBase( CGUIWidget* pWidget );

	CGUIWidget* GetWidget();

protected:
	CGUIString m_strWidgetName;
};

//***********************************************
// CCommand_SetProperty
//*********************************************** 
class CCommand_SetProperty : public CCommandWidgetBase
{
public:
	CCommand_SetProperty( CGUIWidget* pWidget, const CGUIProperty& rNewProp );

	virtual void Execute();
	virtual void Undo();

protected:
	CGUIProperty m_OldProp;
	CGUIProperty m_NewProp;
};


//***********************************************
// CCommand_SetPosition
//*********************************************** 
class CCommand_SetPosition : public CCommandWidgetBase
{
public:
	CCommand_SetPosition( CGUIWidget* pWidget, const CGUIVector2& rPixelOldPos, const CGUIVector2& rPixelNewPos );

	virtual void Execute();
	virtual void Undo();

protected:
	CGUIVector2 m_aPosOld;
	CGUIVector2 m_aPosNew;
};

//***********************************************
// CCommand_SetSize
//*********************************************** 
class CCommand_SetSize : public CCommandWidgetBase
{
public:
	CCommand_SetSize( CGUIWidget* pWidget, const CGUISize& rPixelOldSize, const CGUISize& rPixelNewSize );

	virtual void Execute();
	virtual void Undo();

protected:
	CGUISize m_aSizeOld;
	CGUISize m_aSizeNew;
};


//***********************************************
// CCommand_DeleteWidget
//*********************************************** 
class CCommand_DeleteWidget : public CCommandWidgetBase
{
public:
	CCommand_DeleteWidget( CGUIWidget* pWidget );

	virtual void Execute();
	virtual void Undo();

protected:
	void SaveWidgetCache( CGUIWidget* pWidget );

protected:
	std::vector<CWidgetPropertyCache> m_vecPropertyCaches;
};


#endif //__KEN_COMMAND_20071130_H__


