
file(GLOB HEADERS
        "2.intermediate_opengl/31.selecting3d/*.h"
        "../include/ogldev/*.h"

)

file(GLOB SOURCES
        "2.intermediate_opengl/31.selecting3d/*.cpp"
)

add_executable(selecting3d ${HEADERS} ${SOURCES})
target_link_libraries(selecting3d ${LIBS})