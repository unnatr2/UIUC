#pragma once
#include "Image.h"

 class StickerSheet{
     public:
        StickerSheet(const Image &picture, unsigned max);
        ~StickerSheet();
        StickerSheet(const StickerSheet &other);
        const StickerSheet & operator = (const StickerSheet &other);
        void changeMaxStickers(unsigned max);
        int addSticker(Image &sticker, unsigned x, unsigned y);
        bool translate(unsigned index, unsigned x, unsigned y);
        void removeSticker(unsigned index);
        Image* getSticker(unsigned index) const;
        Image render() const;
    private:
        Image* *arrStickers;
        int *arrX;
        int *arrY;
        int *arrCount;
        unsigned stickerCount;
        unsigned maxStickers;
        void _copy(const StickerSheet &other);
        void _destroy();
        Image *basePicture;
 };