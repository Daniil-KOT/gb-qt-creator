#ifndef BASICSHAPES_H
#define BASICSHAPES_H

#include <QWidget>
#include <QMultiMap>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <baseshape.h>

class BasicShapes : public QGraphicsView
{
    Q_OBJECT

public:
    BasicShapes(QWidget *parent = nullptr);
    ~BasicShapes();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPolygon makeStar();
    void draw(QPainter& painter);
    QGraphicsItem* findShape();
    void deleteShape();

private slots:
    void redraw();

private:
    QPointF currentPos_;
    QList<BaseShape*> shapes_;
    QGraphicsItem* currentShape_ = nullptr;
    QGraphicsScene* scene_ = nullptr;

    enum ShapeTypes
    {
        Rectangle = 0,
        Ellipse = 1,
        Star = 2
    };

    int shapeType_;
};
#endif // BASICSHAPES_H
