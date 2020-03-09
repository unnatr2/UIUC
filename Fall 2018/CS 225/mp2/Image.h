#pragma once

#include "cs225/PNG.h"
using cs225::PNG;

#include "cs225/HSLAPixel.h"
using cs225::HSLAPixel;

class Image : public PNG {
    public:
        void lighten();
        void lighten(double amount);
        void darken();
        void darken(double amount);
        void saturate();
        void saturate(double amount);
        void desaturate();
        void desaturate(double amount);
        void greyscale();
        void rotateColor(double degrees);
        void illinify();
        void scale(double factor);
        void scale(unsigned w, unsigned h);
};
