#Transformations example

file(
 GLOB_RECURSE FINALPROJECT_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE FINALPROJECT_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this assignment7_lighting's asset folder to bin when it is built
add_custom_target(copyAssetsFP ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${FINALPROJECT_INC} DESTINATION include/finalProject_Fire)
add_executable(finalProject_Fire ${FINALPROJECT_SRC} ${FINALPROJECT_INC} ${FINALPROJECT_ASSETS})
target_link_libraries(finalProject_Fire PUBLIC core IMGUI)
target_include_directories(finalProject_Fire PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when assignment7_lighting is built
add_dependencies(finalProject_Fire copyAssetsFP)