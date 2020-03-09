#include "Image.h"

void Image::lighten(){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.l>0.9){
                pixel.l=1;
            }
            else{
                pixel.l+=0.1;
            }
        }
    }
}
void Image::lighten(double amount){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.l>(1-amount)){
                pixel.l=1;
            }
            else{
                pixel.l+=amount;
            }
        }
    }
}
void Image::darken(){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.l<0.1){
                pixel.l=0;
            }
            else{
                pixel.l-=0.1;
            }
        }
    }
}
void Image::darken(double amount){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.l<amount){
                pixel.l=0;
            }
            else{
                pixel.l-=amount;
            }
        }
    }
}
void Image::saturate(){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.s>0.9){
                pixel.s=1;
            }
            else{
                pixel.s+=0.1;
            }
        }
    }
}
void Image::saturate(double amount){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.s>(1-amount)){
                pixel.s=1;
            }
            else{
                pixel.s+=amount;
            }
        }
    }
}
void Image::desaturate(){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.s<0.1){
                pixel.s=0;
            }
            else{
                pixel.s-=0.1;
            }
        }
    }
}
void Image::desaturate(double amount){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.s<amount){
                pixel.s=0;
            }
            else{
                pixel.s-=amount;
            }
        }
    }
}
void Image::greyscale(){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            pixel.s=0;
        }
    }
}
void Image::rotateColor(double degrees){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.h+degrees>360){
                pixel.h=(pixel.h+degrees)-360;
            }
            else if (pixel.h+degrees<0){
                pixel.h=pixel.h+360+degrees;
            }
            else{
                pixel.h+=degrees;
            }
        }
    }
}
void Image::illinify(){
    for (unsigned y=0; y<height(); y++){
        for (unsigned x=0; x<width(); x++){
            HSLAPixel & pixel = getPixel(x,y);
            if (pixel.h>=0 & pixel.h<113.5){
                    pixel.h=11;
            }
            else if (pixel.h>113.5 & pixel.h<293.5){
                pixel.h=216;
            }
            else {
                pixel.h=11;
            }
        }
    }
}
void scaleDown(Image* Img){

}
void Image::scale(double factor){
    if (factor>1.0){
        PNG newImg(width()*factor,height()*factor);
        for (unsigned orig_y=0; orig_y<height(); orig_y++){
            for (unsigned orig_x=0; orig_x<width(); orig_x++){
                HSLAPixel & origPixel = getPixel(orig_x,orig_y);
                unsigned new_x=orig_x*factor;
                unsigned new_y=orig_y*factor;
                for (unsigned loop_y=0; loop_y<factor; loop_y++){
                    for (unsigned loop_x=0; loop_x<factor; loop_x++){
                        HSLAPixel & newPixel = newImg.getPixel(new_x+loop_x,new_y+loop_y);
                        newPixel=origPixel;
                    }
                }
            }
        }
        resize(width()*factor,height()*factor);
        for (unsigned y=0; y<height(); y++){
            for (unsigned x=0; x<width(); x++){
                HSLAPixel & pixel = getPixel(x,y);
                pixel=newImg.getPixel(x,y);
            }
        }
    }
    else if (factor<1.0){
        double factorInv=(1.0/factor);
        PNG newImg(width()*factor,height()*factor);
        for (unsigned new_y=0; new_y<newImg.height(); new_y++){
            for (unsigned new_x=0; new_x<newImg.width(); new_x++){
                HSLAPixel & newPixel = newImg.getPixel(new_x,new_y);
                unsigned orig_x=new_x*factorInv;
                unsigned orig_y=new_y*factorInv;
                float avg_h=0, avg_s=0, avg_l=0, avg_a=0;
                for (unsigned loop_y=0; loop_y<factorInv; loop_y++){
                    for (unsigned loop_x=0; loop_x<factorInv; loop_x++){
                        HSLAPixel & origPixel = getPixel(orig_x+loop_x,orig_y+loop_y);
                        avg_h+=origPixel.h;
                        avg_s+=origPixel.s;
                        avg_l+=origPixel.l;
                        avg_a+=origPixel.a;
                    }
                }
                newPixel.h=avg_h/(factorInv*factorInv);
                newPixel.s=avg_s/(factorInv*factorInv);
                newPixel.l=avg_l/(factorInv*factorInv);
                newPixel.a=avg_a/(factorInv*factorInv);
            }
        }
        resize(width()*factor,height()*factor);
        for (unsigned y=0; y<height(); y++){
            for (unsigned x=0; x<width(); x++){
                HSLAPixel & pixel = getPixel(x,y);
                pixel=newImg.getPixel(x,y);
            }
        }
    } 
}
void Image::scale(unsigned w, unsigned h){
    double factor=w/width();
    scale(factor);
}