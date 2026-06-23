cmake_minimum_required(VERSION 3.10)

set(PROJ_NAME "HMASM"           )   # this is implementation of assembler.
set(PROJ_LANG LANGUAGES C       )   # to be written in pure `C` no external libs.
set(PROJ_VERS VERSION  0.00.3   )   # need to make sure to change this version.
set(PROJ_DESC DESCRIPTION "implementation of assembler"         )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(${PROJ_NAME} ${PROJ_DESC} ${PROJ_LANG} ${PROJ_VERS})
add_custom_target(
    copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
)

# For LAYER-1 (contains lexer-parser and IR generation logic) as executable **
# ** -> executable has to be statically liked with one format and one isa
set(PROJ_SRC_DIR "${CMAKE_SOURCE_DIR}/src"                  )
set(PROJ_INC_DIR "${CMAKE_SOURCE_DIR}/include"              )

set(PROJ_CORE_SRC_DIR    "${PROJ_SRC_DIR}/core"             )
set(PROJ_CORE_INC_DIR    "${PROJ_INC_DIR}/core"             )
set(PROJ_SUPPORT_SRC_DIR "${PROJ_SRC_DIR}/support"          )
set(PROJ_SUPPORT_INC_DIR "${PROJ_INC_DIR}/support"          )

set(PROJ_FMT_SRC_DIR "${PROJ_SRC_DIR}/fmt"                  )
set(PROJ_ISA_SRC_DIR "${PROJ_SRC_DIR}/isa"                  )

file(GLOB PROJ_SRC_FILES "${PROJ_CORE_SRC_DIR}/*.c" "${PROJ_SUPPORT_SRC_DIR}/*.c")
file(GLOB PROJ_INC_FILES "${PROJ_CORE_INC_DIR}/*.h" "${PROJ_SUPPORT_INC_DIR}/*.h")
file(GLOB PROJ_ISA_FILES "${PROJ_ISA_SRC_DIR}/*.c")
file(GLOB PROJ_FMT_FILES "${PROJ_FMT_SRC_DIR}/*.c")

if(DEFINED TARGET_ISA)
    set(ISA_TO_BUILD ${TARGET_ISA}  )
else()
    set(ISA_TO_BUILD ${DEFINE_ISA}  )
endif()

if(DEFINED TARGET_FMT)
    set(FMT_TO_BUILD ${TARGET_FMT}  )
else()
    set(FMT_TO_BUILD ${DEFINE_FMT}  )
endif()

if (DEFINED INSTALL_IN_SYSTEM)
    message(STATUS "System install enabled (Value: ${INSTALL_IN_SYSTEM})")
    set(TARGET_INSTALL_PATH bin)
else()
    set(TARGET_INSTALL_PATH "${CMAKE_SOURCE_DIR}/bin")
endif()

set(TARGET_NAME "hmasm")

add_subdirectory(${CMAKE_SOURCE_DIR}/src/keystone EXCLUDE_FROM_ALL)

add_executable(${TARGET_NAME} )
target_sources(${TARGET_NAME} PRIVATE ${PROJ_SRC_FILES} )
target_sources(${TARGET_NAME} PRIVATE ${PROJ_INC_FILES} )
target_sources(${TARGET_NAME} PRIVATE ${PROJ_ISA_FILES} )
target_sources(${TARGET_NAME} PRIVATE ${PROJ_FMT_FILES} )
target_include_directories(${TARGET_NAME} PRIVATE ${PROJ_INC_DIR}       )
target_include_directories(${TARGET_NAME} PRIVATE ${PROJ_CORE_INC_DIR}  )
target_include_directories(${TARGET_NAME} PRIVATE ${PROJ_SUPPORT_INC_DIR})

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/keystone/include)
target_link_libraries(${TARGET_NAME} PRIVATE keystone stdc++ m)

install(TARGETS ${TARGET_NAME} DESTINATION ${TARGET_INSTALL_PATH})

option(LEXER_DEBUG "Enable lexer debug macros" OFF)
option(PARSER_DEBUG "Enable parser debug macros" OFF)
option(IR_DEBUG "Enable IR debug macros" OFF)

if(LEXER_DEBUG)
    target_compile_definitions(${TARGET_NAME} PRIVATE LEXER_DEBUG)
endif()

if(PARSER_DEBUG)
    target_compile_definitions(${TARGET_NAME} PRIVATE PARSER_DEBUG)
endif()

if(IR_DEBUG)
    target_compile_definitions(${TARGET_NAME} PRIVATE IR_DEBUG)
endif()


# For predefined targets put them all into a folder (if possible)
foreach(predef_target ALL_BUILD ZERO_CHECK INSTALL PACKAGE RUN_TESTS)
    if(TARGET ${predef_target})
        set_target_properties(${predef_target} PROPERTIES FOLDER "CMake Predefined Targets")
    endif()
endforeach()
