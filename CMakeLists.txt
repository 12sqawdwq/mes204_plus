cmake_minimum_required(VERSION 3.22)

# ==============================================================================
# 1. 基础配置
# ==============================================================================
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_PROJECT_NAME mes204_new)
project(${CMAKE_PROJECT_NAME} C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# ==============================================================================
# 2. 架构参数
# ==============================================================================
set(MCU_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
)
add_compile_options(${MCU_FLAGS})
add_link_options(${MCU_FLAGS})

# ==============================================================================
# 3. 搜寻源文件 (做减法，而不是做加法)
# ==============================================================================
# 搜集 Core/Src 下的所有文件
file(GLOB_RECURSE SYSTEM_SOURCES "Core/Src/*.c")

# 【核心修正 1】剔除 system_stm32f4xx.c
# 因为报错显示 STM32_Drivers 库里已经有它了，我们不能再编译一次
list(FILTER SYSTEM_SOURCES EXCLUDE REGEX "system_stm32f4xx.c$")

# 【核心修正 2】不要搜集 Core/UserDriver
# 报错证明 STM32_Drivers 已经偷偷把 UserDriver 里的文件编译了。
# 所以我们这里什么都不用做，只要在后面链接库就行了。

# 链接脚本
file(GLOB LINKER_SCRIPT "STM32*.ld")

# ==============================================================================
# 4. 创建可执行文件
# ==============================================================================
add_executable(${CMAKE_PROJECT_NAME}
        ${SYSTEM_SOURCES}       # 只包含 main.c, stm32f4xx_it.c 等
        ${LINKER_SCRIPT}
        # 即使我们不编译源文件，把头文件加进来是为了方便在 IDE 里跳转查看
        Core/UserDriver/lcd_extreme.c
        Core/Inc/lcd_extreme.h
        Core/Inc/graphics.h
        Core/Inc/video_data.h
        Core/UserDriver/graphics_3d.c
        Core/Inc/graphics_3d.h
        Core/UserDriver/video_player.c
        Core/Inc/video_player.h
)

# ==============================================================================
# 5. 依赖与库
# ==============================================================================
add_subdirectory(cmake/stm32cubemx)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        Core/Inc
        Core/UserDriver  # 头文件路径必须保留，否则 main.c 找不到 .h
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32cubemx  # 你的 graphics.c, lcd_extreme.c, system_*.c 都在这个黑盒子里
        m            # 数学库
)

# ==============================================================================
# 6. 生成 Hex/Bin
# ==============================================================================
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
        -T${LINKER_SCRIPT}
        -Wl,-Map=${CMAKE_PROJECT_NAME}.map
        -Wl,--gc-sections
        -u _printf_float
        -Wl,--print-memory-usage
        --specs=nano.specs
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
        COMMENT "Generating .hex and .bin files..."
)