# MES204 Final Project - STM32F446xx 2004 LCD Graphics Engine

<details>
<summary>中文</summary>

这是一个为 MES204 课程设计的期末项目，基于 STM32F446xx 微控制器，在标准的 20x4 字符型 LCD 上实现了一个功能丰富的图形引擎。项目旨在探索在有限的显示硬件上实现高级图形效果的可能性，例如 3D 渲染、粒子系统和视频播放。

</details>

This is a final project for the MES204 course, based on the STM32F446xx microcontroller. It implements a feature-rich graphics engine on a standard 20x4 character LCD. The project aims to explore the possibilities of achieving advanced graphical effects, such as 3D rendering, particle systems, and video playback, on limited display hardware.

## ✨ Features

<details>
<summary>中文</summary>

- **高性能 LCD 驱动**: 使用 8 位并行接口直接驱动 HD44780 兼容的 LCD，并针对速度进行了优化。
- **图形基元**: 实现了基本的图形绘制功能，作为上层效果的基础。
- **3D 渲染引擎**: 一个简单的线框 3D 渲染器，能够绘制和变换 3D 对象。
- **数学特效**: 利用数学函数在字符 LCD 上生成动态的视觉效果。
- **粒子系统**: 一个基础的粒子引擎，用于创建爆炸、火花等效果。
- **字符视频播放器**: 能够将预处理的视频数据流在 LCD 上播放，创造出独特的动画效果。
- **现代构建系统**: 使用 CMake 和 ARM GCC 工具链，易于配置和构建。

</details>

- **High-Performance LCD Driver**: Directly drives an HD44780-compatible LCD using an 8-bit parallel interface, optimized for speed.
- **Graphics Primitives**: Implements basic graphics drawing functions that serve as a foundation for higher-level effects.
- **3D Rendering Engine**: A simple wireframe 3D renderer capable of drawing and transforming 3D objects.
- **Mathematical Effects**: Utilizes mathematical functions to generate dynamic visual effects on the character LCD.
- **Particle System**: A basic particle engine for creating effects like explosions, sparks, etc.
- **Character Video Player**: Capable of playing pre-processed video data streams on the LCD, creating unique animations.
- **Modern Build System**: Uses CMake and the ARM GCC toolchain for easy configuration and building.

## 🧠 Core Concepts

<details>
<summary>中文</summary>

### 3D 线框渲染

3D 引擎在一个由顶点和边定义的简单线框模型上运行。渲染管线涉及几个关键的数学步骤：
1.  **模型变换**: 使用 4x4 矩阵对 3D 顶点进行旋转、缩放和平移变换。旋转通常通过从欧拉角或四元数派生的旋转矩阵来实现。
2.  **投影**: 将 3D 世界坐标投影到 2D 平面上。考虑到显示的特性，我们使用正交投影，它会丢弃深度信息但能保持平行线。
3.  **光栅化**: 将 2D 线条绘制到字符网格上。这是通过自定义的画线算法（很可能是布雷森汉姆算法的改编版）实现的，该算法确定激活哪些“像素”（或自定义字符）来表示线条。

### 程序化图形 (`math_effect.c`)

此模块利用数学函数在有限的字符显示器上创建复杂的视觉效果。其核心思想是，屏幕上的每个字符单元 `(x, y)` 的状态由一个以其坐标和时间 `t` 为输入的函数 `f(x, y, t)` 决定。通过精心设计函数，可以生成各种动态模式：
- **等离子效果**: 通常通过叠加多个不同频率和相位的正弦或余弦函数来实现。
- **隧道效果**: 通过将笛卡尔坐标 `(x, y)` 转换为极坐标 `(r, θ)` 并应用函数来实现，从而创建径向或旋转的图案。

### 视频转换脚本 (`generator.py`)

`generator.py` 脚本是一个关键的预处理工具，它使用 OpenCV 库将标准视频文件（如 `.mp4`）转换为一个巨大的 C 语言头文件 (`.h`)。这个头文件包含了可直接在 STM32 上使用的动画数据。其工作流程如下：
1.  **加载和调整大小**: 脚本逐帧读取视频，将每一帧转换为灰度图，并将其大小调整为 `20x16` 像素的低分辨率图像。
2.  **二值化**: 应用一个阈值将灰度图像转换为纯黑白（二进制）图像。每个像素要么是 1 (亮)，要么是 0 (暗)。
3.  **数据打包**: 这是最关键的一步。`20x16` 像素的图像被视为一个由 8 个自定义字符组成的网格（2行4列）。每个自定义字符是 `5x8` 像素。脚本遍历这 8 个字符块，并将每个字符块的 `5x8` 像素数据打包成 8 个字节。每一行 5 个像素被编码成一个字节，其中每个比特对应一个像素的状态。
4.  **生成头文件**: 最终，所有帧的数据都被格式化成一个巨大的 C 数组，如 `const uint8_t VIDEO_DATA[NUM_FRAMES][64]`，其中每一帧由 64 个字节（8个字符 * 8字节/字符）表示。然后，主应用程序可以逐帧加载这些数据并更新 LCD 的 CGRAM，从而在字符显示器上“播放”视频。

