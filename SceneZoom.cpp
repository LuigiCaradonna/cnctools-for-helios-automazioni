#include "SceneZoom.h"
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>

SceneZoom::SceneZoom(QGraphicsView* view)
    : QObject(view), _view(view)
{
    this->_view->viewport()->installEventFilter(this);
    this->_view->setMouseTracking(true);
    this->_modifiers = Qt::ControlModifier;
    this->_zoom_factor_base = 1.0015;
    this->_zoom_level = 0;
}

void SceneZoom::zoom(double factor)
{
    this->_view->scale(factor, factor);
    this->_view->centerOn(_target_scene_pos);

    QPointF delta_viewport_pos =
        this->_target_viewport_pos -
        QPointF(this->_view->viewport()->width() / 2.0, this->_view->viewport()->height() / 2.0);

    QPointF viewport_center = this->_view->mapFromScene(this->_target_scene_pos) - delta_viewport_pos;

    this->_view->centerOn(this->_view->mapToScene(viewport_center.toPoint()));

    emit zoomed();
}

void SceneZoom::setModifiers(Qt::KeyboardModifiers modifiers)
{
    this->_modifiers = modifiers;
}

void SceneZoom::setZoomFactorBase(double value)
{
    this->_zoom_factor_base = value;
}

int SceneZoom::getZoomLevel()
{
    return this->_zoom_level;
}

bool SceneZoom::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        QPointF delta = this->_target_viewport_pos - mouse_event->pos();

        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            this->_target_viewport_pos = mouse_event->pos();
            this->_target_scene_pos = this->_view->mapToScene(mouse_event->pos());
        }
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);

        if (QApplication::keyboardModifiers() == this->_modifiers)
        {
            if (wheel_event->angleDelta().y() != 0)
            {
                double angle = wheel_event->angleDelta().y();
                double factor = qPow(this->_zoom_factor_base, angle);

                if (wheel_event->angleDelta().y() > 0)
                    this->_zoom_level++;
                else
                    this->_zoom_level--;

                zoom(factor);

                return true;
            }
        }
    }

    Q_UNUSED(object)
        return false;
}
