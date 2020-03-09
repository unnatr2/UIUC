// CS 225:      MP1
// Developer:   Unnat Ramjiyani
// NetID:       unnatr2

#include "cs225/PNG.h"
using cs225::PNG;

#include "cs225/HSLAPixel.h"
using cs225::HSLAPixel;

#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Input:   inputFile, output,File
// Output:  NULL
// Purpose: Uses the inputFile to create PNG object and then save to outputFile
//          after rotating 180 degrees.
void rotate(std::string inputFile, std::string outputFile) {
    cs225::PNG in;
    in.readFromFile(inputFile);
    cs225::PNG out(in.width(),in.height());
    for (unsigned x=0; x<in.width(); x++) {
        for (unsigned y=0; y<in.height(); y++) {
            HSLAPixel & pixelIn = in.getPixel(x, y);
            HSLAPixel & pixelOut = out.getPixel(in.width()-x-1, in.height()-y-1);
            pixelOut.h=pixelIn.h;
            pixelOut.s=pixelIn.s;
            pixelOut.l=pixelIn.l;
            pixelOut.a=pixelIn.a;
        }
    }
    out.writeToFile(outputFile);
}

// Input:   width, height
// Output:  PNG object
// Purpose: Creates artwork using the dimensions provided using equations in two variables.
PNG myArt(unsigned int width, unsigned int height) {
    double half=width/2;
    PNG png(width, height);
    for (double x=0; x<png.width(); x++) {
        for (double y=0; y<png.height(); y++) {
            HSLAPixel & pixel = png.getPixel(x,y);
            double x1code=(pow(x-(half+220),4)/0.35);
            double y1code=(pow(y-half,4)/1.5);
            double left1sum=x1code+y1code;

            // Create inverse right side of I
            if (left1sum<=1900000000 || x>half+140 || y<half-268 || y>half+268 || x<half+10){
                pixel.h=216;
                pixel.s=.75;
                pixel.l=.29;
                pixel.a=1;
            }
            // Fill in the right side of I 
            else{
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create top horizontal line
            if (pow(x-(half-60),2)+pow(y-(half-235),2)<=81 || (x>=half-60 && x<=half+20 && y>=half-244 && y<=half-226)){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create second horizontal line
            if (pow(x-(half-57),2)+pow(y-(half-208),2)<=63.7 || (x>=half-57 && x<=half+20 && y>=half-216 && y<=half-200)){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create third horizontal line
            if (pow(x-(half-103),2)+pow(y-(half+213.5),2)<=81 || (x>=half-104 && x<=half+20 && y>=half+204 && y<=half+223)){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create bottom line
            if (pow(x-(half-122),2)+pow(y-(half+258),2)<=484 || (x>=half-122 && x<=half+20 && y>=half+236 && y<=half+268)){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create center line
            if (pow(x-(half-16.5),2)+pow(y-(half+180),2)<=113 || pow(x-(half-16.5),2)+pow(y-(half-174),2)<=113 || (x>=half-27 && x<=half-6 && y>=half-175 && y<=half+180)){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create center line
            if (pow(x-(half-49),2)+pow(y-(half+180),2)<=64 || pow(x-(half-49),2)+pow(y-(half-174),2)<=64 || (x>=half-57 && x<=half-41 && y>=half-175 && y<=half+180)){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Clean edges
            if (x>=half-144 && x<=half-100 && y>half+268 && y<=half+285){
                pixel.h=216;
                pixel.s=.75;
                pixel.l=.29;
                pixel.a=1;
            }
            // Create spiral
            if ((pow(x-(half-107),2)+pow(y-(half-226),2))<=pow(42,2) && x<half-107){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create spiral
            if ((pow(x-(half-107),2)+pow(y-(half-226),2))<=pow(26,2) && x<half-107){
                pixel.h=216;
                pixel.s=.75;
                pixel.l=.29;
                pixel.a=1;
            }
            // Create spiral
            if ((pow(x-(half-107),2)+pow(y-(half-210),2))<=pow(26,2) && x>=(half-107)){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create spiral
            if ((pow(x-(half-107),2)+pow(y-(half-210),2))<=pow(10,2) && x>=(half-107)){
                pixel.h=216;
                pixel.s=.75;
                pixel.l=.29;
                pixel.a=1;
            }
            // Create spiral
            if ((pow(x-(half-107),2)+pow(y-(half-228),2))<=pow(8,2) && x<(half-107)){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create spiral
            if (x>=(half-107) && x<=half+20 && y>=(half-268) && y<=(half-252)){
                pixel.h=192;
                pixel.s=1;
                pixel.l=1;
                pixel.a=1;
            }
            // Create orange border
            if (x<=70 || x>=width-70 || y<=70 || y>=height-70){
                pixel.h=11;
                pixel.s=.84;
                pixel.l=.5;
                pixel.a=1;
            }
            // Fill blue border
            if (x<=50 || x>=width-50 || y<=50 || y>=height-50){
                pixel.h=216;
                pixel.s=.75;
                pixel.l=.29;
                pixel.a=1;
            }
        }
    }
    
    // Create noise
    srand(time(NULL));
    for (double x=0; x<png.width(); x++) {
        for (double y=0; y<png.height(); y++) {
            HSLAPixel & pixel = png.getPixel(x,y);
            double random=rand()%100;
            if(pixel.l+(random/700)>1){
                pixel.l-=random/700;
            }
            else{
                pixel.l+=random/700;
            }
        }
    }
    return png;
}
