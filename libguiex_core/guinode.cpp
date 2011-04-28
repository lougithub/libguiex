/** 
* @file guinode.cpp
* @brief A node contains information about the 
* transformation which will apply to it and all of it's children
* @author ken
* @date 2007-11-23
*/


//============================================================================//
// include
//============================================================================// 
#include "guinode.h"
#include "guimath.h"
#include "guiexception.h"
#include "guistring.h"


namespace guiex 
{
	//-----------------------------------------------------------------------
	CGUINode::CGUINode( )
	{
		m_aOrientation = m_aDerivedOrientation = CGUIQuaternion::IDENTITY;
		m_aPosition = m_aDerivedPosition = CGUIVector3::ZERO;
		m_aScale = m_aDerivedScale = CGUIVector3::UNIT_SCALE;
		m_bInheritScale = true;
	}

	//-----------------------------------------------------------------------
	CGUINode::~CGUINode()
	{
	}    
	//-----------------------------------------------------------------------
	const CGUIMatrix4& CGUINode::NodeGetTransform(void)
	{
		return mCachedTransform;
	}
	//-----------------------------------------------------------------------
	/** Gets the full transformation matrix for this node.
	@remarks This method returns the full transformation matrix
	for this node, including the effect of any parent node
	transformations
	*/
	const CGUIMatrix4& CGUINode::NodeGetFullTransform(void)
	{
		return mCachedFullTransform;
	}
	//-----------------------------------------------------------------------
	//!< get the full inverse transform matrix of this node
	const CGUIMatrix4& CGUINode::NodeGetFullInverseTransform(void)
	{
		return mCachedFullInverseTransform;
	}
	//-----------------------------------------------------------------------
	/** Triggers the node to update it's combined transforms.
	@par This method is called internally by Ogre to ask the node
	to update it's complete transformation based on it's parents
	derived transform.
	*/
	void CGUINode::NodeUpdateFromParent(void)
	{
		if ( NodeGetParent() )
		{
			// Combine orientation with that of parent
			CGUIQuaternion parentOrientation = NodeGetParent()->NodeGetDerivedOrientation();
			m_aDerivedOrientation = parentOrientation * m_aOrientation;

			// Update scale
			CGUIVector3 parentScale = NodeGetParent()->NodeGetDerivedScale();
			if (m_bInheritScale)
			{
				// Scale own position by parent scale
				m_aDerivedScale = m_aScale * parentScale;
			}
			else
			{
				// No inheritence
				m_aDerivedScale = m_aScale;
			}

			// Change position vector based on parent's orientation & scale
			m_aDerivedPosition = parentOrientation * (m_aPosition * parentScale);

			// Add altered position vector to parents
			m_aDerivedPosition += NodeGetParent()->NodeGetDerivedPosition();
		}
		else
		{
			// Root node, no parent
			m_aDerivedOrientation = m_aOrientation;
			m_aDerivedPosition = m_aPosition;
			m_aDerivedScale = m_aScale;
		}

		NodeMakeTransform( NodeGetPosition(), NodeGetScale(), NodeGetOrientation(), mCachedTransform);
		NodeMakeTransform( NodeGetDerivedPosition(), NodeGetDerivedScale(), NodeGetDerivedOrientation(), mCachedFullTransform);
		mCachedFullInverseTransform = mCachedFullTransform.inverse();
	}
	//-----------------------------------------------------------------------
	//!< Returns a quaternion representing the nodes orientation.
	const CGUIQuaternion& CGUINode::NodeGetOrientation() const
	{
		return m_aOrientation;
	}
	//-----------------------------------------------------------------------
	//!< Sets the orientation of this node via a quaternion.
	void CGUINode::NodeSetOrientation( const CGUIQuaternion & q )
	{
		m_aOrientation = q;
	}
	//-----------------------------------------------------------------------
	//!< Sets the orientation of this node via quaternion parameters.
	void CGUINode::NodeSetOrientation( real w, real x, real y, real z)
	{
		m_aOrientation.w = w;
		m_aOrientation.x = x;
		m_aOrientation.y = y;
		m_aOrientation.z = z;
	}
	//-----------------------------------------------------------------------
	//!< Resets the nodes orientation (local axes as world axes, no rotation).
	void CGUINode::NodeResetOrientation(void)
	{
		m_aOrientation = CGUIQuaternion::IDENTITY;
	}

