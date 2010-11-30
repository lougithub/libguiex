// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101128.
// -----------------------------------------------------------------------------
#ifndef _RE_WIDGET_INTERFACE_H_
#define _RE_WIDGET_INTERFACE_H_


class QMenu;


namespace RE
{


class ReWidgetInterface
{
public:
	virtual ~ReWidgetInterface()				{}
	virtual void		Tick( qreal _delta )	{}
	virtual QMenu*		GetEditMenu() const		{ return NULL; }
};


}
#endif	// _RE_WIDGET_INTERFACE_H_
