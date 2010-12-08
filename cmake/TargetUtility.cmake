macro( macro_set_target_output_dir targetname )
	set_target_properties( 
		${targetname}  
		PROPERTIES 	DEBUG_POSTFIX "_debug"
		)
	set_target_properties( 
		${targetname}  
		PROPERTIES 	RELEASE_POSTFIX "_release"
		)		
	set_target_properties( 
		${targetname}  
		PROPERTIES 	MINSIZEREL_POSTFIX "_minsizerel"
		)
	set_target_properties( 
		${targetname}  
		PROPERTIES 	RELWITHDEBINFO_POSTFIX "_relwithdebinfo"
		)	
endmacro()
