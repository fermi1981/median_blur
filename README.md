# median_blur
图像中值滤波，xcode工程，支持bmp24和bmp32

命令行:
exe {input bmp file} {output bmp file} {选项}
选项:
-bw|--bw:设置窗体宽（默认3）
-bh|--bh:设置窗体高（默认3）
示例:
exe {input bmpfile} {output bmp file} -bw 3 -bh 3

注意：这个算法是全通道统一处理，效果不一定好。如果需要对每一个通道处理，需要自行完成。
