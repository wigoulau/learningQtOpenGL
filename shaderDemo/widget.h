#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class Widget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void initShader();
    void drawTriangle();

    int compileShader(int type, const char *sourceCode);
    int compileVertexShader(const char *vertextCode);
    int compileFragmentShader(const char *fragmentCode);
    int linkProgram(int vertextShaderId, int fragmentShaderId);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    int mProgramId;
};
#endif // WIDGET_H