</details>

### 3D Wireframe Rendering

The 3D engine operates on a simple wireframe model defined by vertices and edges. The rendering pipeline involves several key mathematical steps:
1.  **Model Transformation**: 3D vertices are transformed using 4x4 matrices for rotation, scaling, and translation. Rotations are typically achieved using rotation matrices derived from Euler angles.
2.  **Projection**: The 3D world coordinates are projected onto a 2D plane. Given the nature of the display, an orthographic projection is used, which discards depth information but preserves parallel lines.
3.  **Rasterization**: The 2D lines are drawn onto the character grid. This is achieved by a custom line-drawing algorithm, likely an adaptation of Bresenham's algorithm, which determines which "pixels" (or custom characters) to activate to represent the line.

### Procedural Graphics (`math_effect.c`)

This module leverages mathematical functions to create complex visual effects on a limited character display. The core idea is that the state of each character cell `(x, y)` on the screen is determined by a function `f(x, y, t)` that takes its coordinates and time `t` as input. By carefully designing the function, various dynamic patterns can be generated:
- **Plasma Effects**: Often created by summing multiple sine or cosine functions with different frequencies and phases.
- **Tunnel Effects**: Achieved by converting Cartesian coordinates `(x, y)` to polar coordinates `(r, θ)` and applying functions to create radial or rotating patterns.

### Video Conversion Script (`generator.py`)

The `generator.py` script is a crucial preprocessing tool that uses the OpenCV library to convert a standard video file (e.g., `.mp4`) into a large C header file (`.h`). This header contains animation data ready to be used on the STM32. The workflow is as follows:
1.  **Load and Resize**: The script reads the video frame by frame, converts each frame to grayscale, and resizes it to a low-resolution `20x16` pixel image.
2.  **Binarization**: A threshold is applied to convert the grayscale image into a pure black-and-white (binary) image. Each pixel is either 1 (on) or 0 (off).
3.  **Data Packing**: This is the most critical step. The `20x16` pixel image is treated as a grid of 8 custom characters (2 rows by 4 columns). Each custom character is `5x8` pixels. The script iterates through these 8 character blocks and packs the `5x8` pixel data of each into 8 bytes. Each 5-pixel row is encoded into a single byte, where each bit corresponds to a pixel's state.
4.  **Header Generation**: Finally, the data for all frames is formatted into a large C array, such as `const uint8_t VIDEO_DATA[NUM_FRAMES][64]`, where each frame is represented by 64 bytes (8 characters * 8 bytes/character). The main application can then load this data frame by frame and update the LCD's CGRAM to "play" the video on the character display.

## Hardware Requirements

<details>
<summary>中文</summary>

- **微控制器**: STM32F446xx 系列 MCU。
- **显示器**: 一块标准的 20x4 字符型 LCD (兼容 HD44780 控制器)。
- **接线**:
    - **LCD 数据端口 (D0-D7)**: `PC2` - `PC9`
    - **LCD 控制端口**:
        - `RS` (Register Select): `PB8`
        - `EN` (Enable): `PB9`

*请注意：接线定义在 `Core/UserDriver/lcd_extreme.c` 文件中，您可以根据实际硬件连接进行修改。*

</details>

- **Microcontroller**: STM32F446xx series MCU.
- **Display**: A standard 20x4 character LCD (HD44780 compatible).
- **Wiring**:
    - **LCD Data Port (D0-D7)**: `PC2` - `PC9`
    - **LCD Control Port**:
        - `RS` (Register Select): `PB8`
        - `EN` (Enable): `PB9`

*Note: Wiring is defined in `Core/UserDriver/lcd_extreme.c`. You can modify it according to your actual hardware connections.*

## 🛠️ Software & Toolchain

<details>
<summary>中文</summary>

