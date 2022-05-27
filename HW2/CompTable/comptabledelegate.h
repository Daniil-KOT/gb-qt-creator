#ifndef COMPTABLEDELEGATE_H
#define COMPTABLEDELEGATE_H

#include <QStyledItemDelegate>

class CompTableDelegate : public QStyledItemDelegate
{
public:
    CompTableDelegate();
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

#endif // COMPTABLEDELEGATE_H
