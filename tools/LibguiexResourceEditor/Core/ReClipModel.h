// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101119.
// -----------------------------------------------------------------------------
#ifndef _RE_CLIP_MODEL_H_
#define _RE_CLIP_MODEL_H_
// -----------------------------------------------------------------------------
// Data in a model can be viewed and modified by different entities with 
// different methods.
// -----------------------------------------------------------------------------
#include "Core\ReTypes.h"
#include "Core\ReModelBase.h"
#include "Core\ReDataModel.h"
#include "Core\ReZoomInfo.h"
#include <list>
#include <QAbstractItemModel>
#include <QPixmap>


class QMimeData;


namespace RE
{


class ReClipModel;
class ReZoomInfo;


// -----------------------------------------------------------------------------
// Clip.
// -----------------------------------------------------------------------------
class ReClipNode : public ReDataNode< ReClipModel >
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	typedef ReDataNode< ReClipModel >	TSuper;
	ReClipNode( ReClipModel* _model ): TSuper( _model ), m_offset( 0, 0 ), m_size( 0, 0 ), m_zoomScalar( 1 ) {}

	void				SetOffset( const QPoint& _offset );
	const QPoint&		GetOffset() const { return m_offset; }
	void				SetSize( const QSize& _size );
	const QSize&		GetSize() const { return m_size; }
	void				SetZoomScalar( int _scalar ) { m_zoomScalar = _scalar; }
	int					GetZoomScalar() const { return m_zoomScalar; }

	// -------------------------------------------------------------------------
	// Override ReDataNode.
	// -------------------------------------------------------------------------
public:
	virtual QVariant GetDetail() const
	{
		return QString( QObject::tr( "x:%1 y:%2\r\nw:%3 h:%4" ) )
			.arg( m_offset.x() / m_zoomScalar )
			.arg( m_offset.y() / m_zoomScalar )
			.arg( m_size.width() / m_zoomScalar )
			.arg( m_size.height() / m_zoomScalar );
	}

protected:
	QPoint				m_offset;
	QSize				m_size;
	int					m_zoomScalar;
};


// -----------------------------------------------------------------------------
// Clip group.
// -----------------------------------------------------------------------------
class ReClipNodeGroup : public ReDataNode< ReClipModel >
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	typedef ReDataNode< ReClipModel >	TSuper;
	ReClipNodeGroup( ReClipModel* _model ): TSuper( _model ), m_image( NULL ), m_zoomScalar( 1 ) {}
	~ReClipNodeGroup();

	void				SetImageId( const QString& _id );
	const QString&		GetImageId() const					{ return m_imageId; }
	QPixmap*			GetImage() const					{ return m_image; }
	void				SetZoomScalar( int _scalar )		{ m_zoomScalar = _scalar; }
	int					GetZoomScalar() const				{ return m_zoomScalar; }

	// -------------------------------------------------------------------------
	// Override ReDataNode.
	// -------------------------------------------------------------------------
public:
	virtual QVariant GetDetail() const
	{
		return m_imageId;
	}

protected:
	QString				m_imageId;
	QPixmap*			m_image;
	int					m_zoomScalar;
};


// -----------------------------------------------------------------------------
// Clip model.
// -----------------------------------------------------------------------------
class ReClipModel : public QAbstractItemModel
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	typedef QAbstractItemModel			TSuper;
	typedef Qt::ItemFlags				TFlags;
	typedef ReDataNode< ReClipModel >	TNode;

	enum eColumn
	{
		EColumn_Preview,
		EColumn_Name,
		EColumn_Detail,
		EColumn_Count
	};

	ReClipModel( QWidget* _parent = NULL );
	~ReClipModel();

	int					GetGroupCount() const { return m_rootNode->GetChildrenCount(); }
	ReClipNodeGroup*	GetGroupByIndex( int _index ) const;
	ReClipNodeGroup*	GetGroupById( const QString& _id );
	ReClipNodeGroup*	CreateGroup( const QString& _id );
	ReClipNode*			CreateClip( ReClipNodeGroup* _group );
	int					GetGroupIndex( ReClipNodeGroup* _group ) const;
	void				DestroyGroup( ReClipNodeGroup* _group );
	void				DestroyClip( ReClipNode* _clip );
	void				Clear();

	void				OnClipChanged( ReClipNode* _clip );
	void				OnClipDeleted( ReClipNode* _clip );
	bool				IsDirty() const;

	bool				Export( const QString& _filePath ) const;
	bool				Import( const QString& _filePath );

	// -------------------------------------------------------------------------
	// Override QAbstractItemModel.
	// -------------------------------------------------------------------------
public:
	virtual QVariant	data( const QModelIndex& _index, int _role = Qt::DisplayRole ) const;
	virtual bool		setData( const QModelIndex& _index, const QVariant& _value, int _role = Qt::EditRole );
	virtual QModelIndex	index( int _row, int _column, const QModelIndex& _parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& _index ) const;	
	virtual bool		insertRows( int _row, int _count, const QModelIndex& _parent = QModelIndex() );
	virtual bool		removeRows( int _row, int _count, const QModelIndex& _parent = QModelIndex() );
	virtual int			rowCount( const QModelIndex& _parent = QModelIndex() ) const;
	virtual int			columnCount( const QModelIndex& _parent = QModelIndex() ) const;
	virtual QVariant	headerData( int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole ) const;
	virtual QMimeData*	mimeData( const QModelIndexList& _indexes ) const;
	virtual TFlags		flags( const QModelIndex& _index ) const;

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	// Misc.
	TNode*				IndexToNode( const QModelIndex& _index ) const;

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
protected:
	ReClipNodeGroup*	m_rootNode;
};


}
#endif	// _RE_CLIP_MODEL_H_
