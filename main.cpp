#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "vector.hpp"
#include "thread.hpp"
#include <vector>
#include <iostream>

#include <thread>
#include <future>
#include <algorithm>  // For std::clamp

// 全局变量：图像宽度和高度
const int g_width = 800;  // 例如，宽度 800
const int g_height = 600; // 例如，高度 600

// 将部分像素数据转换为 unsigned char
void convert_to_byte_array(
    const std::vector<vec3>& pixels,
    std::vector<unsigned char>& imageData,
    int startIndex,
    int endIndex
) {
    for (int i = startIndex; i < endIndex; ++i) {
        int x = i % g_width;
        int y = i / g_width;

#define DEBUG 1
#ifdef DEBUG
        imageData[i * 3 + 0] = static_cast<unsigned char>(std::clamp((0.5 + x * 1.0 / (double)g_width) * 255.0, 0.0, 255.0));
        imageData[i * 3 + 1] = static_cast<unsigned char>(std::clamp((0.7 + x * 1.0 / (double)g_width) * 255.0, 0.0, 255.0));
        imageData[i * 3 + 2] = static_cast<unsigned char>(std::clamp((1.0 + x * 1.0 / (double)g_width) * 255.0, 0.0, 255.0));
#elif
        // 将像素值转换为 unsigned char，并进行范围限制（0 到 255）
        imageData[i * 3 + 0] = static_cast<unsigned char>(std::clamp(255.99 * pixels[i][0], 0.0, 255.0));
        imageData[i * 3 + 1] = static_cast<unsigned char>(std::clamp(255.99 * pixels[i][1], 0.0, 255.0));
        imageData[i * 3 + 2] = static_cast<unsigned char>(std::clamp(255.99 * pixels[i][2], 0.0, 255.0));
#endif // DEBUG
        
    }
}

void draw(const std::vector<vec3>& pixels, std::vector<unsigned char>& imageData) {
    int numThreads = std::thread::hardware_concurrency();  // 获取可用线程数
    int chunkSize = (g_width * g_height + numThreads - 1) / numThreads;

    // 创建线程池
    ThreadPool threadPool(numThreads);

    // 使用线程池处理像素数据转换
    for (int t = 0; t < numThreads; ++t) {
        // 计算每个线程处理的像素范围
        int startIndex = t * chunkSize;
        int endIndex = std::min(startIndex + chunkSize, g_width * g_height);
        if (startIndex >= endIndex) break;

        // 将任务提交到线程池
        threadPool.submit([startIndex, endIndex, &pixels, &imageData] {
            for (int i = startIndex; i < endIndex; ++i) {
                int x = i % g_width;
                int y = i / g_width;

#define DEBUG 1
#ifdef DEBUG
                imageData[i * 3 + 0] = static_cast<unsigned char>(std::clamp((0.5 + x * 1.0 / (double)g_width) * 255.0, 0.0, 255.0));
                imageData[i * 3 + 1] = static_cast<unsigned char>(std::clamp((0.7 + x * 1.0 / (double)g_width) * 255.0, 0.0, 255.0));
                imageData[i * 3 + 2] = static_cast<unsigned char>(std::clamp((1.0 + x * 1.0 / (double)g_width) * 255.0, 0.0, 255.0));
#else
                // 将像素值转换为 unsigned char，并进行范围限制（0 到 255）
                imageData[i * 3 + 0] = static_cast<unsigned char>(std::clamp(255.99 * pixels[i][0], 0.0, 255.0));
                imageData[i * 3 + 1] = static_cast<unsigned char>(std::clamp(255.99 * pixels[i][1], 0.0, 255.0));
                imageData[i * 3 + 2] = static_cast<unsigned char>(std::clamp(255.99 * pixels[i][2], 0.0, 255.0));
#endif // DEBUG

            }
        });
    }

    // 等待所有任务完成
    threadPool.waitForCompletion();

    // 保存图片
    if (stbi_write_png("output.png", g_width, g_height, 3, imageData.data(), g_width * 3)) {
        std::cout << "Image saved successfully as output.png" << std::endl;
    }
    else {
        std::cerr << "Failed to save image" << std::endl;
    }

    // 关闭线程池
    threadPool.shutdown();
}


int main() {
    // 使用 std::vector<vec3> 来模拟像素数据
    std::vector<vec3> pixels(g_height * g_width);

    // 使用 std::vector<unsigned char> 动态分配字节数组
    std::vector<unsigned char> imageData(g_width * g_height * 3);

	draw(pixels, imageData);

    return 0;
}
