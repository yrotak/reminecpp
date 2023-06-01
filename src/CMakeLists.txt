include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty)
link_directories(${CMAKE_SOURCE_DIR}/libs/macos/arm64)
file(GLOB_RECURSE SOURCES *.cpp *.c)
file(GLOB_RECURSE HEADERS *.hpp *.h *.inl)

add_executable(game ${SOURCES} ${HEADERS})

target_include_directories(game PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_NET_INCLUDE_DIRS} ${CURL_INCLUDE_DIR})
target_link_libraries(game PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_net OpenGL::GL GLEW::GLEW glm::glm cryptopp ${CURL_LIBRARIES} OpenSSL::SSL ZLIB::ZLIB)