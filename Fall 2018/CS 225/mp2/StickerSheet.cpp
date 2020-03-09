#include "StickerSheet.h"
#include "Image.h"
#include <iostream>
using namespace std;

StickerSheet::StickerSheet(const Image &picture, unsigned max)
{
    basePicture = new Image(picture);
    stickerCount = 0;
    maxStickers = max;
    arrX = new int[max];
    arrY = new int[max];
    arrCount = new int[max];
    arrStickers = new Image *[max];
    for (unsigned i = 0; i < max; i++)
    {
        arrStickers[i] = NULL;
        arrCount[i] = 0;
        arrX[i] = 0;
        arrY[i] = 0;
    }
}

StickerSheet::~StickerSheet()
{
    _destroy();
}

StickerSheet::StickerSheet(const StickerSheet &other)
{
    _copy(other);
}

const StickerSheet &StickerSheet::operator=(const StickerSheet &other)
{
    _destroy();
    _copy(other);
    return *this;
}

void StickerSheet::changeMaxStickers(unsigned max)
{
    if (maxStickers == max)
    {
        return;
    }
    Image **newArrStickers = new Image *[max];
    int *newArrCount = new int[max];
    int *newArrX = new int[max];
    int *newArrY = new int[max];
    unsigned newStickerCount = 0;
    Image *newBasePicture = new Image(*basePicture);

    if (maxStickers > max)
    {
        for (unsigned i = max; i < maxStickers; i++)
        {
            if (arrCount[i] != 0)
            {
                delete arrStickers[i];
                arrStickers[i] = NULL;
                arrCount[i]--;
                arrX[i] = 0;
                arrY[i] = 0;
            }
        }
        maxStickers = max;
    }
    if (maxStickers < max)
    {
        for (unsigned i = 0; i < max; i++)
        {
            newArrStickers[i] = NULL;
            newArrCount[i] = 0;
            newArrX[i] = 0;
            newArrY[i] = 0;
        }
    }
    for (unsigned i = 0; i < maxStickers; i++)
    {
        if (arrCount[i] != 0)
        {
            newArrCount[i] = arrCount[i];
            newArrStickers[i] = new Image(*arrStickers[i]);
            newArrX[i] = arrX[i];
            newArrY[i] = arrY[i];
            newStickerCount++;
        }
        else
        {
            newArrCount[i] = 0;
        }
    }
    _destroy();
    maxStickers = max;
    arrStickers = newArrStickers;
    arrCount = newArrCount;
    arrX = newArrX;
    arrY = newArrY;
    stickerCount = newStickerCount;
    basePicture = newBasePicture;
}

int StickerSheet::addSticker(Image &sticker, unsigned x, unsigned y)
{
    if (stickerCount < maxStickers)
    {
        for (unsigned i = 0; i < maxStickers; i++)
        {
            if (arrCount[i] == 0)
            {
                arrStickers[i] = new Image(sticker);
                arrCount[i]++;
                stickerCount++;
                arrX[i] = x;
                arrY[i] = y;
                return i;
            }
        }
    }
    return -1;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y)
{
    if (arrCount[index] != 0 && index < maxStickers)
    {
        arrX[index] = x;
        arrY[index] = y;
        return true;
    }
    return false;
}

void StickerSheet::removeSticker(unsigned index)
{
    if (arrCount[index] != 0 && index < maxStickers)
    {
        delete arrStickers[index];
        arrStickers[index] = NULL;
        arrCount[index]--;
        arrX[index] = 0;
        arrY[index] = 0;
        stickerCount--;
    }
}

Image *StickerSheet::getSticker(unsigned index) const
{
    if (arrCount[index] != 0 && index < maxStickers)
    {
        return arrStickers[index];
    }
    return NULL;
}

Image StickerSheet::render() const
{
    unsigned maxWidth = basePicture->width();
    unsigned maxHeight = basePicture->height();
    for (unsigned i = 0; i < maxStickers; i++)
    {
        if (arrCount[i] != 0)
        {
            unsigned x = arrX[i] + arrStickers[i]->width();
            unsigned y = arrY[i] + arrStickers[i]->height();
            if (maxWidth < x)
            {
                maxWidth = x;
            }
            if (maxHeight < y)
            {
                maxHeight = y;
            }
        }
    }
    Image *output = new Image(*basePicture);
    output->resize(maxWidth, maxHeight);

    for (unsigned i = 0; i < maxStickers; i++)
    {
        if (arrCount[i] != 0)
        {
            Image currSticker = *arrStickers[i];
            for (unsigned y = arrY[i]; y < arrY[i] + currSticker.height(); y++)
            {
                for (unsigned x = arrX[i]; x < arrX[i] + currSticker.width(); x++)
                {
                    HSLAPixel stickerPixel = currSticker.getPixel(x - arrX[i], y - arrY[i]);
                    HSLAPixel *outPixel = &output->getPixel(x, y);
                    if (stickerPixel.a != 0)
                    {
                        outPixel->h = stickerPixel.h;
                        outPixel->s = stickerPixel.s;
                        outPixel->l = stickerPixel.l;
                        outPixel->a = stickerPixel.a;
                    }
                }
            }
        }
    }
    Image out = *output;
    delete output;
    output = NULL;
    return out;
}

void StickerSheet::_copy(const StickerSheet &other)
{
    basePicture = new Image(*other.basePicture);
    maxStickers = other.maxStickers;
    stickerCount = other.stickerCount;
    arrCount = new int[maxStickers];
    arrX = new int[maxStickers];
    arrY = new int[maxStickers];
    arrStickers = new Image *[maxStickers];
    for (unsigned i = 0; i < maxStickers; i++)
    {
        if (other.arrCount[i] != 0)
        {
            arrStickers[i] = new Image(*other.arrStickers[i]);
            arrCount[i] = other.arrCount[i];
            arrX[i] = other.arrX[i];
            arrY[i] = other.arrY[i];
        }
        else
        {
            arrStickers[i] = NULL;
            arrCount[i] = 0;
            arrX[i] = 0;
            arrY[i] = 0;
        }
    }
}

void StickerSheet::_destroy()
{
    delete basePicture;
    for (unsigned i = 0; i < maxStickers; i++)
    {
        if (arrCount[i] != 0)
        {
            delete arrStickers[i];
            arrStickers[i] = NULL;
        }
    }
    delete[] arrStickers;
    delete[] arrCount;
    delete[] arrX;
    delete[] arrY;
    basePicture = NULL;
    arrStickers = NULL;
    arrCount = NULL;
    arrX = NULL;
    arrY = NULL;
}