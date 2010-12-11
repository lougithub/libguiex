// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101120.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReClipPanelWidget.h"
#include "Core\ReClipModel.h"
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>

namespace RE
{


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
ReClipPanelWidget::ReClipPanelWidget( ReClipModel* _model, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
{
	m_clipTableView = new QTableView( this );
	QHeaderView* headerView = m_clipTableView->horizontalHeader();
	headerView->setResizeMode( QHeaderView::Interactive );
	headerView->setStretchLastSection( true );

	QItemSelectionModel* selModel = new QItemSelectionModel( _model );
	m_clipTableView->setModel( /*( QStandardItemModel* )*/_model );
	m_clipTableView->setSelectionModel( selModel );

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget( m_clipTableView, 1 );

	setLayout( layout );
}


//void ReClipPanelWidget::SetClipMode( ReClipModel* _model )
//{
//	QItemSelectionModel* selModel = new QItemSelectionModel( _model );
//	m_clipTableView->setModel( ( QStandardItemModel* )_model );
//	m_clipTableView->setSelectionModel( selModel );
//}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
