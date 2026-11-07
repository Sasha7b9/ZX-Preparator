# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (ZX-Preparator)

set(CMAKE_UNITY_BUILD ON)

include_directories(
    src/
    ../ThirdParty/wxWidgets/include/
    ../generated/ThirdParty/lib/vc_x64_lib/mswu/
)

link_directories(../generated/ThirdParty/lib/vc_x64_lib)

if(WIN32)

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -std:c++20")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /STACK:10000000")

    add_compile_options(
        /wd4514     # 'xxx': unreferenced inline function has been removed
        /wd4625     # 'xxx': copy constructor was implicitly defined as deleted
        /wd4626     # 'xxx': assignment operator was implicitly defined as deleted
        /wd4820     # 'xxx': 'y' bytes padding added after data member 'zzz'
        /wd4865     # the underlying type of '<unnamed-enum-DISPLAYCONFIG_OUTPUT_TECHNOLOGY_OTHER>' will change when '/Zc:enumTypes' is specified on the command line
        /wd5026     # 'xxx': move constructor was implicitly defined as deleted
        /wd5027     # 'xxx': move assignment operator was implicitly defined as deleted
        /wd5039     # 'xxx': pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws an exception.
        /wd5045     # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    )
else()

endif()

file(GLOB SRC            ./src/*.*)
file(GLOB COMPUTER       ./src/Computer/*.*)
file(GLOB CONTROLS       ./src/Controls/*.*)
file(GLOB CPU            ./src/CPU/*.*)
file(GLOB EMULATOR       ./src/Emulator/*.*)
file(GLOB DISPLAY        ./src/Display/*.*)
file(GLOB NOTEBOOK       ./src/Notebook/*.*)
file(GLOB PARSER         ./src/Parser/*.*)
file(GLOB UTILS          ./src/Utils/*.*)

source_group(src         FILES ${SRC})
source_group(Computer    FILES ${COMPUTER})
source_group(Controls    FILES ${CONTROLS})
source_group(CPU         FILES ${CPU})
source_group(Emulator    FILES ${EMULATOR})
source_group(Display     FILES ${DISPLAY})
source_group(Notebook    FILES ${NOTEBOOK})
source_group(Parser      FILES ${PARSER})
source_group(Utils       FILES ${UTILS})


set_source_files_properties(./src/Parser/ParserTAP_.cpp
    PROPERTIES HEADER_FILE_ONLY ON)


add_executable(${PROJECT_NAME}
    ${SRC}
    ${COMPUTER}
    ${CONTROLS}
    ${CPU}
    ${EMULATOR}
    ${DISPLAY}
    ${NOTEBOOK}
    ${PARSER}
    ${UTILS}
)


target_link_libraries(${PROJECT_NAME}
    wxbase33u
    wxmsw33u_core
    wxpng
    wxzlib
    comctl32
    rpcrt4
)

