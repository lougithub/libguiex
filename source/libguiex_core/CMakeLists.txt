set( library_name "libguiex_core" )
message( STATUS "generate library: " ${library_name} )

aux_source_directory(. src_list )
aux_source_directory(../../include/${library_name}/ inc_list )


#foreach( F ${src_list} ${inc_list} )
#    message( ${F} )
#endforeach( F )
add_library( ${library_name} STATIC ${src_list} ${inc_list}  )
add_dependencies( ${library_name} libguiex_core )

