# OpenGL显示YUV
## 初始化Shader
初始化Shader的步骤比较多，主要可以分为3步：创建Shader，创建Program，初始化Texture。
### 创建一个Shader对象
* 编写Vertex Shader和Fragment Shader源码。
* 创建两个shader 实例：glCreateShader
* 给Shader实例指定源码：glShaderSource
* 在线编译shaer源码：glCompileShader
* 检查编译结果：glGetShaderiv
### 创建一个Program对象
* 创建program：glCreateProgram
* 绑定shader到program：glAttachShader
* 链接program：glLinkProgram
* 检查链接结果：glGetProgramiv
* 使用porgram：glUseProgram
### 初始化Texture。可以分为以下步骤。
* 定义顶点数组
* 设置纹理顶点数组
* 初始化纹理
    * 获取Uniform变量指针：glGetUniformLocation
    * 创建纹理ID：glGenTextures
    * 绑定纹理：glBindTexture
    * 设置纹理参数：glTexParameteri，包含4个参数：GL_TEXTURE_MAG_FILTER、GL_TEXTURE_MIN_FILTER、GL_TEXTURE_WRAP_S、GL_TEXTURE_WRAP_T

## 循环显示YUV
* 设置纹理
    * 激活纹理：glActiveTexture
    * 绑定纹理：glBindTexture
    * 设置纹理像素数据：glTexImage2D
    * 设置纹理采样器对应的纹理单元：glUniform1i
* 绘制
glDrawArrays，这里使用三角形图元：GL_TRIANGLE_STRIP
* 显示

## 参考
[OpenGL播放YUV420P](https://blog.csdn.net/leixiaohua1020/article/details/40379845)