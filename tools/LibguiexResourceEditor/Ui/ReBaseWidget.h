// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#ifndef _RE_BASE_WIDGET_H_
#define _RE_BASE_WIDGET_H_

#include <QWidget>
#include "Ui\ReWidgetInterface.h"
#include "Core\ReDragInfo.h"


class QMenu;


namespace RE
{


template< class T >
class ReBaseWidget : public T, public ReWidgetInterface
{
//	Q_OBJECT
//	typedef T	TSuper;

	// -----------------------------------------------------
	// General
	// -----------------------------------------------------
public:
	ReBaseWidget( QWidget* _parent = NULL );

	ReDragInfo&			GetDragInfoRef()					{ return m_dragInfo; }
	const ReDragInfo&	GetDragInfo() const					{ return m_dragInfo; }

protected:
	ReDragInfo			m_dragInfo;
};


template< class T >
ReBaseWidget< T >::ReBaseWidget( QWidget* _parent /* = NULL */ )
: T( _parent )
{

}


}		// namespace RE
#endif	// _RE_BASE_WIDGET_H_
