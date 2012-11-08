#include "imageprocessor.h"
#include "colormodelhelper.h"

ImageProcessor::ImageProcessor()
{
}

QImage ImageProcessor::colorTransfer(const QImage &lightImg, const QImage &targetImg) const
{
    QImage resultImage(targetImg);

    Vector3 eLightImg = expectedValue(lightImg);
    Vector3 eTargetImg = expectedValue(targetImg);

    Vector3 dLightImg = dispersion(lightImg, eLightImg);
    Vector3 dTargetImg = dispersion(targetImg, eTargetImg);

    for (int y = 0; y < resultImage.height(); y++)
    {
        for (int x = 0; x < resultImage.width(); x++)
        {
            LAB currLabColor = RGB2LAB(resultImage.pixel(x, y));

            LAB newLabColor;
            newLabColor.l = eLightImg.val1 + (currLabColor.l - eTargetImg.val1) * dLightImg.val1 / dTargetImg.val1;
            newLabColor.a = eLightImg.val2 + (currLabColor.a - eTargetImg.val2) * dLightImg.val2 / dTargetImg.val2;
            newLabColor.b = eLightImg.val3 + (currLabColor.b - eTargetImg.val3) * dLightImg.val3 / dTargetImg.val3;

            QRgb newRGBColor = LAB2RGB(newLabColor);
            resultImage.setPixel(x, y, newRGBColor);
        }
    }

    return resultImage;
}

Vector3 ImageProcessor::expectedValue(const QImage &img) const
{
    Vector3 cVector;
    cVector.val1 = 0.0;
    cVector.val2 = 0.0;
    cVector.val3 = 0.0;

    double imgSize = img.width() * img.height();

    for (int y = 0; y < img.height(); y++)
    {
        for (int x = 0; x < img.width(); x++)
        {
            LAB labColor = RGB2LAB(img.pixel(x, y));
            cVector.val1 += labColor.l;
            cVector.val2 += labColor.a;
            cVector.val3 += labColor.b;
        }
    }

    Vector3 eVector;
    eVector.val1 = 1.0 / imgSize * cVector.val1;
    eVector.val2 = 1.0 / imgSize * cVector.val2;
    eVector.val3 = 1.0 / imgSize * cVector.val3;

    return eVector;
}

Vector3 ImageProcessor::dispersion(const QImage &img, const Vector3 &expectedVal) const
{
    Vector3 ceVector;
    ceVector.val1 = 0.0;
    ceVector.val2 = 0.0;
    ceVector.val3 = 0.0;

    double imgSize = img.width() * img.height();

    for (int y = 0; y < img.height(); y++)
    {
        for (int x = 0; x < img.width(); x++)
        {
            LAB labColor = RGB2LAB(img.pixel(x, y));
            ceVector.val1 += pow(labColor.l - expectedVal.val1, 2.0);
            ceVector.val2 += pow(labColor.a - expectedVal.val2, 2.0);
            ceVector.val3 += pow(labColor.b - expectedVal.val3, 2.0);
        }
    }

    Vector3 dVector;
    dVector.val1 = sqrt(1.0 / imgSize * ceVector.val1);
    dVector.val2 = sqrt(1.0 / imgSize * ceVector.val2);
    dVector.val3 = sqrt(1.0 / imgSize * ceVector.val3);

    return dVector;
}
