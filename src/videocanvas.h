#ifndef VIDEOCANVAS_H
#define VIDEOCANVAS_H

#include <QLabel>
#include <QWidget>
#include <QEvent>
#include <QMouseEvent>

class VideoCanvas : public QLabel
{
    Q_OBJECT
public:
    explicit VideoCanvas(QWidget *parent=nullptr);

    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void mouseDoubleClick(QMouseEvent *event);
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);

public slots:

};

#endif // VIDEOCANVAS_H
