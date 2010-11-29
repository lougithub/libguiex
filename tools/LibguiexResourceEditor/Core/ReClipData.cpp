// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101119.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReClipData.h"
#include "Core\ReClipModel.h"


namespace RE
{


// -------------------------------------------------------------------------
// General.
// -------------------------------------------------------------------------
ReClipData::ReClipData()
: m_isFlushingData( false )
{

}


// -------------------------------------------------------------------------
// Override.
// -------------------------------------------------------------------------
void ReClipData::setData( const QVariant& _value, int _role /* = Qt::UserRole + 1 */ )
{
	// In case we are flushing data to model, we should skip the setData requests
	// looped back.
	if( !m_isFlushingData )
	{
		QStandardItemModel* model = TSuper::model();
		QModelIndex idx = index();
		qreal x = model->data( model->index( idx.row(), 0, QModelIndex() ) ).toDouble();
		qreal y = model->data( model->index( idx.row(), 1, QModelIndex() ) ).toDouble();
		qreal w = model->data( model->index( idx.row(), 2, QModelIndex() ) ).toDouble();
		qreal h = model->data( model->index( idx.row(), 3, QModelIndex() ) ).toDouble();

		if( x >= 0.0f && x < 1.0f )
			m_offset.setX( x );
		if( y >= 0.0f && y < 1.0f )
			m_offset.setY( y );
		if( w > 0.0f && w <= 1.0f )
			m_size.setWidth( w );
		if( h > 0.0f && h <= 1.0f )
			m_size.setWidth( h );

		emitDataChanged();
	}
}


// -------------------------------------------------------------------------
// Interface.
// -------------------------------------------------------------------------
void ReClipData::Flush()
{
	m_isFlushingData = true;

	ReClipModel* model = dynamic_cast< ReClipModel* >( TSuper::model() );
	model->SetDataFromItem( index() );

	m_isFlushingData = false;
}


}
