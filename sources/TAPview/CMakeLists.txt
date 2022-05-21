# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (TAPview)

set(CMAKE_UNITY_BUILD OFF)

include_directories(src/
                    ../ThirdParty/wxWidgets/include/
                    ../generated/ThirdParty/lib/vc_dll/mswud/)

link_directories(../generated/ThirdParty/lib/vc_dll)

add_definitions(-DWXUSINGDLL)

if(WIN32)

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

    add_compile_options(/wd4514)
    add_compile_options(/wd4623)
    add_compile_options(/wd4625)
    add_compile_options(/wd4626)
    add_compile_options(/wd4820)
    add_compile_options(/wd5026)
    add_compile_options(/wd5027)
    add_compile_options(/wd5039)
    add_compile_options(/wd5045)

else()

endif()

file(GLOB SRC       src/*.*)

source_group(src    FILES ${SRC})

add_executable(${PROJECT_NAME} ${SRC}
)


target_link_libraries(${PROJECT_NAME} wxbase31ud wxmsw31ud_core)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase315ud_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw315ud_core_vc_custom.dll $(OutputPath) /Y)
