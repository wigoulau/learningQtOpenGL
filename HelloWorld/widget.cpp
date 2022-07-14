#include "widget.h"

Widget::Widget(QWidget *parent)
    : QGLWidget(parent)
{
    this->resize(320, 320);
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    qDebug("%s %d", __func__, __LINE__);
    // 设置背景色为黑色，不透明
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void Widget::resizeGL(int w, int h)
{
    qDebug("%s %d: w = %d, h = %d", __func__, __LINE__, w, h);
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    qDebug("%s %d", __func__, __LINE__);
    // 清楚缓存
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(2.0);
    // 绘制坐标范围为(-1, 1),窗口中间为(0,0)坐标，不是系统的窗口坐标，这里需要用OpenGL坐标
    //  画2个点(0, 0), (0, 0.5)
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 0.0);   // 红色
    glVertex2f(0, 0.5);
    glVertex2f(0, 0);
    glEnd();

    // 画线
    // GL_LINES每条线取2个点，依次下去(v0, v1), (v2, v3)
    // GL_LINE_LOOP,按顺序连接所有的点，而且连接首尾点，形成一个闭环(v0, v1, v2, v3, v0)
    // GL_LINE_STRIP,按顺序连接所有的点，不连首尾点(v0, v1, v2, v3)
    glBegin(GL_LINES);
    glColor3f(0, 0, 1.0);   // 蓝色
    glVertex2f(-0.5, 0);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, -0.5);
    glEnd();

    // 绘制三角形
    // GL_TRIANGLES,取3个点（v0，v1，v2）、（v3，v4，v5）
    // GL_TRIANGLE_STRIP， 循环取3个点（v0，v1，v2）、（v1，v2，v3）、（v2，v3，v4）
    // GL_TRIANGLE_FAN，绘制各三角形，形成一个扇形序列，以V0未起始点，（v0，v1，v2）、（v0，v2，v3）、（v0，v3，v4）
    glBegin(GL_TRIANGLES);
    glColor3f(0, 1.0, 0);   // 绿色
    glVertex2f(0.2, 0.5);
    glVertex2f(0, 0.3);
    glVertex2f(0, -0.3);
    glVertex2f(0.5, -0.3);
    glVertex2f(0.5, 0.5);
    glEnd();

    // 绘制四边形
    // GL_QUADS，取4个点(v0, v1, v2, v3)
    // GL_QUAD_STRIP
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 0);
    glVertex2f(-0.6, 0.1);
    glVertex2f(-0.6, 0.5);
    glVertex2f(-0.9, 0.5);
    glVertex2f(-0.9, -0.3);
    glVertex2f(-0.6, -0.5);
    glEnd();

    // GL_POLYGON多边形，连接所有点，而且连接首尾点

    glFlush();

}
