#include "basicshapes.h"

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainterPath>
#include <QtMath>

BasicShapes::BasicShapes(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setMinimumSize(960, 540);
    shapeType_ = ShapeTypes::Rectangle;
    scene_ = new QGraphicsScene(this);
    scene_->setSceneRect(0, 0, 1920, 1080);
    setScene(scene_);
    srand(clock());
}

BasicShapes::~BasicShapes()
{
}

void BasicShapes::mousePressEvent(QMouseEvent *event)
{
    currentPos_ = this->mapToScene(event->pos());
    currentShape_ = findShape();

    if (Qt::LeftButton == event->button() && !currentShape_)
    {
        BaseShape* shape;
        if (ShapeTypes::Star != shapeType_)
        {
            QPoint topLeft(currentPos_.toPoint().x(), currentPos_.toPoint().y());
            QPoint botRight(currentPos_.toPoint().x() + scene_->width() / (5 + rand() % 5), currentPos_.toPoint().y() + scene_->height() / (5 + rand() % 5));
            shape = new BaseShape(scene_, shapeType_, QPolygon(QRect(topLeft, botRight)));
            shapes_.append(shape);
            scene_->addItem(shapes_.last());
        }
        else
        {
            shape = new BaseShape(scene_, shapeType_, makeStar());
            shapes_.append(shape);
            scene_->addItem(shapes_.last());
        }
        connect(shape, &BaseShape::redraw, this, &BasicShapes::redraw);
    }
    else if (Qt::RightButton == event->button())
    {
        if (currentShape_)
        {
            deleteShape();
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void BasicShapes::mouseReleaseEvent(QMouseEvent *event)
{
    if (Qt::RightButton == event->button() || Qt::MiddleButton == event->button())
    {
        return;
    }

    if (ShapeTypes::Star != shapeType_ && !currentShape_)
    {
        ++shapeType_;
    }
    else if (!currentShape_)
    {
        shapeType_ = ShapeTypes::Rectangle;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void BasicShapes::wheelEvent(QWheelEvent *event)
{
    const qreal scaleFactor = pow(2.0, event->angleDelta().y() / 240.0);
    scale(scaleFactor, scaleFactor);
}

void BasicShapes::keyPressEvent(QKeyEvent *event)
{
    const qreal scaleFactor = 0.5;
    if (Qt::Key_Plus == event->key())
    {
        scale(scaleFactor + 1, scaleFactor + 1);
    }
    else if (Qt::Key_Minus == event->key())
    {
        scale(scaleFactor, scaleFactor);
    }
}

QPolygon BasicShapes::makeStar()
{
    QPolygon star;

    const int rays = 5;
    const double angle = 2 * 3.14 / rays;
    const int outerRadius = 40 + rand() % 100;
    const int innerRadius = outerRadius / 2;

    for (int i = 1; i <= rays; ++i)
    {
        star.append(currentPos_.toPoint() + QPoint(innerRadius * cos((i - 0.5) * angle),
                                         innerRadius * sin((i - 0.5) * angle)));

        star.append(currentPos_.toPoint() + QPoint(outerRadius * cos(i * angle),
                                         outerRadius * sin(i * angle)));
    }

    return star;
}

QGraphicsItem* BasicShapes::findShape()
{
    return scene_->itemAt(currentPos_, QTransform());
}

void BasicShapes::deleteShape()
{
    scene_->removeItem(currentShape_);
    currentShape_ = nullptr;
}

void BasicShapes::redraw()
{
    scene_->update();
    this->update();
}

