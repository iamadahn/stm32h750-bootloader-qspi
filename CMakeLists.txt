cmake_minimum_required(VERSION 3.17)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Add you project name
project(bootloader C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_compile_options(-mcpu=cortex-m7 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_compile_options(-lc -lm -lnosys -specs=nano.specs)
add_compile_options(-Wall)

add_compile_options(-O0 -g)

# Add Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/cmsis
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/hal/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys
)

# Add you source file
file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/hal/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/sys/*.c"
)


# Add linker file
file(GLOB LINKER_SCRIPT "*.ld")
message(STATUS "Found linker script - ${LINKER_SCRIPT}")

# this options for C++
add_link_options(-specs=nano.specs -lc -lm)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

add_definitions(
    -DUSE_HAL_DRIVER
    -DSTM32H750xx
)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

