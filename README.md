# 绘制png
```添加必要的宏
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
```
`stb_image_write`用的是C，需要另外添加`_CRT_SECURE_NO_DEPRECATE`和`_SCL_SECURE_NO_DEPRECATE`这两个宏。\
打开项目----项目属性—配置属性----C/C++ ----预处理器----预处理定义
（已经用cmake自动添加了）

```
# 针对 Visual Studio 目标，设置预处理宏
target_compile_definitions(raytracing PRIVATE
    _CRT_SECURE_NO_DEPRECATE
    _SCL_SECURE_NO_DEPRECATE
)
```