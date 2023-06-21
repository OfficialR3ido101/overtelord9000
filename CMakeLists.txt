cmake_minimum_required(VERSION 3.0)

project(overtelord9000)

add_executable(overtelord9000 src/main.cpp)

install(TARGETS overtelord9000 RUNTIME DESTINATION bin)
include_directories("/opt/vcpkg/installed/x64-linux/include/")
find_package(dpp REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Opus CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(overtelord9000 PRIVATE unofficial-sodium::sodium unofficial-sodium::sodium_config_public)
target_link_libraries(overtelord9000 PRIVATE ZLIB::ZLIB)
target_link_libraries(overtelord9000 PRIVATE Opus::opus)
target_link_libraries(overtelord9000 PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(overtelord9000 PRIVATE dpp::dpp)