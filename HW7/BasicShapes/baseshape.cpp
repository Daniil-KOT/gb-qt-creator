#include "baseshape.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

BaseShape::BaseShape(QObject *parent)
    : QObject{parent},
      QGraphicsItem()
{

}

BaseShape::BaseShape(QObject *parent, int shapeType, QPolygon shape)
    : QObject(parent),
      QGraphicsItem(),
      shape_(shape)
{
    srand(clock());

    switch(shapeType)
    {
    case ShapeTypes::Rectangle:
        shapeType_ = ShapeTypes::Rectangle;
        break;

    case ShapeTypes::Ellipse:
        shapeType_ = ShapeTypes::Ellipse;
        break;

    case ShapeTypes::Star:
        shapeType_ = ShapeTypes::Star;
        break;
    }

    brush_.setColor(QColor(rand() % 256, rand() % 256, rand() % 256));
    brush_.setStyle(Qt::BrushStyle::SolidPattern);
}

void BaseShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(brush_);
    switch(shapeType_)
    {
    case ShapeTypes::Rectangle:
        painter->drawRect(shape_.boundingRect());
        break;

    case ShapeTypes::Ellipse:
        painter->drawEllipse(shape_.boundingRect());
        break;

    case ShapeTypes::Star:
        painter->drawPolygon(shape_);
        break;
    }

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF BaseShape::boundingRect() const
{
    return shape_.boundingRect();
}

bool BaseShape::containsPoint(const QPoint& point)
{
    return shape_.containsPoint(point, Qt::FillRule::OddEvenFill);
}

void BaseShape::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (Qt::MouseButton::LeftButton == event->button())
    {
        mouseLeftBtnPressed_ = true;
        currentPos_ = event->pos().toPoint();
    }
    else if (Qt::MouseButton::RightButton == event->button())
    {
    }
    else if (Qt::MouseButton::LeftButton == event->button())
    {
        isMoving_ = true;
        return;
    }
}

void BaseShape::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    mouseLeftBtnPressed_ = false;
    isMoving_ = false;
}

void BaseShape::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isMoving_)
    {
        QPoint delta = event->pos().toPoint() - currentPos_;
        for (int i = 0; i < shape_.size(); ++i)
        {
            shape_[i].setX(shape_[i].x() + delta.x());
            shape_[i].setY(shape_[i].y() + delta.y());
        }
        currentPos_ = event->pos().toPoint();
    }
}
