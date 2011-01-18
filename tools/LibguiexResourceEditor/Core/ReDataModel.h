// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110115.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_DATAMODEL_H_
#define _RE_EDITOR_DATAMODEL_H_


#include "Core\ReItemGroup.h"
#include <QString>
#include <QVariant>


namespace RE
{


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
template< class M >
class ReDataNode
{
public:
	typedef ReItemGroup< ReDataNode< M >* >		TNodeList;
	typedef typename TNodeList::TItemListItor	TNodeListItor;
	typedef typename TNodeList::TItemListCItor	TNodeListCItor;

	ReDataNode( M* _model ) : m_model( _model ), m_parent( NULL ), m_nextId( 0 ) {}
	virtual ~ReDataNode() { DestroyChildren(); }

	M*					GetModel() const							{ return m_model; }
	void				SetModel( M* _model )						{ m_model = _model; }

	ReDataNode*			GetParent() const							{ return m_parent; }
	void				SetParent( ReDataNode* _parent )			{ m_parent = _parent; }

	void				SetName( const QString& _name )				{ m_name = _name; }
	const QString&		GetName() const								{ return m_name; }

	virtual void		AddChild( ReDataNode* _node )				{ m_children.Add( _node ); _node->SetParent( this ); ++m_nextId; }
	virtual void		RemoveChild( ReDataNode* _node )			{ m_children.Remove( _node ); _node->SetParent( this ); }	
	virtual int			GetChildrenCount() const					{ return m_children.Size(); }
	virtual ReDataNode*	GetChild( int _index )						{ return m_children.GetAt( _index ); }
	virtual int			GetChildIndex( ReDataNode* _child ) const	{ return m_children.Index( _child ); }
	virtual void		DestroyChildren()							{ m_children.Destroy(); }

	int					GetNextChildId() const						{ return m_nextId; }
	virtual QVariant	GetDetail() const							{ return QVariant(); }

protected:
	M*					m_model;
	ReDataNode*			m_parent;
	QString				m_name;
	TNodeList			m_children;
	int					m_nextId;
};


}
#endif	// _RE_EDITOR_DATAMODEL_H_
