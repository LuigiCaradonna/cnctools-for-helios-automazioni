#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <qevent.h>

class CncGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

protected:
    /*
     * Override of the method mouseMoveEvent to emit a custom signal.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    /*
     * Override of the method keyPressEvent to emit a custom signal.
     */
    void keyPressEvent(QKeyEvent* event) override;

signals:
    /*
     * Signal emitted when the mouse moves over the scene.
     */
    void signalMousePos(QPointF pos);

    /*
     * Signal emitted when a key on the keyboard is pressd while the QGraphicsScene is active.
     */
    void signalKeyPressed(int key);
};