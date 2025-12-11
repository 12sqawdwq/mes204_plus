import cv2
import numpy as np
import os

# ================= 配置区域 =================
# 视频文件路径 (请修改为你电脑上的实际视频路径)
VIDEO_PATH = 'D:\LEARNING\XJTLUCHIPS\MES303TC\MES303TC\\bad-apple.mp4'
# 输出文件名
OUTPUT_FILE = 'D:\LEARNING\XJTLUCHIPS\MES303TC\MES303TC\\bad-apple.h'
# 逻辑分辨率：只生成 8 个字符的数据
# 4个字符宽 * 5像素 = 20
# 2个字符高 * 8像素 = 16
TARGET_W = 20
TARGET_H = 16


# =======================================

def generate_header():
    cap = cv2.VideoCapture(VIDEO_PATH)
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    print(f"处理视频: {VIDEO_PATH}, 目标分辨率: 20x16, 总帧数: {total_frames}")

    c_data = []

    # 预览窗口
    cv2.namedWindow("Preview (20x16)", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("Preview (20x16)", 400, 320)

    processed_count = 0

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        # 1. 转灰度 & 缩放
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        resized = cv2.resize(gray, (TARGET_W, TARGET_H), interpolation=cv2.INTER_AREA)

        # 2. 二值化 (关键步骤：转为纯黑白)
        # 阈值设为 128，大于128为亮(不显示)，小于128为黑(显示像素)
        # LCD1602 上 1 是黑点，0 是不显示。通常视频里黑色背景是0，白色主体是255。
        # 如果是 Bad Apple (黑影白底)，我们需要反色，或者根据视觉调整。
        # 这里假设：视频里的【白色】区域点亮像素(1)，【黑色】区域熄灭(0)
        _, binary = cv2.threshold(resized, 128, 1, cv2.THRESH_BINARY)

        # 预览
        cv2.imshow("Preview (20x16)", binary * 255)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        # 3. 打包数据为 CGRAM 格式
        # 我们需要生成 64 个字节 (8个字符 * 8行)
        frame_bytes = []

        # 字符块顺序：先第一行4个(0-3)，再第二行4个(4-7)
        # 坐标定义：(x_start, y_start)
        block_origins = [
            (0, 0), (5, 0), (10, 0), (15, 0),  # Top Row
            (0, 8), (5, 8), (10, 8), (15, 8)  # Bot Row
        ]

        for (bx, by) in block_origins:
            # 对于每个字符块，遍历 8 行
            for r in range(8):
                row_val = 0
                # 遍历 5 列
                for c in range(5):
                    pixel = binary[by + r][bx + c]
                    if pixel:
                        # 1602 的像素排列：高位在左还是右？
                        # 通常 0x10 是最左边，0x01 是最右边
                        row_val |= (1 << (4 - c))

                frame_bytes.append(f"0x{row_val:02X}")

        # 格式化 C 数组
        c_data.append("    {" + ", ".join(frame_bytes) + "},")

        processed_count += 1
        if processed_count % 100 == 0:
            print(f"已处理: {processed_count}")

    cap.release()
    cv2.destroyAllWindows()

    # 写入文件
    with open(OUTPUT_FILE, 'w') as f:
        f.write(f"#include <stdint.h>\n\n")
        f.write(f"const uint32_t TOTAL_FRAMES = {processed_count};\n\n")
        # 每一帧 64 字节
        f.write(f"const uint8_t VIDEO_DATA[{processed_count}][64] = {{\n")
        for line in c_data:
            f.write(line + "\n")
        f.write("};\n")

    print(f"完成！生成 {OUTPUT_FILE}")


if __name__ == "__main__":
    generate_header()