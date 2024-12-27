#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "vector.hpp"
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
    // 多线程设置
    int numThreads = std::thread::hardware_concurrency();  // 获取可用线程数
	// 计算每个线程处理的像素数量
    int chunkSize = (g_width * g_height + numThreads - 1) / numThreads;

    std::vector<std::future<void>> futures;

    // 使用多个线程处理像素数据转换
    for (int t = 0; t < numThreads; ++t) {
		// 计算每个线程处理的像素范围
        int startIndex = t * chunkSize;
        int endIndex = std::min(startIndex + chunkSize, g_width * g_height);
        if (startIndex >= endIndex) break;

        futures.push_back(std::async(
            std::launch::async,
            convert_to_byte_array,
            //将 pixels 作为常量引用传递给 convert_to_byte_array，因为它不会被修改。
            std::cref(pixels),
            //将 imageData 作为非const引用传递，因为它会被修改。
            std::ref(imageData),
            startIndex,
            endIndex
        ));
    }

    // 等待所有线程完成
    for (auto& future : futures) {
        future.get();
    }

    // 使用 stb_image_write 输出为 PNG 文件
    if (stbi_write_png("output.png", g_width, g_height, 3, imageData.data(), g_width * 3)) {
        std::cout << "Image saved successfully as output.png" << std::endl;
    }
    else {
        std::cerr << "Failed to save image" << std::endl;
    }
}

int main() {
    // 使用 std::vector<vec3> 来模拟像素数据
    std::vector<vec3> pixels(g_height * g_width);

    // 使用 std::vector<unsigned char> 动态分配字节数组
    std::vector<unsigned char> imageData(g_width * g_height * 3);

	draw(pixels, imageData);

    return 0;
}
