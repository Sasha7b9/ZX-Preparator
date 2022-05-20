# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (TAPview)

set(CMAKE_UNITY_BUILD OFF)

include_directories(.
                    ../ThirdParty/wxWidgets/include
                    ../generated/ThirdParty/lib/vc_dll/mswud/)

if(WIN32)

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

    add_compile_options(/wd4514)
    add_compile_options(/wd4625)
    add_compile_options(/wd4626)
    add_compile_options(/wd4820)
    add_compile_options(/wd5026)
    add_compile_options(/wd5027)
    add_compile_options(/wd5039)

else()

endif()

file(GLOB SRC                      *.h *.cpp)

source_group(src                   FILES ${SRC})

add_executable(${PROJECT_NAME} ${SRC}
)


target_link_libraries(${PROJECT_NAME} wxbase31ud wxmsw31ud_core Ws2_32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\ThirdParty\\lib\\vc_dll\\wxbase316ud_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\ThirdParty\\lib\\vc_dll\\wxmsw316ud_core_vc_custom.dll $(OutputPath) /Y)
