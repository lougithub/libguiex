set( target_name "ogg" )
message( STATUS "generate library: " ${target_name} )

set( src_list 
    ../src/bitwise.c
    ../src/framing.c
	)

set( inc_list
    ../include/ogg/ogg.h
    ../include/ogg/os_types.h
	)
	
include_directories( ../include )
add_library( ${target_name} STATIC ${src_list} ${inc_list}  )

set_target_properties(${target_name} PROPERTIES FOLDER external)

