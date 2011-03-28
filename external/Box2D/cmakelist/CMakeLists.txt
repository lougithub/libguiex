set( target_name "box2d" )
message( STATUS "generate library: " ${target_name} )

set(BOX2D_Collision_SRCS
	../Box2D/Collision/b2BroadPhase.cpp
	../Box2D/Collision/b2CollideCircle.cpp
	../Box2D/Collision/b2CollidePolygon.cpp
	../Box2D/Collision/b2Collision.cpp
	../Box2D/Collision/b2Distance.cpp
	../Box2D/Collision/b2DynamicTree.cpp
	../Box2D/Collision/b2TimeOfImpact.cpp
)
set(BOX2D_Collision_HDRS
	../Box2D/Collision/b2BroadPhase.h
	../Box2D/Collision/b2Collision.h
	../Box2D/Collision/b2Distance.h
	../Box2D/Collision/b2DynamicTree.h
	../Box2D/Collision/b2TimeOfImpact.h
)
set(BOX2D_Shapes_SRCS
	../Box2D/Collision/Shapes/b2CircleShape.cpp
	../Box2D/Collision/Shapes/b2PolygonShape.cpp
)
set(BOX2D_Shapes_HDRS
	../Box2D/Collision/Shapes/b2CircleShape.h
	../Box2D/Collision/Shapes/b2PolygonShape.h
	../Box2D/Collision/Shapes/b2Shape.h
)
set(BOX2D_Common_SRCS
	../Box2D/Common/b2BlockAllocator.cpp
	../Box2D/Common/b2Math.cpp
	../Box2D/Common/b2Settings.cpp
	../Box2D/Common/b2StackAllocator.cpp
)
set(BOX2D_Common_HDRS
	../Box2D/Common/b2BlockAllocator.h
	../Box2D/Common/b2Math.h
	../Box2D/Common/b2Settings.h
	../Box2D/Common/b2StackAllocator.h
)
set(BOX2D_Dynamics_SRCS
	../Box2D/Dynamics/b2Body.cpp
	../Box2D/Dynamics/b2ContactManager.cpp
	../Box2D/Dynamics/b2Fixture.cpp
	../Box2D/Dynamics/b2Island.cpp
	../Box2D/Dynamics/b2World.cpp
	../Box2D/Dynamics/b2WorldCallbacks.cpp
)
set(BOX2D_Dynamics_HDRS
	../Box2D/Dynamics/b2Body.h
	../Box2D/Dynamics/b2ContactManager.h
	../Box2D/Dynamics/b2Fixture.h
	../Box2D/Dynamics/b2Island.h
	../Box2D/Dynamics/b2TimeStep.h
	../Box2D/Dynamics/b2World.h
	../Box2D/Dynamics/b2WorldCallbacks.h
)
set(BOX2D_Contacts_SRCS
	../Box2D/Dynamics/Contacts/b2CircleContact.cpp
	../Box2D/Dynamics/Contacts/b2Contact.cpp
	../Box2D/Dynamics/Contacts/b2ContactSolver.cpp
	../Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
	../Box2D/Dynamics/Contacts/b2PolygonContact.cpp
	../Box2D/Dynamics/Contacts/b2TOISolver.cpp
)
set(BOX2D_Contacts_HDRS
	../Box2D/Dynamics/Contacts/b2CircleContact.h
	../Box2D/Dynamics/Contacts/b2Contact.h
	../Box2D/Dynamics/Contacts/b2ContactSolver.h
	../Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h
	../Box2D/Dynamics/Contacts/b2PolygonContact.h
	../Box2D/Dynamics/Contacts/b2TOISolver.h
)
set(BOX2D_Joints_SRCS
	../Box2D/Dynamics/Joints/b2DistanceJoint.cpp
	../Box2D/Dynamics/Joints/b2FrictionJoint.cpp
	../Box2D/Dynamics/Joints/b2GearJoint.cpp
	../Box2D/Dynamics/Joints/b2Joint.cpp
	../Box2D/Dynamics/Joints/b2LineJoint.cpp
	../Box2D/Dynamics/Joints/b2MouseJoint.cpp
	../Box2D/Dynamics/Joints/b2PrismaticJoint.cpp
	../Box2D/Dynamics/Joints/b2PulleyJoint.cpp
	../Box2D/Dynamics/Joints/b2RevoluteJoint.cpp
	../Box2D/Dynamics/Joints/b2WeldJoint.cpp
)
set(BOX2D_Joints_HDRS
	../Box2D/Dynamics/Joints/b2DistanceJoint.h
	../Box2D/Dynamics/Joints/b2FrictionJoint.h
	../Box2D/Dynamics/Joints/b2GearJoint.h
	../Box2D/Dynamics/Joints/b2Joint.h
	../Box2D/Dynamics/Joints/b2LineJoint.h
	../Box2D/Dynamics/Joints/b2MouseJoint.h
	../Box2D/Dynamics/Joints/b2PrismaticJoint.h
	../Box2D/Dynamics/Joints/b2PulleyJoint.h
	../Box2D/Dynamics/Joints/b2RevoluteJoint.h
	../Box2D/Dynamics/Joints/b2WeldJoint.h
)
set(BOX2D_General_HDRS
	../Box2D/Box2D.h
)
include_directories( ../ )


# These are used to create visual studio folders.
source_group(Collision FILES ${BOX2D_Collision_SRCS} ${BOX2D_Collision_HDRS})
source_group(Collision\\Shapes FILES ${BOX2D_Shapes_SRCS} ${BOX2D_Shapes_HDRS})
source_group(Common FILES ${BOX2D_Common_SRCS} ${BOX2D_Common_HDRS})
source_group(Dynamics FILES ${BOX2D_Dynamics_SRCS} ${BOX2D_Dynamics_HDRS})
source_group(Dynamics\\Contacts FILES ${BOX2D_Contacts_SRCS} ${BOX2D_Contacts_HDRS})
source_group(Dynamics\\Joints FILES ${BOX2D_Joints_SRCS} ${BOX2D_Joints_HDRS})
source_group(Include FILES ${BOX2D_General_HDRS})

add_library( ${target_name} STATIC
    ${BOX2D_General_HDRS}
    ${BOX2D_Joints_SRCS}
    ${BOX2D_Joints_HDRS}
    ${BOX2D_Contacts_SRCS}
    ${BOX2D_Contacts_HDRS}
    ${BOX2D_Dynamics_SRCS}
    ${BOX2D_Dynamics_HDRS}
    ${BOX2D_Common_SRCS}
    ${BOX2D_Common_HDRS}
    ${BOX2D_Shapes_SRCS}
    ${BOX2D_Shapes_HDRS}
    ${BOX2D_Collision_SRCS}
    ${BOX2D_Collision_HDRS}
    )

set_target_properties(${target_name} PROPERTIES FOLDER external)

