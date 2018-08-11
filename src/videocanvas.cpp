#include "videocanvas.h"

VideoCanvas::VideoCanvas(QWidget *parent) : QLabel(parent)
{

}

void VideoCanvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit mouseDoubleClick(event);
}

void VideoCanvas::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event);
}

void VideoCanvas::mousePressEvent(QMouseEvent *event)
{
    emit mousePressed(event);
}
