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
// CCommandWidgetProperty
//*********************************************** 
class CCommandWidgetProperty : public CCommandWidgetBase
{
public:
	CCommandWidgetProperty( CGUIWidget* pWidget );

	virtual void Execute();
	virtual void Undo();

protected:
	CGUIProperty m_ExecuteProp;
	CGUIProperty m_UndoProp;
};


//***********************************************
// CCommand_SetPosition
//*********************************************** 
class CCommand_SetPosition : public CCommandWidgetBase
{
public:
	CCommand_SetPosition( CGUIWidget* pWidget, const CGUIVector2& rPixelPos );

	virtual void Execute();
	virtual void Undo();

protected:
	CGUIVector2 m_aPosOld;
	CGUIVector2 m_aPosNew;
};

#endif //__KEN_COMMAND_20071130_H__