	//-----------------------------------------------------------------------
	//!< Sets the position of the node relative to it's parent.
	void CGUINode::NodeSetPosition(const CGUIVector3& pos)
	{
		m_aPosition = pos;
	}
	//-----------------------------------------------------------------------
	//!< Sets the position of the node relative to it's parent.
	void CGUINode::NodeSetPosition(real x, real y, real z)
	{
		NodeSetPosition(CGUIVector3(x,y,z));
	}

	//-----------------------------------------------------------------------
	//!< Gets the position of the node relative to it's parent.
	const CGUIVector3 & CGUINode::NodeGetPosition(void) const
	{
		return m_aPosition;
	}
	//-----------------------------------------------------------------------
	/** Gets a matrix whose columns are the local axes based on
	the nodes orientation relative to it's parent. */
	CGUIMatrix3 CGUINode::NodeGetLocalAxes(void) const
	{
		CGUIVector3 axisX = CGUIVector3::UNIT_X;
		CGUIVector3 axisY = CGUIVector3::UNIT_Y;
		CGUIVector3 axisZ = CGUIVector3::UNIT_Z;

		axisX = m_aOrientation * axisX;
		axisY = m_aOrientation * axisY;
		axisZ = m_aOrientation * axisZ;

		return CGUIMatrix3(axisX.x, axisY.x, axisZ.x,
			axisX.y, axisY.y, axisZ.y,
			axisX.z, axisY.z, axisZ.z);
	}

