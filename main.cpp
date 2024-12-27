#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

const int g_width = 800;
const int g_height = 600;



int main()
{
	unsigned char* pixels = new unsigned char[g_width * g_height * 3];
	for (int i = 0; i < g_width; ++i) {
		for (int j = 0; j < g_height; ++j) {
			pixels[(i + j * g_width) * 3 + 0] = 255.99 * double(i) / g_width;
			pixels[(i + j * g_width) * 3 + 1] = 255.99 * double(j) / g_height;
			pixels[(i + j * g_width) * 3 + 2] = 0.2;
		}
	}
	stbi_write_png("output.png", g_width, g_height, 3, pixels, g_width * 3);
}