#include "comptabledelegate.h"

#include <QPainter>
#include <QBrush>

CompTableDelegate::CompTableDelegate()
{

}

void CompTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QBrush brush(Qt::GlobalColor::cyan);
    painter->setBrush(brush);
    painter->drawRect(option.rect);

    QStyledItemDelegate::paint(painter, option, index);
}

QSize CompTableDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  [[maybe_unused]]const QModelIndex &index) const
{
    return QSize(option.rect.width(), option.rect.height());
}
