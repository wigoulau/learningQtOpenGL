#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGLWidget>

class Widget : public QGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
#endif // WIDGET_H
