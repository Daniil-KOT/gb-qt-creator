#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <QGraphicsItem>
#include <QBrush>

class BaseShape : public QObject, public QGraphicsItem
{
    Q_OBJECT       
public:
    explicit BaseShape(QObject *parent = nullptr);
    BaseShape(QObject *parent = nullptr, int shapeType = 0, QPolygon shape = QPolygon());
    QRectF boundingRect() const override;
    bool containsPoint(const QPoint& point);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

signals:
    void redraw();

private:
    enum ShapeTypes
    {
        Rectangle = 0,
        Ellipse = 1,
        Star = 2
    };

    ShapeTypes shapeType_;
    QPolygon shape_;
    QBrush brush_;
    QPoint currentPos_;
    bool isMoving_ = false;
    bool isRotating_ = false;
};

#endif // SHAPE_H
