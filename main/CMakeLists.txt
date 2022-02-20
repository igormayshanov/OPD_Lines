add_executable(Lines main.cpp field.h field.cpp ball.h ball.cpp utils.h utils.cpp findPath.h findPath.cpp)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(Freetype REQUIRED)
find_package(JPEG REQUIRED)
find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

target_include_directories(Lines PRIVATE ${SFML_INCLUDE_DIR})
target_compile_features(Lines PUBLIC cxx_std_17)
target_compile_definitions(Lines PRIVATE SFML_STATIC)

target_link_libraries(Lines ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})