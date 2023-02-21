#include "CncGraphicsView.h"

CncGraphicsView::CncGraphicsView(QGraphicsScene* scene)
{
    this->m_originX = 0.f;
    this->m_originY = 0.f;
}

void CncGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton)
    {
        // Store original position.
        m_originX = event->x();
        m_originY = event->y();
    }
}

void CncGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::MiddleButton)
    {
        QPointF oldP = mapToScene(m_originX, m_originY);
        QPointF newP = mapToScene(event->pos());
        QPointF translation = newP - oldP;

        translate(translation.x(), translation.y());

        m_originX = event->x();
        m_originY = event->y();
    }
}