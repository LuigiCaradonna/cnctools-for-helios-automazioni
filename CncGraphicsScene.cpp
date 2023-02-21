#include "CncGraphicsScene.h"

void CncGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    // Mouse pointer position over the scene
    QPointF pos = event->scenePos();

    // Emit the signal providing the mouse position as parameter
    emit signalMousePos(pos);
}

void CncGraphicsScene::keyPressEvent(QKeyEvent* event)
{
    // Emit the signal providing the key code
    emit signalKeyPressed(event->key());
}