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
	ReDataNode( M* _model ) : m_model( _model ), m_parent( NULL ), m_zoomScalar( 1 ) {}
	virtual ~ReDataNode() {}

	M*					GetModel() const							{ return m_model; }
	void				SetModel( M* _model )						{ m_model = _model; }

	ReDataNode*			GetParent() const							{ return m_parent; }
	void				SetParent( ReDataNode* _parent )			{ m_parent = _parent; }

	void				SetName( const QString& _name )				{ m_name = _name; }
	const QString&		GetName() const								{ return m_name; }

	int					GetZoomScalar() const						{ return m_zoomScalar; }
	void				SetZoomScalar( int _scalar )				{ m_zoomScalar = _scalar; }

	virtual int			GetChildrenCount() const					{ return 0; }
	virtual ReDataNode*	GetChild( int _index )						{ return NULL; }
	virtual int			GetChildIndex( ReDataNode* _child ) const	{ return -1; }
	virtual void		DestroyChildren()							{}

	virtual QVariant	GetDetail() const							{ return QVariant(); }

protected:
	M*					m_model;
	ReDataNode*			m_parent;
	QString				m_name;
	int					m_zoomScalar;

};


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
template< class M >
class ReDataNodeGroup : public ReDataNode< M >
{
public:
	typedef ReDataNode< M >						TSuper;
	typedef ReItemGroup< ReDataNode< M >* >		TNodeList;
	typedef typename TNodeList::TItemListItor	TNodeListItor;
	typedef typename TNodeList::TItemListCItor	TNodeListCItor;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReDataNodeGroup( M* _model ): TSuper( _model ), m_nextId( 0 ) {}

	TNodeListItor		ChildBegin()		{ return m_children.Begin(); }
	TNodeListItor		ChildEnd()			{ return m_children.End(); }
	TNodeListCItor		ChildBegin() const	{ return m_children.Begin(); }
	TNodeListCItor		ChildEnd() const	{ return m_children.End(); }
	int					GetNextChildId()	{ return m_nextId++; }

	virtual void		AddNode( ReDataNode* _node )	{ m_children.Add( _node ); _node->SetParent( this ); }
	virtual void		RemoveNode( ReDataNode* _node )	{ m_children.Remove( _node ); _node->SetParent( this ); }	

	// -------------------------------------------------------------------------
	// Override ReDataNode.
	// -------------------------------------------------------------------------
public:
	virtual ~ReDataNodeGroup()										{ DestroyChildren(); }
	virtual int			GetChildrenCount() const					{ return m_children.Size(); }
	virtual ReDataNode*	GetChild( int _index )						{ return ( ReDataNode* )m_children.GetAt( _index ); }
	virtual int			GetChildIndex( ReDataNode* _child ) const	{ return m_children.Index( _child ); }
	virtual void		DestroyChildren()							{ m_children.Destroy(); }

protected:
	TNodeList			m_children;
	int					m_nextId;
};


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
#endif	// _RE_EDITOR_DATAMODEL_H_
