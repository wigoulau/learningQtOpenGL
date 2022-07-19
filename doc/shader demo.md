# shader 介绍和实践
## shader介绍
shader采用GLSL语言编写，类似C的语法，运行在GPU中，包含顶点着色器(Vertex shader)和片段着色器(Fragment shader).
### 顶点着色器(Vertex shader)
### 片段着色器(Fragment shader)
### 操作流程
创建程序 --> 编译顶点着色器 --> 编译片段着色器 --> 链接程序 --> 检测错误

## 实践
* 创建工程
参考[HelloWorld](./HelloWorld.md)，创建基本工程
* 顶点着色器程序
```c++
const char *vertexShaderCode = "layout (location = 0) in    vec4 vPosition; \
        out vec4 vColor; \
        void main() {  \
             gl_Position  = vPosition; \
             gl_PointSize = 10.0; \
             vColor = vec4(1.0, 0.5, 0.0, 1.0); \
        };";
```
vPosition为输入参数，绘图顶点位置
gl_Position为默认输出函数，表示绘图的顶点位置，也可修改gl_Position  = vPosition;的运算，来改变顶点位置，如gl_Position  = vPosition + 0.5;x,y位置会往右上移动0.5距离
vColor为输出参数，设置绘图的颜色
* 片段着色器程序
```C++
const char *fragmentShaderCode = "precision mediump float; \
        in vec4 vColor; \
        void main() {  \
             gl_FragColor = vColor; \
        };";
```
vColor为输入参数，绘图颜色。由顶点着色器程序传过来，在此变量名必须要一致。

* 编译着色器程序
主要调用下面几个API，gl开头的API都是OpenGL规范定义的API，各系统平台都支持，QT中需要void QOpenGLFunctions::glDeleteShader(GLuint shader)， 并继承QOpenGLExtraFunctions。同时在初始化时调用initializeOpenGLFunctions()
```c
// 创建着色器，type为GL_VERTEX_SHADER或GL_FRAGMENT_SHADER
GLuint glCreateShader(GLenum type)
// 加载着色器程序
void glShaderSource(GLuint shader, GLsizei count, const char** string, const GLint* length)
// 编译着色器程序
void glCompileShader(GLuint shader)
// 检查程序是否错误
void glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
// 获取着色器错误信息
void glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
// 删除着色器程序，链接后可以删除
void glDeleteShader(GLuint shader)
```

* 链接程序
主要调用下面几个API：
```c
// 创建程序
GLuint glCreateProgram()
// 加载着色器
void glAttachShader(GLuint program, GLuint shader)
// 链接程序
void glLinkProgram(GLuint program)
// 检查链接程序是否错误
void glGetProgramiv(GLuint program, GLenum pname, GLint* params)
// 获取链接错误信息
void glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
```

* 绘图
在paintGL()中调用
```c++
    // 绘图顶点位置
    GLfloat vertices[] = {
        0.25f, 0.25f, 0.0f,  //V0
        -0.75f, 0.25f, 0.0f, //V1
        -0.75f, -0.75f, 0.0f, //V2
        0.25f, -0.75f, 0.0f, //V3

        0.75f, -0.25f, 0.0f, //V4
        0.75f, 0.75f, 0.0f, //V5
        -0.25f, 0.75f, 0.0f, //V6
        -0.25f, -0.25f, 0.0f, //V7
    }
    // 设置背景颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 清楚上一次的绘图Buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // 使用前面创建的program
    glUseProgram(mProgramId);
    // 设置顶点参数，绑定顶点着色器的location = 0的变量值为vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    // 画三角形，三个顶点为：vertices[0:2]
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // 画三角形，三个顶点为：vertices[2:4]
    glDrawArrays(GL_TRIANGLES, 2, 3);
```

