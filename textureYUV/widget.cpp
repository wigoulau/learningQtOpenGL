#include "widget.h"
#include <QGLWidget>
#include <iostream>
using namespace std;

const char *vertexShaderCode = "    \
        layout (location = 0) in vec4 vPosition; \
        layout(location = 1) in vec2 vTexcoord; \
        out vec2 Texcoord;  \
        void main() {  \
             gl_Position  = vPosition; \
             gl_PointSize = 10.0; \
             Texcoord = vTexcoord; \
        };";

const char *fragmentShaderCode = "  \
        in vec2 Texcoord;   \
        uniform sampler2D tex_y; \
        uniform sampler2D tex_u; \
        uniform sampler2D tex_v; \
        void main() {  \
            vec3 yuv;   \
            vec3 rgb;   \
            yuv.x = texture2D(tex_y, Texcoord).r;   \
            yuv.y = texture2D(tex_u, Texcoord).r - 0.5; \
            yuv.z = texture2D(tex_v, Texcoord).r - 0.5; \
            rgb = mat3(1,       1,          1,  \
                       0,       -0.39465,   2.03211,    \
                       1.13983, -0.58060,   0) * yuv;   \
            gl_FragColor = vec4(rgb, 1); \
        };";


Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    width = 400;
    height = 300;
    this->resize(width, height);
    pYuvBuf = new unsigned char[width * height * 3 / 2];
    pPlane[0] = pYuvBuf;
    pPlane[1] = pPlane[0] + width * height;
    pPlane[2] = pPlane[1] + width * height / 4;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            pYuvBuf[i * height + j] = (i * height + j) % 255;
        }
    }
}

Widget::~Widget()
{
    if (pYuvBuf) {
        delete pYuvBuf;
        pYuvBuf = NULL;
    }
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
    // 创建一个容纳shader的容器
    int shaderId = glCreateShader(type);
    // shader容器中添加shader的源代码
    glShaderSource(shaderId, 1, &sourceCode, NULL);
    // 对shader容器中的源代码进行编译
    glCompileShader(shaderId);

    int success = 0;
    char infoLog[1024] = "";
    // 获取编译和链接的信息
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
    // 创建程序容器
    int programId = glCreateProgram();
    // 将shader容器添加到程序中
    glAttachShader(programId, vertextShaderId);
    glAttachShader(programId, fragmentShaderId);
    // 链接程序
    glLinkProgram(programId);

    int success = 0;
    char infoLog[1024] = "";
    // 获取编译和链接的信息
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 1024, NULL, infoLog);
        cout << "link program error:" << infoLog << endl;
        fflush(stdout);
        return 0;
    }

    return programId;
}

#define ATTRIB_VERTEX   0
#define ATTRIB_TEXTURE  1
void Widget::initShader()
{
    int vertextShaderId = compileVertexShader(vertexShaderCode);
    int fragmengShaderId = compileFragmentShader(fragmentShaderCode);
    mProgramId = linkProgram(vertextShaderId, fragmengShaderId);
    qDebug("mProgramId is %d", mProgramId);
    // 使用程序
    glUseProgram(mProgramId);

    glBindAttribLocation(mProgramId, ATTRIB_VERTEX, "vPosition");
    glBindAttribLocation(mProgramId, ATTRIB_TEXTURE, "vTexcoord");

    // 获取Uniform变量指针
    textureUniformY = glGetUniformLocation(mProgramId, "tex_y");
    textureUniformU = glGetUniformLocation(mProgramId, "tex_u");
    textureUniformV = glGetUniformLocation(mProgramId, "tex_v");

#ifdef TEXTURE_ROTATE
    static const GLfloat vertexVertices[] = {
        -1.0f, -0.5f,
         0.5f, -1.0f,
        -0.5f,  1.0f,
         1.0f,  0.5f,
    };
#else
    static const GLfloat vertexVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f,
    };
#endif

#ifdef TEXTURE_HALF
    static const GLfloat textureVertices[] = {
        0.0f,  1.0f,
        0.5f,  1.0f,
        0.0f,  0.0f,
        0.5f,  0.0f,
    };
#else
    static const GLfloat textureVertices[] = {
        0.0f,  1.0f,
        1.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
    };
#endif

    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
    glEnableVertexAttribArray(ATTRIB_VERTEX);

    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);

    // 初始化Texture
    glGenTextures(1, &texIdY);
    glBindTexture(GL_TEXTURE_2D, texIdY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &texIdU);
    glBindTexture(GL_TEXTURE_2D, texIdU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &texIdV);
    glBindTexture(GL_TEXTURE_2D, texIdV);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glDeleteShader(vertextShaderId);
    glDeleteShader(fragmengShaderId);
}

void Widget::displayYUV()
{
    // 清除缓冲区图像
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Y
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIdY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pPlane[0]);
    glUniform1i(textureUniformY, 0);
    // U
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texIdU);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, pPlane[1]);
    glUniform1i(textureUniformU, 1);
    // V
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texIdV);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, pPlane[2]);
    glUniform1i(textureUniformV, 2);

    // 绘制
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initShader();
}

void Widget::resizeGL(int w, int h)
{
    qDebug("%s: w=%d, h=%d", __func__, w, h);
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    qDebug("%s  %d", __func__, __LINE__);
    displayYUV();
}

