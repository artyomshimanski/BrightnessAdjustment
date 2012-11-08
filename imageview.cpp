#include "imageview.h"

#include <QPixmap>

ImageView::ImageView(QWidget *parent) :
    QLabel(parent)
{
    setMinimumSize(256, 320);
    setAlignment(Qt::AlignCenter);
    setFrameShape(QFrame::StyledPanel);
}

void ImageView::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

void ImageView::setPixmap(const QPixmap &pix)
{
    QLabel::setPixmap(pix.scaled(minimumSize(), Qt::KeepAspectRatio));
}
