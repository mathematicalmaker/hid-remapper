# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/jason/Dev/Pico/hid-remapper/firmware/pico-sdk/tools/pioasm"
  "/home/jason/Dev/Pico/hid-remapper/firmware/build_pico/pioasm"
  "/home/jason/Dev/Pico/hid-remapper/firmware/build_pico/pioasm-install"
  "/home/jason/Dev/Pico/hid-remapper/firmware/build_pico/pico-sdk/src/rp2_common/tinyusb/pioasm/tmp"
  "/home/jason/Dev/Pico/hid-remapper/firmware/build_pico/pico-sdk/src/rp2_common/tinyusb/pioasm/src/pioasmBuild-stamp"
  "/home/jason/Dev/Pico/hid-remapper/firmware/build_pico/pico-sdk/src/rp2_common/tinyusb/pioasm/src"
  "/home/jason/Dev/Pico/hid-remapper/firmware/build_pico/pico-sdk/src/rp2_common/tinyusb/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/jason/Dev/Pico/hid-remapper/firmware/build_pico/pico-sdk/src/rp2_common/tinyusb/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/jason/Dev/Pico/hid-remapper/firmware/build_pico/pico-sdk/src/rp2_common/tinyusb/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
