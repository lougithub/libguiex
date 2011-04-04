set( target_name "libguiex_framework" )
message( STATUS "generate library: " ${target_name} )


set( ${target_name}_framework_SRCS 
	guiframework.h
	guiframeworkbase.cpp
	guiframeworkbase.h
	)
source_group( "base" FILES ${${target_name}_framework_SRCS} )

set( ${target_name}_windows_SRCS 
	guiframework_windows.cpp
	guiframework_windows.h
	)
source_group( "windows" FILES ${${target_name}_windows_SRCS} )

set( ${target_name}_mac_SRCS 
	guiframework_mac.cpp
	guiframework_mac.h
	)
source_group( mac FILES ${${target_name}_mac_SRCS} )

if( BUILD_PLATFORM_WIN32 )

	set( ${target_name}_SRCS
		${${target_name}_framework_SRCS}
		${${target_name}_windows_SRCS}
		)

	include_directories( "${PROJECT_SOURCE_DIR}/external/openal/include" )

elseif( BUILD_PLATFORM_IPHONE OR BUILD_PLATFORM_MACOS )

	set( ${target_name}_SRCS
		${${target_name}_framework_SRCS}
		${${target_name}_mac_SRCS}
		)

else( )

	message( FATAL_ERROR "unsupport platform" )

endif()

include_directories( "${PROJECT_SOURCE_DIR}/external/tinyxml" )
include_directories( "${PROJECT_SOURCE_DIR}/external/lua/src" )
include_directories( "${PROJECT_SOURCE_DIR}/external/freetype/include" )
include_directories( "${PROJECT_SOURCE_DIR}/external/Box2D" )
    
add_library( ${target_name} STATIC ${${target_name}_SRCS} )
set_target_properties(${target_name} PROPERTIES FOLDER library)
