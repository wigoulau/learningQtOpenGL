#include "widget.h"
#include <iostream>
using namespace std;

const char *vertexShaderCode = "layout (location = 0) in vec4 vPosition; \
        out vec4 vColor; \
        void main() {  \
             gl_Position  = vPosition; \
             gl_PointSize = 10.0; \
             vColor = vec4(1.0, 0.5, 0.0, 1.0); \
        };";

const char *fragmentShaderCode = "precision mediump float; \
        in vec4 vColor; \
        void main() {  \
             gl_FragColor = vColor; \
        };";


Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    //this->resize(400, 300);
}

Widget::~Widget()
{
}

int Widget::compileVertexShader(const char *vertextCode)
{
    return compileShader(GL_VERTEX_SHADER, vertextCode);
}

int Widget::compileFragmentShader(const char *fragmentCode)
{
    return compileShader(GL_FRAGMENT_SHADER, fragmentCode);
}

int Widget::compileShader(int type, const char* sourceCode)
{
    int shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &sourceCode, NULL);
    glCompileShader(shaderId);

    int success = 0;
    char infoLog[1024] = "";
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
        cout << "compile shader error:" << infoLog << endl;
        fflush(stdout);
        return 0;
    }

    return shaderId;
}

int Widget::linkProgram(int vertextShaderId, int fragmentShaderId)
{
    int programId = glCreateProgram();
    glAttachShader(programId, vertextShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

    int success = 0;
    char infoLog[1024] = "";
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 1024, NULL, infoLog);
        cout << "link program error:" << infoLog << endl;
        fflush(stdout);
        return 0;
    }

    return programId;
}


void Widget::initShader()
{
    int vertextShaderId = compileVertexShader(vertexShaderCode);
    int fragmengShaderId = compileFragmentShader(fragmentShaderCode);
    mProgramId = linkProgram(vertextShaderId, fragmengShaderId);
    qDebug("mProgramId is %d", mProgramId);
    glDeleteShader(vertextShaderId);
    glDeleteShader(fragmengShaderId);
}

void Widget::drawTriangle()
{
    GLfloat vertices[] = {
        0.25f, 0.25f, 0.0f,  //V0
        -0.75f, 0.25f, 0.0f, //V1
        -0.75f, -0.75f, 0.0f, //V2
        0.25f, -0.75f, 0.0f, //V3

        0.75f, -0.25f, 0.0f, //V4
        0.75f, 0.75f, 0.0f, //V5
        -0.25f, 0.75f, 0.0f, //V6
        -0.25f, -0.25f, 0.0f, //V7

        -0.25f, 0.75f, 0.0f, //V6
        -0.75f, 0.25f, 0.0f, //V1

        0.75f, 0.75f, 0.0f, //V5
        0.25f, 0.25f, 0.0f, //V0

        -0.25f, -0.25f, 0.0f, //V7
        -0.75f, -0.75f, 0.0f, //V2

        0.75f, -0.25f, 0.0f, //V4
        0.25f, -0.75f, 0.0f //V3
    };
    glUseProgram(mProgramId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 2, 3);

}
//https://blog.csdn.net/weixin_44729066/article/details/122775941
//https://blog.csdn.net/bendanfly/article/details/124034475
void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initShader();
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    drawTriangle();
}

