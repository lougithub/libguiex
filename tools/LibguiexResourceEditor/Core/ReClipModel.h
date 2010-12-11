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
#include "Core\ReClipData.h"
#include <list>
#include <QStandardItemModel>


namespace RE
{


// Make sure all Qt super classes are listed before custom super classes.
// For this class, it wound fail the QAbstractItemView::setModel if we 
// do not follow this rule.
class ReClipModel : public QStandardItemModel, public ReModelBase< ReClipData >
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	typedef QStandardItemModel			TSuper;
	typedef ReModelBase< ReClipData >	TSuperB;

	enum eColumn
	{
		EColumn_Icon,
		//EColumn_Name,
		EColumn_X,
		EColumn_Y,
		EColumn_W,
		EColumn_H,
		EColumn_Count
	};

	ReClipModel( QWidget* _parent = NULL );

	// -------------------------------------------------------------------------
	// Override QStandardItemModel.
	// -------------------------------------------------------------------------
public:
	typedef Qt::ItemFlags	TFlags;
	virtual bool		setData( const QModelIndex& _index, const QVariant& _value, int _role = Qt::EditRole );
	virtual QVariant	data( const QModelIndex& _index, int _role = Qt::DisplayRole ) const;
	virtual TFlags		flags( const QModelIndex& _index ) const;// { return 0xFFFFFFFF; }

	// -------------------------------------------------------------------------
	// Override ReModelBase.
	// -------------------------------------------------------------------------
public:
	virtual ReClipData*	CreateData();
	virtual void		RecycleData( ReClipData* _clip );

	// -------------------------------------------------------------------------
	// Model specific.
	// -------------------------------------------------------------------------
public:
	void				SetDataFromItem( const QModelIndex& _index );
	void				SetImageHandle( TImageHandle _handle );
	TImageHandle		GetImageHandle() const { return m_imageHandle; }

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
protected:
	TImageHandle		m_imageHandle;
	QPixmap				m_pixmap;
};


}
#endif	// _RE_CLIP_MODEL_H_
