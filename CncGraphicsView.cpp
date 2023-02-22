#include "CncGraphicsView.h"

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

/********** CONSTRUCTOR **********/

CncGraphicsView::CncGraphicsView(QGraphicsScene* scene, QWidget* parent) :
    QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setDragMode(QGraphicsView::ScrollHandDrag);

    setMaxSize();
    centerOn(0, 0);

    zoomDelta = 0.1;
    panSpeed = 4;
    _doMousePanning = false;
    _doKeyZoom = false;
    _scale = 1.0;

    setScene(scene);

    panButton = Qt::MiddleButton;
    zoomKey = Qt::Key_Z;
}


/********** PROTECTED FUNCTIONS **********/

qreal CncGraphicsView::getScale() const
{
    return _scale;
}

void CncGraphicsView::keyPressEvent(QKeyEvent* event)
{
    qint32 key = event->key();

    if (key == zoomKey) {
        _doKeyZoom = true;
    }

    if (_doKeyZoom) {
        if (key == Qt::Key_Up)
            zoomIn();

        else if (key == Qt::Key_Down)
            zoomOut();
    }

    else {
        QGraphicsView::keyPressEvent(event);
    }
}

void CncGraphicsView::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == zoomKey) {
        _doKeyZoom = false;
    }

    QGraphicsView::keyReleaseEvent(event);
}

void CncGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (_doMousePanning) {
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(_lastMousePos);
        pan(mouseDelta);
    }

    QGraphicsView::mouseMoveEvent(event);
    _lastMousePos = event->pos();
}

void CncGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == panButton) {
        _lastMousePos = event->pos();
        _doMousePanning = true;
    }

    QGraphicsView::mousePressEvent(event);
}

void CncGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == panButton) {
        _doMousePanning = false;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void CncGraphicsView::wheelEvent(QWheelEvent* event)
{
    QPoint scrollAmount = event->angleDelta();

    // Apply zoom.
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void CncGraphicsView::zoom(float scaleFactor)
{
    scale(scaleFactor, scaleFactor);
    _scale *= scaleFactor;
}

void CncGraphicsView::zoomIn()
{
    zoom(1 + zoomDelta);
}

void CncGraphicsView::zoomOut()
{
    zoom(1 - zoomDelta);
}

void CncGraphicsView::pan(QPointF delta)
{
    // Scale the pan amount by the current zoom.
    delta *= _scale;
    delta *= panSpeed;

    // Have panning be anchored from the mouse.
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(), VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // For zooming to anchor from the view center.
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}


/********** PRIVATE FUNCTIONS **********/

void CncGraphicsView::setMaxSize()
{
    setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
}
