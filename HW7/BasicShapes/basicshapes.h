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
//    void mouseMoveEvent(QMouseEvent* event) override;
//    void paintEvent(QPaintEvent*) override;

private:
    QPolygon makeStar();
    void draw(QPainter& painter);
    int findShape();
    void deleteShape(int idx);

private:
    QPoint currentPos_;
    bool mouseLeftBtnPressed_ = false;
    bool isMoving_ = false;
    QList<BaseShape*> shapes_;
    BaseShape* currentShape_ = nullptr;
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
