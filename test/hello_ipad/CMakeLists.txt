if( APPLE )

    set( execute_name "hello_ipad" )
    message( STATUS "generate execute: " ${execute_name} )

    # all source
    set( ${execute_name}_SRCS 
        ApplicationEngine.h
        ApplicationEngine.mm
        hello_ipad.h
        hello_ipad.mm
        )

    #set resource data
    #SET_SOURCE_FILES_PROPERTIES(
    #    apple1_aux1
    #    apple1_aux2
    #    PROPERTIES
    #    MACOSX_PACKAGE_LOCATION MacOSX
    #    )
    #SET_SOURCE_FILES_PROPERTIES(
    #    apple1_res1
    #    apple1_res2
    #    PROPERTIES
    #    MACOSX_PACKAGE_LOCATION Resources
    #    )


    # Set the OS X Bundle specific CMake variables which will be used to populate the plist for
    # the application bundle
    set(MACOSX_BUNDLE_INFO_STRING "${execute_name}")
    set(MACOSX_BUNDLE_BUNDLE_NAME "${execute_name}")
    #MACOSX_BUNDLE_ICON_FILE
    #set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.rogue-research.SimpleCocoaVTK")
    #set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${VTK_VERSION}")
    #set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION})
    #set(MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION})
    #set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2010. All Rights Reserved.")

    add_executable( ${execute_name} MACOSX_BUNDLE ${${execute_name}_SRCS} )
    add_dependencies( ${execute_name} libguiex_module libguiex_widget )
    target_link_libraries( ${execute_name} "-framework Cocoa -framework OpenGLES -framework IOKit")
    # Make sure the 'Resources' Directory is correctly created before we build
    #add_custom_command (TARGET SimpleCocoaVTK PRE_BUILD COMMAND mkdir -p ${EXECUTABLE_OUTPUT_PATH}/\${CONFIGURATION}/SimpleCocoaVTK.app/Contents/Resources)
endif()
