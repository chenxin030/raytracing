#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "vector.hpp"
#include <vector>
#include <iostream>

#include <thread>
#include <future>
#include <algorithm>  // For std::clamp

// ȫ�ֱ�����ͼ���Ⱥ͸߶�
const int g_width = 800;  // ���磬��� 800
const int g_height = 600; // ���磬�߶� 600

// ��������������ת��Ϊ unsigned char
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
        // ������ֵת��Ϊ unsigned char�������з�Χ���ƣ�0 �� 255��
        imageData[i * 3 + 0] = static_cast<unsigned char>(std::clamp(255.99 * pixels[i][0], 0.0, 255.0));
        imageData[i * 3 + 1] = static_cast<unsigned char>(std::clamp(255.99 * pixels[i][1], 0.0, 255.0));
        imageData[i * 3 + 2] = static_cast<unsigned char>(std::clamp(255.99 * pixels[i][2], 0.0, 255.0));
#endif // DEBUG
        
    }
}

void draw(const std::vector<vec3>& pixels, std::vector<unsigned char>& imageData) {
    // ���߳�����
    int numThreads = std::thread::hardware_concurrency();  // ��ȡ�����߳���
	// ����ÿ���̴߳������������
    int chunkSize = (g_width * g_height + numThreads - 1) / numThreads;

    std::vector<std::future<void>> futures;

    // ʹ�ö���̴߳�����������ת��
    for (int t = 0; t < numThreads; ++t) {
		// ����ÿ���̴߳�������ط�Χ
        int startIndex = t * chunkSize;
        int endIndex = std::min(startIndex + chunkSize, g_width * g_height);
        if (startIndex >= endIndex) break;

        futures.push_back(std::async(
            std::launch::async,
            convert_to_byte_array,
            //�� pixels ��Ϊ�������ô��ݸ� convert_to_byte_array����Ϊ�����ᱻ�޸ġ�
            std::cref(pixels),
            //�� imageData ��Ϊ��const���ô��ݣ���Ϊ���ᱻ�޸ġ�
            std::ref(imageData),
            startIndex,
            endIndex
        ));
    }

    // �ȴ������߳����
    for (auto& future : futures) {
        future.get();
    }

    // ʹ�� stb_image_write ���Ϊ PNG �ļ�
    if (stbi_write_png("output.png", g_width, g_height, 3, imageData.data(), g_width * 3)) {
        std::cout << "Image saved successfully as output.png" << std::endl;
    }
    else {
        std::cerr << "Failed to save image" << std::endl;
    }
}

int main() {
    // ʹ�� std::vector<vec3> ��ģ����������
    std::vector<vec3> pixels(g_height * g_width);

    // ʹ�� std::vector<unsigned char> ��̬�����ֽ�����
    std::vector<unsigned char> imageData(g_width * g_height * 3);

	draw(pixels, imageData);

    return 0;
}
