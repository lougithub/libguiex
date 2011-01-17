// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110115.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAnimSheet.h"
#include "Core\ReAnimModel.h"
#include <QVBoxLayout>
#include <QTreeView>
#include <QHeaderView>


namespace RE
{


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
ReAnimSheet::ReAnimSheet( ReAnimModel* _model, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
{
	m_view = new QTreeView( this );
	m_view->setAlternatingRowColors( true );
	//QHeaderView* headerView = m_view->horizontalHeader();
	//headerView->setResizeMode( QHeaderView::Interactive );
	//headerView->setStretchLastSection( true );

	//QItemSelectionModel* selModel = new QItemSelectionModel( _model );
	m_view->setModel( _model );
	//m_view->setSelectionModel( selModel );

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget( m_view, 1 );

	setLayout( layout );
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
