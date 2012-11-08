#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>

typedef struct
{
    double val1;
    double val2;
    double val3;
} Vector3;

class ImageProcessor
{
public:
    ImageProcessor();

    QImage colorTransfer(const QImage &lightImg, const QImage &targetImg) const;

private:
    Vector3 expectedValue(const QImage &img) const;
    Vector3 dispersion(const QImage &img, const Vector3 &expectedVal) const;
};

#endif // IMAGEPROCESSOR_H
