// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110115.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReClipSheet.h"
#include "Ui\ReClipUiInfo.h"
#include "Core\ReClipModel.h"
#include <QVBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QStyledItemDelegate>


namespace
{
	class ReClipDelegate : public QStyledItemDelegate
	{
	public:
		virtual QSize sizeHint( const QStyleOptionViewItem& _option, const QModelIndex& _index ) const
		{
			return QSize( RE::CLIP_PREVIEW_WIDTH, RE::CLIP_PREVIEW_HEIGHT );
		}
	};
}


namespace RE
{


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
ReClipSheet::ReClipSheet( ReClipModel* _model, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
{
	m_clipView = new QTreeView( this );
	m_clipView->setAlternatingRowColors( true );
	m_clipView->setDragDropMode( QAbstractItemView::DragOnly );
	m_clipView->setModel( _model );
	m_clipView->setItemDelegate( new ReClipDelegate() );

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget( m_clipView, 1 );

	setLayout( layout );
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
