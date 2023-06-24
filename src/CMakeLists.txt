include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty)


if(APPLE)
    link_directories(${CMAKE_SOURCE_DIR}/libs/macos/arm64)
endif()

if(UNIX AND NOT APPLE)
    link_directories(${CMAKE_SOURCE_DIR}/libs/linux)
endif()



file(GLOB_RECURSE SOURCES *.cpp *.c)
file(GLOB_RECURSE HEADERS *.hpp *.h *.inl)

add_executable(game ${SOURCES} ${HEADERS})

target_include_directories(game PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_NET_INCLUDE_DIRS} ${CURL_INCLUDE_DIR})

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/thirdparty/imgui)

add_library("imgui" "${IMGUI_DIR}/imgui.cpp"
                  "${IMGUI_DIR}/imgui_demo.cpp"
                  "${IMGUI_DIR}/imgui_draw.cpp"
                  "${IMGUI_DIR}/imgui_tables.cpp"
                  "${IMGUI_DIR}/imgui_widgets.cpp"
                  
                  "${IMGUI_DIR}/imgui_impl_opengl3.cpp"
                  "${IMGUI_DIR}/imgui_impl_sdl2.cpp")

target_include_directories("imgui" PRIVATE "${IMGUI_DIR}" ${SDL2_INCLUDE_DIRS})

target_link_libraries(game PRIVATE ${SDL2_LIBRARIES} SDL2_net OpenGL::GL GLEW::GLEW glm::glm cryptopp ${CURL_LIBRARIES} OpenSSL::SSL ZLIB::ZLIB imgui)

if(UNIX AND NOT APPLE)
target_link_libraries(game PRIVATE pthread)
endif()