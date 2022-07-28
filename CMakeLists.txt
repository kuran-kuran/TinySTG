cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(Pico_ePaper_Code)
pico_sdk_init()

# Add a compilation subdirectory
add_subdirectory(lib/Config)
add_subdirectory(lib/Infrared)
add_subdirectory(lib/Icm20948)
add_subdirectory(lib/LCD)
add_subdirectory(lib/Fonts)
add_subdirectory(lib/GUI)

# Add a header directory
include_directories(./lib/Config)
include_directories(./lib/GUI)
include_directories(./lib/Icm20948)
include_directories(./lib/LCD)
include_directories(./lib/Infrared)

# create an executable file
add_executable(TinySTG 
src/TinyLibrary/AdpcmDecorder.cpp
src/TinyLibrary/Mathmatics.cpp
src/TinyLibrary/SoundMixer.cpp
src/TinyLibrary/SoundPlayer.cpp
src/TinyLibrary/Pico/Controller.cpp
src/TinyLibrary/Pico/Screen.cpp
src/TinyLibrary/Pico/SoundStream.cpp
src/TinyLibrary/Pico/TinyFile.cpp
src/Boss1.cpp
src/Boss2.cpp
src/Boss3.cpp
src/Boss4.cpp
src/Boss5.cpp
src/Boss6.cpp
src/Bullet.cpp
src/Enemy.cpp
src/Enemy1.cpp
src/Enemy2.cpp
src/Enemy3.cpp
src/Enemy4.cpp
src/Enemy5.cpp
src/Explode.cpp
src/Game.cpp
src/Global.cpp
src/MainLoop.cpp
src/Player.cpp
src/PowerUp.cpp
src/Resource.cpp
src/Shot.cpp
src/Sound.cpp
TinySTG.cpp
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(TinySTG 0)
#pico_enable_stdio_uart(TinySTG 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(TinySTG)

target_link_libraries(TinySTG LCD Infrared Icm20948 GUI Fonts Config pico_stdlib hardware_spi)