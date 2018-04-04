
# The path to this file.
set(KIWI_SOURCES_PATH ${CMAKE_CURRENT_LIST_DIR})
# The output path for the externals.
set(KIWI_OUPUT_PATH)
# The macro defines the output path of the externals.
macro(set_kiwi_external_path EXTERNAL_PATH)
	set(KIWI_OUPUT_PATH ${EXTERNAL_PATH})
endmacro(set_kiwi_external_path)

# The function adds an external to the project.
# PROJECT_NAME is the name of your project (for example: freeverb_project)
# EXTERNAL_NAME is the name of your external (for example: freeverb~)
# EXTERNAL_SOURCES are the source files (for example: freeverb~.c)
# The function should be call:
# add_external(freeverb_project freeverb~ "userpath/freeverb~.c userpath/otherfile.c")
# later see how to manage relative and absolute path
function(add_kiwi_external PROJECT_NAME EXTERNAL_NAME EXTERNAL_SOURCES)
	source_group(src FILES ${EXTERNAL_SOURCES})
	add_library(${PROJECT_NAME} SHARED ${EXTERNAL_SOURCES})

	# Includes the path to Pure Data sources.
	target_include_directories(${PROJECT_NAME} PRIVATE ${KIWI_SOURCES_PATH})

	# Defines plateform specifix suffix and the linking necessities.
	set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
	set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".kiwix")

	# Defines the name of the external.
	# On XCode with CMake < 3.4 if the name of an external ends with tilde but doesn't have a dot, the name must be 'name~'.
	# CMake 3.4 is not sure, but it should be between 3.3.2 and 3.6.2
	string(FIND ${EXTERNAL_NAME} "." NAME_HAS_DOT)
	string(FIND ${EXTERNAL_NAME} "~" NAME_HAS_TILDE)
	if((${CMAKE_VERSION} VERSION_LESS 3.4) AND (CMAKE_GENERATOR STREQUAL Xcode) AND (NAME_HAS_DOT EQUAL -1) AND (NAME_HAS_TILDE GREATER -1))
			set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME '${EXTERNAL_NAME}')
	else()
		set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${EXTERNAL_NAME})
	endif()

	# Defines the output path of the external.
  set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${KIWI_OUPUT_PATH})
	set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${KIWI_OUPUT_PATH})
	set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${KIWI_OUPUT_PATH})
	foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
		    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
			  set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${KIWI_OUPUT_PATH})
				set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${KIWI_OUPUT_PATH})
				set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${KIWI_OUPUT_PATH})
	endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

endfunction(add_kiwi_external)
