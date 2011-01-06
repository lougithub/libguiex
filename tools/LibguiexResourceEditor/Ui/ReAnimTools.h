// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101222.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_TOOLS_H_
#define _RE_ANIM_TOOLS_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReAnimDef.h"
#include "Core\ReDragInfo.h"


namespace RE
{


class ReAnimGraphicsItem;


// -----------------------------------------------------------------------------
// Animation tool base.
// -----------------------------------------------------------------------------
class ReAnimTool
{
public:
	ReAnimTool(): m_item( NULL ) {}
	virtual ~ReAnimTool() {}

	// -------------------------------------------------------------------------
	// Tool interface.
	// -------------------------------------------------------------------------
public:
	virtual void		Setup( ReAnimGraphicsItem* _item ) = 0;
	virtual void		Release();

	ReAnimGraphicsItem*	GetItem() const { return m_item; }


protected:
	ReAnimGraphicsItem*	m_item;
};


// -----------------------------------------------------------------------------
// Animation tool for selection.
// -----------------------------------------------------------------------------
class ReAnimSelectTool : public ReAnimTool
{
public:
	virtual void		Setup( ReAnimGraphicsItem* _item );
};



// -----------------------------------------------------------------------------
// Animation tool for translation.
// -----------------------------------------------------------------------------
class ReAnimMoveTool : public ReAnimTool
{
public:
	virtual void		Setup( ReAnimGraphicsItem* _item );
};


// -----------------------------------------------------------------------------
// Animation tool for rotation.
// -----------------------------------------------------------------------------
class ReAnimRotateTool : public ReAnimTool
{
public:
	virtual void		Setup( ReAnimGraphicsItem* _item );
};


// -----------------------------------------------------------------------------
// Animation tool for scaling.
// -----------------------------------------------------------------------------
class ReAnimScaleTool : public ReAnimTool
{
public:
	virtual void		Setup( ReAnimGraphicsItem* _item );
};


// -----------------------------------------------------------------------------
// Animation tool for alpha setting.
// -----------------------------------------------------------------------------
class ReAnimAlphaTool : public ReAnimTool
{
public:
	virtual void		Setup( ReAnimGraphicsItem* _item );
};


}
#endif	// _RE_ANIM_TOOLS_H_
