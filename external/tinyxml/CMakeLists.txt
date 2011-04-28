set( target_name "tinyxml" )
message( STATUS "generate library: " ${target_name} )

set( src_list 
    ../tinystr.cpp
    ../tinyxml.cpp
    ../tinyxmlerror.cpp
    ../tinyxmlparser.cpp
    )

set( inc_list
    ../tinystr.h
    ../tinyxml.h
    )
			
	


#foreach( F ${src_list} ${inc_list} )
#    message( ${F} )
#endforeach( F )
add_library( ${target_name} STATIC ${src_list} ${inc_list}  )
set_target_properties(${target_name} PROPERTIES FOLDER external)

