cmake_minimum_required(VERSION 3.12)
project(Winjet VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 23)

include_directories("C:/Program Files/boost/boost_1_81_0")
link_directories("C:/Program Files/boost/boost_1_81_0/stage/lib")
link_libraries("odbc32.lib")
add_compile_definitions(UNICODE _WIN32_WINNT=0x0601)

add_executable(
    Winjet WIN32
    src/main.cpp
    src/CallBackTimer.cpp
    src/controller.cpp
    src/messages.cpp
    src/my_ssql.cpp
    src/paint.cpp
    src/process.cpp
    src/ssql.cpp
    src/util.cpp
    src/Components/Ball.cpp
    src/Components/Command.cpp
    src/Components/Component.cpp
    src/Components/DigitalClock.cpp
    src/Components/Text.cpp
    src/Widgets/canvas.cpp
    src/Widgets/window.cpp
)

add_executable(
    CrashRestore
    src/CrashRestore/CrashRestore.cpp
)

add_executable(
    Test
    src/Test/Test.cpp
)

add_custom_command(
    TARGET Winjet POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "C:/Users/eldeeb/source/repos/mhdeeb/Winjet/resource" $<TARGET_FILE_DIR:Winjet>/resource
)