- **构建系统**: CMake 3.22+
- **编译器**: GNU Arm Embedded Toolchain (arm-none-eabi-gcc)
- **IDE (可选)**: CLion, VS Code with CMake Tools, or any other C/C++ IDE.
- **代码生成**: STM32CubeMX (项目包含 `.ioc` 文件 `mes204_new.ioc`)

</details>

- **Build System**: CMake 3.22+
- **Compiler**: GNU Arm Embedded Toolchain (arm-none-eabi-gcc)
- **IDE (Optional)**: CLion, VS Code with CMake Tools, or any other C/C++ IDE.
- **Code Generation**: STM32CubeMX (The project includes the `.ioc` file `mes204_new.ioc`)

## 🚀 How to Build

<details>
<summary>中文</summary>

该项目配置为使用 CLion 或手动通过 CMake 进行构建。

### 使用 CLion

1.  在 CLion 中打开项目文件夹。
2.  配置一个 "Embedded GDB Server" 运行/调试配置。
3.  选择您的调试探针 (e.g., ST-Link)。
4.  点击 "Build" 和 "Run" 或 "Debug"。

### 手动构建 (命令行)

1.  **安装工具链**: 确保 `arm-none-eabi-gcc` 在您的系统路径中。
2.  **创建构建目录**:
    ```bash
    mkdir build
    cd build
    ```
3.  **运行 CMake 配置**:
    ```bash
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=Debug
    ```
4.  **编译项目**:
    ```bash
    cmake --build .
    ```
5.  **烧录**:
    生成的 `.hex` 和 `.bin` 文件位于 `build` 目录中。您可以使用 STM32CubeProgrammer 或您选择的任何工具将固件烧录到目标板上。

</details>

This project is configured to be built with CLion or manually via CMake.

### Using CLion

1.  Open the project folder in CLion.
2.  Configure an "Embedded GDB Server" run/debug configuration.
3.  Select your debug probe (e.g., ST-Link).
4.  Click "Build" and "Run" or "Debug".

### Manual Build (Command Line)

1.  **Install Toolchain**: Ensure `arm-none-eabi-gcc` is in your system's PATH.
2.  **Create Build Directory**:
    ```bash
    mkdir build
    cd build
    ```
3.  **Run CMake Configuration**:
    ```bash
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=Debug
    ```
4.  **Compile the Project**:
    ```bash
    cmake --build .
    ```
5.  **Flash**:
    The generated `.hex` and `.bin` files are located in the `build` directory. You can use STM32CubeProgrammer or any tool of your choice to flash the firmware onto the target board.

## 📂 Project Structure

<details>
<summary>中文</summary>

```
.
├── Core/
│   ├── Inc/          # 主要头文件 (main.h, graphics.h, etc.)
│   ├── Src/          # 主要源文件 (main.c, stm32f4xx_it.c, etc.)
│   └── UserDriver/   # 用户自定义的驱动和应用逻辑 (lcd_extreme.c, graphics_3d.c)
├── Drivers/
│   ├── CMSIS/        # ARM Cortex Microcontroller Software Interface Standard
│   └── STM32F4xx_HAL_Driver/ # STMicroelectronics HAL 库
├── cmake/            # CMake 配置文件和工具链文件
├── CMakeLists.txt    # 主 CMake 构建脚本
└── mes204_new.ioc    # STM32CubeMX 配置文件
```

</details>

```
.
├── Core/
│   ├── Inc/          # Main header files (main.h, graphics.h, etc.)
│   ├── Src/          # Main source files (main.c, stm32f4xx_it.c, etc.)
│   └── UserDriver/   # User-defined drivers and application logic (lcd_extreme.c, graphics_3d.c)
├── Drivers/
│   ├── CMSIS/        # ARM Cortex Microcontroller Software Interface Standard
│   └── STM32F4xx_HAL_Driver/ # STMicroelectronics HAL Library
├── cmake/            # CMake configuration and toolchain files
├── CMakeLists.txt    # Main CMake build script
└── mes204_new.ioc    # STM32CubeMX configuration file
```

## 🤝 Contributing

<details>
<summary>中文</summary>

欢迎通过 Pull Requests 或 Issues 对该项目做出贡献。

</details>

Contributions to this project are welcome via Pull Requests or Issues.

## 📄 License

<details>
<summary>中文</summary>

该项目遵循 MIT 许可。有关详细信息，请参阅 `LICENSE` 文件。

</details>

This project is licensed under the MIT License. See the `LICENSE` file for details.


