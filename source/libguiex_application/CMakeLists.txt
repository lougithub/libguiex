set( target_name "libguiex_application" )
message( STATUS "generate library: " ${target_name} )


set( ${target_name}_glut_SRCS 
	app_glut/app_glut.cpp
	)
source_group( "glut" FILES ${${target_name}_glut_SRCS} )

set( ${target_name}_ios_SRCS 
	app_ios/app_ios.h
	app_ios/app_ios.mm
	app_ios/app_ios_engine.cpp
	app_ios/app_ios_engine.h
	)
source_group( "ios" FILES ${${target_name}_ios_SRCS} )

if( BUILD_PLATFORM_WIN32 )
	set( ${target_name}_SRCS ${${target_name}_glut_SRCS} )
elseif( BUILD_PLATFORM_IOS )
	set( ${target_name}_SRCS ${${target_name}_ios_SRCS} )
elseif( BUILD_PLATFORM_MACOS )
	set( ${target_name}_SRCS ${${target_name}_ios_SRCS} )
else( )
	message( FATAL_ERROR "unsupport platform" )
endif()

include_directories( "${PROJECT_SOURCE_DIR}/external/glut" )

add_library( ${target_name} STATIC ${${target_name}_SRCS} )
set_target_properties(${target_name} PROPERTIES FOLDER library)

