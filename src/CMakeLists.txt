file(GLOB SOURCES *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

set(PCG32_DIR "/home/GuiDesktop/.conan2/p/pcg-c4aa2cafcf36e3/p/include")
set(XOSHIRO_DIR "/home/GuiDesktop/.conan2/p/xoshi1a4816164a331/p/include")

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${PCG32_DIR}
    ${XOSHIRO_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    fmt::fmt
    OpenMP::OpenMP_CXX
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -g
    $<$<CONFIG:Debug>:>
    $<$<CONFIG:Release>:-O3>
)
