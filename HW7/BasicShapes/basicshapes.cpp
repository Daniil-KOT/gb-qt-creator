#include "basicshapes.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QtMath>

BasicShapes::BasicShapes(QWidget *parent)
    : QGraphicsView(parent)
{
    shapeType_ = ShapeTypes::Rectangle;
    scene_ = new QGraphicsScene(this);
    scene_->setSceneRect(0, 0, 1920, 1000);
    setScene(scene_);
    srand(clock());
}

BasicShapes::~BasicShapes()
{
}

void BasicShapes::mousePressEvent(QMouseEvent *event)
{
    currentPos_ = event->pos();

    int idx = findShape();

    if (Qt::MouseButton::LeftButton == event->button() && !idx)
    {
        mouseLeftBtnPressed_ = true;

        if (ShapeTypes::Star != shapeType_)
        {
            QPoint topLeft(currentPos_.x(), currentPos_.y());
            QPoint botRight(currentPos_.x() + scene_->width() / (5 + rand() % 5), currentPos_.y() + scene_->height() / (5 + rand() % 5));

            shapes_.append(new BaseShape(nullptr, shapeType_, QPolygon(QRect(topLeft, botRight))));
            qDebug() << shapes_.last()->pos();
            scene_->addItem(shapes_.last());
        }
        else
        {
            shapes_.append(new BaseShape(nullptr, shapeType_, makeStar()));
            qDebug() << shapes_.last()->pos();
            scene_->addItem(shapes_.last());
        }
    }
    else if (Qt::MouseButton::RightButton == event->button())
    {
        if (idx)
        {
            deleteShape(idx);
        }
    }
    else if (Qt::MouseButton::LeftButton == event->button() && idx)
    {
        isMoving_ = true;
        return;
    }

    this->update();
}

void BasicShapes::mouseReleaseEvent(QMouseEvent *event)
{
    if (Qt::MouseButton::RightButton == event->button())
    {
        return;
    }

    if (ShapeTypes::Star != shapeType_ && !isMoving_)
    {
        ++shapeType_;
    }
    else if (!isMoving_)
    {
        shapeType_ = ShapeTypes::Rectangle;
    }

    mouseLeftBtnPressed_ = false;
    isMoving_ = false;
}

//void BasicShapes::mouseMoveEvent(QMouseEvent *event)
//{
//    if (isMoving_ && currentShape_)
//    {
//        QPoint tmp = event->pos() - currentPos_;

//        qreal dx = currentShape_->boundingRect().center().x() + static_cast<qreal>(tmp.x());
//        qreal dy = currentShape_->boundingRect().center().y() + static_cast<qreal>(tmp.y());
//        currentShape_->moveBy(dx, dy);

//        currentPos_ = event->pos();

//        this->update();
//    }
//}

QPolygon BasicShapes::makeStar()
{
    QPolygon star;

    const int rays = 5;
    const double angle = 2 * 3.14 / rays;
    const int outerRadius = 40 + rand() % 100;
    const int innerRadius = outerRadius / 2;

    for (int i = 1; i <= rays; ++i)
    {
        star.append(currentPos_ + QPoint(innerRadius * cos((i - 0.5) * angle),
                                         innerRadius * sin((i - 0.5) * angle)));

        star.append(currentPos_ + QPoint(outerRadius * cos(i * angle),
                                         outerRadius * sin(i * angle)));
    }

    return star;
}

int BasicShapes::findShape()
{
    for (int i = 0; i < shapes_.size(); ++i)
    {
        if (shapes_.at(i)->containsPoint(currentPos_))
        {
            currentShape_ = shapes_.at(i);
            // i + 1 because it can return i == 0
            // so the clause on line 28 will fail
            // even though it shouldn't
            return i + 1;
        }
    }

    return 0;
}

void BasicShapes::deleteShape(int idx)
{
    currentShape_ = nullptr;
    // idxs.second - 1 because we modified index on line 171
    scene_->removeItem(shapes_.at(idx - 1));
}

