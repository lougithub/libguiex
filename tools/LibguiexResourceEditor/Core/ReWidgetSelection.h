// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110115.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_WIDGETSELECTION_H_
#define _RE_EDITOR_WIDGETSELECTION_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReItemGroup.h"
#include "Core\ReDragInfo.h"
#include <QWidget>


namespace RE
{


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class ReSelItem
{
public:
	ReSelItem( QWidget* _widget ): m_widget( _widget ) {}

	QWidget*			GetWidget() const	{ return m_widget; }
	ReDragInfo&			GetDragInfoRef()	{ return m_dragInfo; }

protected:
	QWidget*			m_widget;
	ReDragInfo			m_dragInfo;
};


class ReSelEqualPredicate
{
public:
	bool IsEqual( const ReSelItem& _a, const ReSelItem& _b )
	{
		return _a.GetWidget() == _b.GetWidget();
	}
};


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class ReWidgetSelection : public ReItemGroup< ReSelItem, ReSelEqualPredicate >
{
	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	typedef ReItemGroup< ReSelItem, ReSelEqualPredicate >	TSuper;

	enum eState
	{
		EState_Idle,
		EState_Wait,
		EState_Move
	};

	void				GotoIdleState()					{ m_state = EState_Idle; }
	void				GotoWaitState();
	void				GotoMoveState()					{ m_state = EState_Move; }
	bool				IsIdleState() const				{ return EState_Idle == m_state; }
	bool				IsWaitState() const				{ return EState_Wait == m_state; }
	bool				IsMoveState() const				{ return EState_Move == m_state; }

	ReDragInfo&			GetDragInfoRef()				{ return m_dragInfo; }
	void				MoveBy( const QPoint& _delta );
	void				DragMoveBy( const QPoint& _delta );

	// ----------------------------------------------------------------------------
	// Overload ReItemGroup.
	// ----------------------------------------------------------------------------
public:
	virtual void		Add( QWidget* _widget )			{ TSuper::Add( ReSelItem( _widget ) ); }
	virtual void		Remove( QWidget* _widget )		{ TSuper::Remove( ReSelItem( _widget ) ); }
	bool				Find( QWidget* _widget ) const	{ return TSuper::Find( ReSelItem( _widget ) ); }
	int					Index( QWidget* _widget ) const	{ return TSuper::Index( ReSelItem( _widget ) ); }

protected:
	ReDragInfo			m_dragInfo;
	eState				m_state;
};


}
#endif	// _RE_EDITOR_WIDGETSELECTION_H_
