// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101203.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_MODEL_H_
#define _RE_ANIM_MODEL_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <QList>
#include <QAbstractItemModel>
#include "Core\ReAnimDef.h"


namespace RE
{


class ReAnimNode;
class ReAnimFrame;
class ReAnimTrack;
class ReAnimEntity;


class ReAnimModel : public QAbstractItemModel
{
	Q_OBJECT
	typedef QAbstractItemModel	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	enum eColumn
	{
		EColumn_Key,
		EColumn_Value,
		EColumn_Count
	};

	ReAnimModel( QObject* _parent = NULL );

	qreal				GetTotalLength() const;

	// -------------------------------------------------------------------------
	// Override QAbstractItemModel.
	// -------------------------------------------------------------------------
public:
	virtual QModelIndex	index( int _row, int _column, const QModelIndex& _parent = QModelIndex() ) const;
	virtual QModelIndex	parent( const QModelIndex& _child ) const;
	virtual QVariant	data( const QModelIndex &index, int role /* = Qt::DisplayRole */ ) const;
	virtual bool		setData( const QModelIndex& _index, const QVariant& _value, int _role /* = Qt::EditRole */ );
	virtual int			rowCount( const QModelIndex& _parent /* = QModelIndex */ ) const;
	virtual int			columnCount( const QModelIndex& _parent /* = QModelIndex */ ) const;
	virtual QVariant	headerData( int _section, Qt::Orientation _orient, int _role = Qt::DisplayRole );
	virtual Qt::ItemFlags	flags( const QModelIndex& _index ) const;
	//virtual bool		insertRows( int _row, int _count, const QModelIndex& _parent = QModelIndex() );
	//virtual bool		removeRows( int _row, int _count, const QModelIndex& _parent = QModelIndex() );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	void				OnFrameChanged( eTrackType _trackType, ReAnimFrame* _frame );

	// -------------------------------------------------------------------------
	// Items & Tracks & Frames.
	// -------------------------------------------------------------------------
public:
	// Entity.
	ReAnimEntity*		CreateEntity();

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	ReAnimNode*			ItemFromIndex( const QModelIndex& _index ) const;
	//int					PositionToInsertTrack( eTrackType _trackType ) const;

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	//ReAnimTrack*		m_tracks[ ETrackType_Count ];

	typedef QList< ReAnimEntity* >		TItemList;
	typedef TItemList::iterator			TItemListItor;
	typedef TItemList::const_iterator	TItemListCItor;

	TItemList			m_items;
};

}
#endif	// _RE_ANIM_MODEL_H_
