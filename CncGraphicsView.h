#pragma once

#include <QtWidgets/QGraphicsView>
#include <qevent.h>
#include <QWheelEvent>
#include <QKeyEvent>

class CncGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CncGraphicsView(QGraphicsScene* scene);

    qreal panSpeed;
    qreal zoomDelta;
    qreal zoomKey;
    Qt::MouseButton panButton;

    void pan(QPointF delta);
    void zoom(float scaleFactor);
    void zoomIn();
    void zoomOut();

    qreal getScale() const;

protected:
    float m_originX, m_originY;

    /*
     * Override of the method mousePressEvent to emit a custom signal.
     */
    void mousePressEvent(QMouseEvent* event) override;

    /*
     * Override of the method mouseMoveEvent to emit a custom signal.
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;

    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

private:
    // Flags to determine if zooming or panning should be done.
    bool _doMousePanning;
    bool _doKeyZoom;

    QPoint _lastMousePos;
    qreal _scale;

    void setMaxSize();
};

