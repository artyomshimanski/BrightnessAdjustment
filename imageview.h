#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QLabel>

class QPixmap;

class ImageView : public QLabel
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);

    void setPixmap(const QPixmap &pix);
    
protected:
    void mousePressEvent(QMouseEvent *);

signals:
    void clicked();
};

#endif // IMAGEVIEW_H
