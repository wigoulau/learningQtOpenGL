# Qt OpenGL Hello World
## QtCreator创建新工程
选择Application -- Qt Widgets Application，然后填写工程名称和选择目录
在Class Information中，Base class选择QWidget，取消勾选Generate form

## 工程文件HelloWorld.pro中添加OpenGL支持
```C++
QT += opengl
win32:LIBS += -lopengl32 -lglu32
unix:LIBS += -lglut -lGLU
```
## 头文件widget.h中，将类Widget继承GLWidget，并重写OpenGL的3个函数
```c++
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
```
## widget.cpp中实现opengl重写函数
* void initializeGL()窗口启动时会调用此初始化函数，在函数中可以调用
```
// 设置背景色为黑色，不透明
glClearColor(1.0, 1.0, 1.0, 1.0);
```
其中参数分别对应r,g,b,a归一化的值，取值范围为0~1，即对应0~255

* resizeGL(int w, int h)为改变窗口大小是会调用
在此可以设置OpenGL的渲染大小为窗口大小
```
glViewport(0, 0, this->width(), this->height());
```
* paintGL()渲染函数,在此可以绘制对应的图像

* opengl绘图基本类型
```
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON 0x0009
```
