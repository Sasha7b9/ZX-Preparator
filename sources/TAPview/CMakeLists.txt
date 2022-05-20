# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (TAPview)

set(CMAKE_UNITY_BUILD OFF)

include_directories(.)

if(WIN32)

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

else()

endif()

file(GLOB SRC                      *.*)

source_group(src                   FILES ${SRC})

add_library(${PROJECT_NAME} ${SRC}
)
