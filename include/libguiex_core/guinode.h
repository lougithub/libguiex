/** 
* @file guinode.h
* @brief A node contains information about the 
* transformation which will apply to it and all of it's children
* @author ken
* @date 2007-11-23
*/

#ifndef __GUI_NODE_20071123_H__
#define __GUI_NODE_20071123_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guimatrix3.h"
#include "guimatrix4.h"
#include "guiquaternion.h"
#include "guistring.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/** Class representing a general-purpose node an articulated scene graph.
	@remarks A node in the scene graph is a node in a structured tree. A node contains
	information about the transformation which will apply to
	it and all of it's children. Child nodes can have transforms of their own, which
	are combined with their parent's transformations.
	*/
	class GUIEXPORT CGUINode
	{
	public:
		/** Enumeration denoting the spaces which a transform can be relative to.
		*/
		enum ETransformSpace
		{
			eTS_LOCAL,		//!< Transform is relative to the local space
			eTS_PARENT,		//!< Transform is relative to the space of the parent node
			eTS_WORLD		//!< Transform is relative to world space
		};

	public:
		//!< constructor
		CGUINode( );

		//!< destructor
		virtual ~CGUINode();  

		//!< Gets this node's parent (NULL if this is the root).
		virtual CGUINode* GetParent(void) const;



		//!< Returns a quaternion representing the nodes orientation.
		virtual const CGUIQuaternion & getOrientation() const;

		//!< Sets the orientation of this node via a quaternion.
		virtual void setOrientation( const CGUIQuaternion& q );

		//!< Sets the orientation of this node via quaternion parameters.
		virtual void setOrientation( real w, real x, real y, real z);

		//!< Resets the nodes orientation (local axes as world axes, no rotation).
		virtual void resetOrientation(void);



		//!< Sets the position of the node relative to it's parent.
		virtual void setPosition(const CGUIVector3& pos);

		//!< Sets the position of the node relative to it's parent.
		virtual void setPosition(real x, real y, real z);

		//!< Gets the position of the node relative to it's parent.
		virtual const CGUIVector3 & getPosition(void) const;




		/** Sets the scaling factor applied to this node.
		@remarks Scaling factors, unlike other transforms, are not always inherited by child nodes. 
		Whether or not scalings affect both the size and position of the child nodes depends on
		the setInheritScale option of the child. In some cases you want a scaling factor of a parent node
		to apply to a child node (e.g. where the child node is a part of the same object, so you
		want it to be the same relative size and position based on the parent's size), but
		not in other cases (e.g. where the child node is just for positioning another object,
		you want it to maintain it's own size and relative position). The default is to inherit
		as with other transforms.
		@par Note that like rotations, scalings are oriented around the node's origin.
		*/
		virtual void setScale(const CGUIVector3& scale);

		/** Sets the scaling factor applied to this node.
		@remarks Scaling factors, unlike other transforms, are not always inherited by child nodes. 
		Whether or not scalings affect both the size and position of the child nodes depends on
		the setInheritScale option of the child. In some cases you want a scaling factor of a parent node
		to apply to a child node (e.g. where the child node is a part of the same object, so you
		want it to be the same relative size and position based on the parent's size), but
		not in other cases (e.g. where the child node is just for positioning another object,
		you want it to maintain it's own size and relative position). The default is to inherit
		as with other transforms.
		@par Note that like rotations, scalings are oriented around the node's origin.
		*/
		virtual void setScale(real x, real y, real z);

		/** Gets the scaling factor of this node.
		*/
		virtual const CGUIVector3 & getScale(void) const;

		/** Tells the node whether it should inherit scaling factors from it's parent node.
		@remarks
		Scaling factors, unlike other transforms, are not always inherited by child nodes. 
		Whether or not scalings affect both the size and position of the child nodes depends on
		the setInheritScale option of the child. In some cases you want a scaling factor of a parent node
		to apply to a child node (e.g. where the child node is a part of the same object, so you
		want it to be the same relative size and position based on the parent's size), but
		not in other cases (e.g. where the child node is just for positioning another object,
		you want it to maintain it's own size and relative position). The default is to inherit
		as with other transforms.
		@param inherit If true, this node's scale and position will be affected by its parent's scale. If false,
		it will not be affected.
		*/
		virtual void setInheritScale(bool inherit);

		/** Returns true if this node is affected by scaling factors applied to the parent node. 
		@remarks See setInheritScale for more info.
		*/
		virtual bool getInheritScale(void) const;

		/** Scales the node, combining it's current scale with the passed in scaling factor. 
		@remarks This method applies an extra scaling factor to the node's existing scale, (unlike setScale
		which overwrites it) combining it's current scale with the new one. E.g. calling this 
		method twice with CGUIVector3(2,2,2) would have the same effect as setScale(CGUIVector3(4,4,4)) if
		the existing scale was 1.
		@par Note that like rotations, scalings are oriented around the node's origin.
		*/
		virtual void scale(const CGUIVector3& scale);

		/** Scales the node, combining it's current scale with the passed in scaling factor. 
		@remarks This method applies an extra scaling factor to the node's existing scale, (unlike setScale
		which overwrites it) combining it's current scale with the new one. E.g. calling this 
		method twice with CGUIVector3(2,2,2) would have the same effect as setScale(CGUIVector3(4,4,4)) if
		the existing scale was 1.
		@par Note that like rotations, scalings are oriented around the node's origin.
		*/
		virtual void scale(real x, real y, real z);

		/** Moves the node along the cartesian axes.
		@par This method moves the node by the supplied vector along the
		world cartesian axes, i.e. along world x,y,z
		@param d Vector with x,y,z values representing the translation.
		@param relativeTo The space which this transform is relative to.
		*/
		virtual void translate(const CGUIVector3& d, ETransformSpace relativeTo = eTS_PARENT);

		/** Moves the node along the cartesian axes.
		@par This method moves the node by the supplied vector along the
		world cartesian axes, i.e. along world x,y,z
		@param x
		@param y
		@param z real x, y and z values representing the translation.
		@param relativeTo The space which this transform is relative to.
		*/
		virtual void translate(real x, real y, real z, ETransformSpace relativeTo = eTS_PARENT);

		/** Moves the node along arbitrary axes.
		@remarks This method translates the node by a vector which is relative to
		a custom set of axes.
		@param axes A 3x3 Matrix containg 3 column vectors each representing the
		axes X, Y and Z respectively. In this format the standard cartesian
		axes would be expressed as:
		<pre>
		1 0 0
		0 1 0
		0 0 1
		</pre>
		i.e. the identity matrix.
		@param move Vector relative to the axes above.
		@param relativeTo The space which this transform is relative to.
		*/
		virtual void translate(const CGUIMatrix3& axes, const CGUIVector3& move, ETransformSpace relativeTo = eTS_PARENT);

		/** Moves the node along arbitrary axes.
		@remarks This method translates the node by a vector which is relative to
		a custom set of axes.
		@param axes A 3x3 Matrix containg 3 column vectors each representing the
		axes X, Y and Z respectively. In this format the standard cartesian
		axes would be expressed as
		<pre>
		1 0 0
		0 1 0
		0 0 1
		</pre>
		i.e. the identity matrix.
		@param x,y,z Translation components relative to the axes above.
		@param relativeTo The space which this transform is relative to.
		*/
		virtual void translate(const CGUIMatrix3& axes, real x, real y, real z, ETransformSpace relativeTo = eTS_PARENT);



		//!< Rotate the node around the Z-axis.
		virtual void roll(const CGUIRadian& angle, ETransformSpace relativeTo = eTS_LOCAL);
		inline void roll(real degrees, ETransformSpace relativeTo = eTS_LOCAL)
		{
			roll ( CGUIAngle(degrees), relativeTo );
		}


		//!< Rotate the node around the X-axis.
		virtual void pitch(const CGUIRadian& angle, ETransformSpace relativeTo = eTS_LOCAL);
		inline void pitch(real degrees, ETransformSpace relativeTo = eTS_LOCAL) 
		{
			pitch ( CGUIAngle(degrees), relativeTo );
		}

		//!< Rotate the node around the Y-axis.
		virtual void yaw(const CGUIRadian& angle, ETransformSpace relativeTo = eTS_LOCAL);
		inline void yaw(real degrees, ETransformSpace relativeTo = eTS_LOCAL)
		{
			yaw ( CGUIAngle(degrees), relativeTo );
		}

		//!< Rotate the node around an arbitrary axis.
		virtual void rotate(const CGUIVector3& axis, const CGUIRadian& angle, ETransformSpace relativeTo = eTS_LOCAL);
		inline void rotate(const CGUIVector3& axis, real degrees, ETransformSpace relativeTo = eTS_LOCAL) 
		{
			rotate ( axis, CGUIAngle(degrees), relativeTo );
		}

		//!< Rotate the node around an aritrary axis using a Quarternion.
		virtual void rotate(const CGUIQuaternion& q, ETransformSpace relativeTo = eTS_LOCAL);



		/** Gets a matrix whose columns are the local axes based on
		the nodes orientation relative to it's parent. */
		virtual CGUIMatrix3 getLocalAxes(void) const;

		/** Gets the orientation of the node as derived from all parents.
		*/
		virtual const CGUIQuaternion & getDerivedOrientation(void);

		/** Gets the position of the node as derived from all parents.
		*/
		virtual const CGUIVector3 & getDerivedPosition(void);

		/** Gets the scaling factor of the node as derived from all parents.
		*/
		virtual const CGUIVector3 & getDerivedScale(void);



		/** Gets the full transformation matrix for this node.
		@remarks This method returns the full transformation matrix
		for this node, including the effect of any parent node
		transformations
		*/
		virtual const CGUIMatrix4& getFullTransform(void);

		//!< get the full inverse transform matrix of this node
		virtual const CGUIMatrix4& getFullInverseTransform(void);

		/** 
		@remarks This is only used if the SceneManager chooses to render the node.
		*/
		void getWorldTransforms(CGUIMatrix4* xform);

		/** Gets the world orientation of this node; this is used in order to
		more efficiently update parameters to vertex & fragment programs, since inverting Quaterion
		and Vector in order to derive object-space positions / directions for cameras and
		lights is much more efficient than inverting a complete 4x4 matrix, and also 
		eliminates problems introduced by scaling. */
		const CGUIQuaternion& getWorldOrientation(void);

		/** Gets the world orientation of this node; this is used in order to
		more efficiently update parameters to vertex & fragment programs, since inverting Quaterion
		and Vector in order to derive object-space positions / directions for cameras and
		lights is much more efficient than inverting a complete 4x4 matrix, and also 
		eliminates problems introduced by scaling. */
		const CGUIVector3& getWorldPosition(void);



	protected:

		/// Only available internally - notification of parent.
		virtual void SetParent(CGUINode* parent);

		/** Triggers the node to update it's combined transforms.
		@par This method is called internally by Ogre to ask the node
		to update it's complete transformation based on it's parents
		derived transform.
		*/
		virtual void updateFromParent(void);

		//!< called when updated from parent
		virtual void OnUpdatedFromParent();

		/** Internal method for building a CGUIMatrix4 from orientation / scale / position. 
		@remarks Transform is performed in the order rotate, scale, translation, i.e. translation is independent
		of orientation axes, scale does not affect size of translation, rotation and scaling are always
		centered on the origin.
		*/
		void makeTransform(
			const CGUIVector3& position, 
			const CGUIVector3& scale, 
			const CGUIQuaternion& orientation, 
			CGUIMatrix4& destMatrix ) const;

		/** Internal method for building an inverse CGUIMatrix4 from orientation / scale / position. 
		@remarks As makeTransform except it build the inverse given the same data as makeTransform, so
		performing -translation, 1/scale, -rotate in that order.
		*/
		void makeInverseTransform(
			const CGUIVector3& position, 
			const CGUIVector3& scale, 
			const CGUIQuaternion& orientation, 
			CGUIMatrix4& destMatrix );



		//!< set dirty flag
		void	SetDirtyFlag();

		//!< update if the node is dirty
		void	UpdateDirtyNode();

		/// set child
		void	SetChild( const CGUINode* pNode);

		/// set next sibling
		void	SetNextSibling( const CGUINode* pNode );

	protected:
		CGUIQuaternion	m_aOrientation;		//!< Stores the orientation of the node relative to it's parent.
		CGUIVector3		m_aPosition;		//!< Stores the position/translation of the node relative to its parent.
		CGUIVector3		m_aScale;			//!< Stores the scaling factor applied to this node
		bool			m_bInheritScale;	//!< Stores whether this node inherits scale from it's parent


		bool			m_bDirtyFlag;		//!< dirty rect
		CGUINode*		m_pParent;			//!< parent
		CGUINode*		m_pChild;			//!< child
		CGUINode*		m_pNextSibling;		//!< sibling


		/** Cached combined orientation.
		@par
		This member is the orientation derived by combining the
		local transformations and those of it's parents.
		This is updated when updateFromParent is called by the
		SceneManager or the nodes parent.
		*/
		CGUIQuaternion m_aDerivedOrientation;

		/** Cached combined position.
		@par
		This member is the position derived by combining the
		local transformations and those of it's parents.
		This is updated when updateFromParent is called by the
		SceneManager or the nodes parent.
		*/
		CGUIVector3 m_aDerivedPosition;

		/** Cached combined scale.
		@par
		This member is the scale derived by combining the
		local transformations and those of it's parents.
		This is updated when updateFromParent is called by the
		SceneManager or the nodes parent.
		*/
		CGUIVector3 m_aDerivedScale;	
		
		
		bool mCachedTransformOutOfDate;		//!< whether transform is out of data
		bool mCachedInverseTransformOutOfDate;		//!< whether transform is out of data

		CGUIMatrix4 mCachedTransform;		//!< Cached derived transform as a 4x4 matrix
		CGUIMatrix4 mCachedInverseTransform;		//!< Cached derived transform as a 4x4 matrix
	};

} //namespace

#endif