	//-----------------------------------------------------------------------
	/** Moves the node along the cartesian axes.
	@par This method moves the node by the supplied vector along the
	world cartesian axes, i.e. along world x,y,z
	@param d Vector with x,y,z values representing the translation.
	@param relativeTo The space which this transform is relative to.
	*/
	void CGUINode::NodeTranslate(const CGUIVector3& d, ETransformSpace relativeTo)
	{
		CGUIVector3 adjusted;
		switch(relativeTo) 
		{
		case eTS_LOCAL:
			// position is relative to local so transform downwards
			m_aPosition += m_aOrientation * d;
			break;
		case eTS_WORLD:
			// position is relative to parent so transform upwards
			if (NodeGetParent())
			{
				m_aPosition += NodeGetParent()->NodeGetDerivedOrientation().Inverse() * d; 
			}
			else
			{
				m_aPosition += d;
			}
			break;
		case eTS_PARENT:
			m_aPosition += d;
			break;
		}
	}
	//-----------------------------------------------------------------------
	/** Moves the node along the cartesian axes.
	@par This method moves the node by the supplied vector along the
	world cartesian axes, i.e. along world x,y,z
	@param x
	@param y
	@param z real x, y and z values representing the translation.
	@param relativeTo The space which this transform is relative to.
	*/
	void CGUINode::NodeTranslate(real x, real y, real z, ETransformSpace relativeTo)
	{
		CGUIVector3 v(x,y,z);
		NodeTranslate(v, relativeTo);
	}
	//-----------------------------------------------------------------------
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
	void CGUINode::NodeTranslate(const CGUIMatrix3& axes, const CGUIVector3& move, ETransformSpace relativeTo)
	{
		CGUIVector3 derived = axes * move;
		NodeTranslate(derived, relativeTo);
	}
	//-----------------------------------------------------------------------
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
	void CGUINode::NodeTranslate(const CGUIMatrix3& axes, real x, real y, real z, ETransformSpace relativeTo)
	{
		CGUIVector3 d(x,y,z);
		NodeTranslate(axes,d,relativeTo);
	}
	//-----------------------------------------------------------------------
	//!< Rotate the node around the Z-axis.
	void CGUINode::NodeRoll(real angle, ETransformSpace relativeTo)
	{
		NodeRotate(CGUIVector3::UNIT_Z, angle, relativeTo);
	}
	//-----------------------------------------------------------------------
	//!< Rotate the node around the X-axis.
	void CGUINode::NodePitch(real angle, ETransformSpace relativeTo)
	{
		NodeRotate(CGUIVector3::UNIT_X, angle, relativeTo);
	}
	//-----------------------------------------------------------------------
	//!< Rotate the node around the Y-axis.
	void CGUINode::NodeYaw(real angle, ETransformSpace relativeTo)
	{
		NodeRotate(CGUIVector3::UNIT_Y, angle, relativeTo);
	}
	//-----------------------------------------------------------------------
	//!< Rotate the node around an arbitrary axis.
	void CGUINode::NodeRotate(const CGUIVector3& axis, real angle, ETransformSpace relativeTo)
	{
		CGUIQuaternion q;
		q.FromAngleAxis(angle,axis);
		NodeRotate(q, relativeTo);
	}
	//-----------------------------------------------------------------------
	void CGUINode::NodeRotate(const CGUIQuaternion& q, ETransformSpace relativeTo)
	{
		switch(relativeTo) 
		{
		case eTS_PARENT:
			// Rotations are normally relative to local axes, transform up
			m_aOrientation = q * m_aOrientation;
			break;
		case eTS_WORLD:
			// Rotations are normally relative to world axes, transform up
			m_aOrientation = m_aOrientation * NodeGetDerivedOrientation().Inverse() * q * NodeGetDerivedOrientation();
			break;
		case eTS_LOCAL:
			// Note the order of the mult, i.e. q comes after
			m_aOrientation = m_aOrientation * q;
			break;
		}
	}
	//-----------------------------------------------------------------------
	/** Gets the orientation of the node as derived from all parents.
	*/
	const CGUIQuaternion & CGUINode::NodeGetDerivedOrientation(void)
	{
		return m_aDerivedOrientation;
	}
	//-----------------------------------------------------------------------
	/** Gets the position of the node as derived from all parents.
	*/
	const CGUIVector3 & CGUINode::NodeGetDerivedPosition(void)
	{
		return m_aDerivedPosition;
	}
	//-----------------------------------------------------------------------
	/** Gets the scaling factor of the node as derived from all parents.
	*/
	const CGUIVector3 & CGUINode::NodeGetDerivedScale(void)
	{
		return m_aDerivedScale;
	}
	//-----------------------------------------------------------------------
	/** Sets the scaling factor applied to this node.
	@remarks Scaling factors, unlike other transforms, are not always inherited by child nodes. 
	Whether or not scalings affect both the size and position of the child nodes depends on
	the NodeSetInheritScale option of the child. In some cases you want a scaling factor of a parent node
	to apply to a child node (e.g. where the child node is a part of the same object, so you
	want it to be the same relative size and position based on the parent's size), but
	not in other cases (e.g. where the child node is just for positioning another object,
	you want it to maintain it's own size and relative position). The default is to inherit
	as with other transforms.
	@par Note that like rotations, scalings are oriented around the node's origin.
	*/
	void CGUINode::NodeSetScale(const CGUIVector3& scale)
	{
		m_aScale = scale;
	}
	//-----------------------------------------------------------------------
	/** Sets the scaling factor applied to this node.
	@remarks Scaling factors, unlike other transforms, are not always inherited by child nodes. 
	Whether or not scalings affect both the size and position of the child nodes depends on
	the NodeSetInheritScale option of the child. In some cases you want a scaling factor of a parent node
	to apply to a child node (e.g. where the child node is a part of the same object, so you
	want it to be the same relative size and position based on the parent's size), but
	not in other cases (e.g. where the child node is just for positioning another object,
	you want it to maintain it's own size and relative position). The default is to inherit
	as with other transforms.
	@par Note that like rotations, scalings are oriented around the node's origin.
	*/
	void CGUINode::NodeSetScale(real x, real y, real z)
	{
		m_aScale.x = x;
		m_aScale.y = y;
		m_aScale.z = z;
	}
	//-----------------------------------------------------------------------
	/** Gets the scaling factor of this node.
	*/
	const CGUIVector3 & CGUINode::NodeGetScale(void) const
	{
		return m_aScale;
	}
	//-----------------------------------------------------------------------
	/** Tells the node whether it should inherit scaling factors from it's parent node.
	@remarks
	Scaling factors, unlike other transforms, are not always inherited by child nodes. 
	Whether or not scalings affect both the size and position of the child nodes depends on
	the NodeSetInheritScale option of the child. In some cases you want a scaling factor of a parent node
	to apply to a child node (e.g. where the child node is a part of the same object, so you
	want it to be the same relative size and position based on the parent's size), but
	not in other cases (e.g. where the child node is just for positioning another object,
	you want it to maintain it's own size and relative position). The default is to inherit
	as with other transforms.
	@param inherit If true, this node's scale and position will be affected by its parent's scale. If false,
	it will not be affected.
	*/
	void CGUINode::NodeSetInheritScale(bool inherit)
	{
		m_bInheritScale = inherit;
	}
	//-----------------------------------------------------------------------
	/** Returns true if this node is affected by scaling factors applied to the parent node. 
	@remarks See NodeSetInheritScale for more info.
	*/
	bool CGUINode::NodeGetInheritScale(void) const
	{
		return m_bInheritScale;
	}
	//-----------------------------------------------------------------------
	/** Scales the node, combining it's current scale with the passed in scaling factor. 
	@remarks This method applies an extra scaling factor to the node's existing scale, (unlike NodeSetScale
	which overwrites it) combining it's current scale with the new one. E.g. calling this 
	method twice with CGUIVector3(2,2,2) would have the same effect as NodeSetScale(CGUIVector3(4,4,4)) if
	the existing scale was 1.
	@par Note that like rotations, scalings are oriented around the node's origin.
	*/
	void CGUINode::NodeScale(const CGUIVector3& scale)
	{
		m_aScale = m_aScale * scale;
	}
	//-----------------------------------------------------------------------
	/** Scales the node, combining it's current scale with the passed in scaling factor. 
	@remarks This method applies an extra scaling factor to the node's existing scale, (unlike NodeSetScale
	which overwrites it) combining it's current scale with the new one. E.g. calling this 
	method twice with CGUIVector3(2,2,2) would have the same effect as NodeSetScale(CGUIVector3(4,4,4)) if
	the existing scale was 1.
	@par Note that like rotations, scalings are oriented around the node's origin.
	*/
	void CGUINode::NodeScale(real x, real y, real z)
	{
		m_aScale.x *= x;
		m_aScale.y *= y;
		m_aScale.z *= z;
	}
	//-----------------------------------------------------------------------
	/** Internal method for building a CGUIMatrix4 from orientation / scale / position. 
	@remarks Transform is performed in the order rotate, scale, translation, i.e. translation is independent
	of orientation axes, scale does not affect size of translation, rotation and scaling are always
	centered on the origin.
	*/
	void CGUINode::NodeMakeTransform(const CGUIVector3& position, const CGUIVector3& scale, 
		const CGUIQuaternion& orientation, CGUIMatrix4& destMatrix) const
	{
		destMatrix = CGUIMatrix4::IDENTITY;
		// Ordering:
		//    1. Scale
		//    2. Rotate
		//    3. Translate

		// Parent scaling is already applied to derived position
		// Own scale is applied before rotation
		CGUIMatrix3 rot3x3, scale3x3;
		orientation.ToRotationMatrix(rot3x3);
		scale3x3 = CGUIMatrix3::ZERO;
		scale3x3[0][0] = scale.x;
		scale3x3[1][1] = scale.y;
		scale3x3[2][2] = scale.z;

		destMatrix = rot3x3 * scale3x3;
		destMatrix.setTrans(position);
	}
	//-----------------------------------------------------------------------
	/** 
	@remarks This is only used if the SceneManager chooses to render the node.
	*/
	void CGUINode::NodeGetWorldTransforms(CGUIMatrix4* xform)
	{
		// Assumes up to date
		*xform = NodeGetFullTransform();
	}
	//-----------------------------------------------------------------------
	/** Gets the world orientation of this node; this is used in order to
	more efficiently update parameters to vertex & fragment programs, since inverting Quaterion
	and Vector in order to derive object-space positions / directions for cameras and
	lights is much more efficient than inverting a complete 4x4 matrix, and also 
	eliminates problems introduced by scaling. */
	const CGUIQuaternion& CGUINode::NodeGetWorldOrientation(void)
	{
		return NodeGetDerivedOrientation();
	}
	//-----------------------------------------------------------------------
	/** Gets the world orientation of this node; this is used in order to
	more efficiently update parameters to vertex & fragment programs, since inverting Quaterion
	and Vector in order to derive object-space positions / directions for cameras and
	lights is much more efficient than inverting a complete 4x4 matrix, and also 
	eliminates problems introduced by scaling. */
	const CGUIVector3& CGUINode::NodeGetWorldPosition(void)
	{
		return NodeGetDerivedPosition();
	}
	//------------------------------------------------------------------------------
}

