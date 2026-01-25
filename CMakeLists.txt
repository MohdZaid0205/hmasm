cmake_minimum_required(VERSION 3.10)

set(PROJ_NAME "HMASM"           )   # this is implementation of assembler.
set(PROJ_LANG LANGUAGES C       )   # to be written in pure `C` no external libs.
set(PROJ_VERS VERSION  0.00.3   )   # need to make sure to change this version.
set(PROJ_DESC DESCRIPTION "implementation of assembler"         )

project(${PROJ_NAME} ${PROJ_DESC} ${PROJ_LANG} ${PROJ_VERS})

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

# by default we compile all default isa(s) and format(s) otherwise user defined
set(DEFINE_FMT "elf;pc;bin"         )
set(DEFINE_ISA "i386;riscv32;arm32" )

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
add_executable(${TARGET_NAME} )
target_sources(${TARGET_NAME} PRIVATE ${PROJ_SRC_FILES} )
target_sources(${TARGET_NAME} PRIVATE ${PROJ_INC_FILES} )
target_sources(${TARGET_NAME} PRIVATE ${PROJ_ISA_FILES} )
target_sources(${TARGET_NAME} PRIVATE ${PROJ_FMT_FILES} )
target_include_directories(${TARGET_NAME} PRIVATE ${PROJ_INC_DIR}       )
target_include_directories(${TARGET_NAME} PRIVATE ${PROJ_CORE_INC_DIR}  )
target_include_directories(${TARGET_NAME} PRIVATE ${PROJ_SUPPORT_INC_DIR})
install(TARGETS ${TARGET_NAME} DESTINATION ${TARGET_INSTALL_PATH})

# For predefined targets put them all into a folder (if possible)
foreach(predef_target ALL_BUILD ZERO_CHECK INSTALL PACKAGE RUN_TESTS)
    if(TARGET ${predef_target})
        set_target_properties(${predef_target} PROPERTIES FOLDER "CMake Predefined Targets")
    endif()
endforeach()
