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


class ReAnimItem;
class ReAnimFrame;
class ReAnimTrack;


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
	// Tracks & Frame.
	// -------------------------------------------------------------------------
public:
	// Track.
	ReAnimTrack*		CreateTrack( eTrackType _type );
	void				DeleteTrack( eTrackType _type );
	ReAnimTrack*		GetTrackByType( eTrackType _type ) const;
	ReAnimTrack*		GetTrackByIndex( int _index ) const;	
	int					GetTrackCount() const;
	int					GetTrackIndex( eTrackType _type ) const;

	// Frame.
	ReAnimFrame*		CreateFrame( eTrackType _trackType, qreal _time );
	void				DeleteFrame( eTrackType _trackType, ReAnimFrame* _frame );
	ReAnimFrame*		GetFrameByIndex( eTrackType _trackType, int _index ) const;

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	ReAnimItem*			ItemFromIndex( const QModelIndex& _index ) const;
	int					PositionToInsertTrack( eTrackType _trackType ) const;

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	ReAnimTrack*		m_tracks[ ETrackType_Count ];
};

}
#endif	// _RE_ANIM_MODEL_H_
