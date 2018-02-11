#ifndef POSTVIEWDELEGATE_H
#define POSTVIEWDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QApplication>
#include "mainwindow.h"
#include <QDebug>

class PostViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline PostViewDelegate(MainWindow *mainWindow) : QItemDelegate(mainWindow) {}

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
    {
        qDebug() << "test " << index.column();
        if (index.column() != 0) {
            QItemDelegate::paint(painter, option, index);
            return;
        }

        // Set up a QStyleOptionProgressBar to precisely mimic the
        // environment of a progress bar.
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;
        progressBarOption.rect.setHeight(50);

        // Set the progress and text values of the style option.
        int progress = 10;
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        QStyleOptionProgressBar progressBarOption2;
        progressBarOption2.state = QStyle::State_Enabled;
        progressBarOption2.direction = QApplication::layoutDirection();
        progressBarOption2.rect = option.rect;
        progressBarOption2.fontMetrics = QApplication::fontMetrics();
        progressBarOption2.minimum = 0;
        progressBarOption2.maximum = 100;
        progressBarOption2.textVisible = true;
        progressBarOption2.rect.setHeight(100);
        progressBarOption2.rect.setY(200);
        progressBarOption2.textAlignment = Qt::AlignBottom;

        // Set the progress and text values of the style option.
        int progress2 = 20;
        progressBarOption2.progress = progress2 < 0 ? 0 : progress2;
        progressBarOption2.text = QString().sprintf("%d%%", progressBarOption2.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption2, painter);
    }
};

#endif // POSTVIEWDELEGATE_H
