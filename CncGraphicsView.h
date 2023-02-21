#pragma once

#include <QtWidgets/QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <qevent.h>

class CncGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CncGraphicsView(QGraphicsScene* scene);

protected:
    float m_originX, m_originY;

    /*
     * Override of the method mouseMoveEvent to emit a custom signal.
     */
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

};

