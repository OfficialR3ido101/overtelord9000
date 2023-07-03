cmake_minimum_required(VERSION 3.1.0)

project(overtelord9000)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpermissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -fconcepts" )

add_executable(overtelord9000
src/main.cpp
src/websocket/websocketserver.cpp
src/discord/Discordbot.cpp
src/matrix/Matrixbot.cpp
src/utils.cpp
)

install(TARGETS overtelord9000 RUNTIME DESTINATION bin)
include_directories("/opt/vcpkg/installed/x64-linux/include/")
find_package(dpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Opus CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Quotient CONFIG REQUIRED)
target_link_libraries(overtelord9000 PRIVATE websocketpp::websocketpp)
target_link_libraries(overtelord9000 PRIVATE unofficial-sodium::sodium unofficial-sodium::sodium_config_public)
target_link_libraries(overtelord9000 PRIVATE ZLIB::ZLIB)
target_link_libraries(overtelord9000 PRIVATE Opus::opus)
target_link_libraries(overtelord9000 PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(overtelord9000 PRIVATE dpp::dpp)
target_link_libraries(overtelord9000 PRIVATE Qt5::Widgets)
target_link_libraries(overtelord9000 PRIVATE Quotient)
