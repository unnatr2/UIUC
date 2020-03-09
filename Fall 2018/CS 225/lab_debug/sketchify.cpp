#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
using namespace cs225;

// sets up the output image
PNG* setupOutput(unsigned w, unsigned h) {
  PNG* image = new PNG(w, h);
  return image;
}

// Returns my favorite color
HSLAPixel* myFavoriteColor(double hue) {
  HSLAPixel* p= new HSLAPixel(hue, 0.8, 0.5);
  return p;
}

void sketchify(std::string inputFile, std::string outputFile) {
    // Load in.png
    PNG* original = new PNG;
    original->readFromFile(inputFile);
    unsigned width = original->width();
    unsigned height = original->height();
    // std::cout << "width=" << width << '\n';
    // std::cout << "height=" << height << '\n';

    // Create out.png
    PNG* output = setupOutput(width, height);
    // std::cout << "width=" << output->width() << '\n';
    // std::cout << "height=" << output->height() << '\n';



    // Load our favorite color to color the outline
    HSLAPixel* myPixel = myFavoriteColor(192);
    // std::cout << "H=" << myPixel->h << '\n';

    // Go over the whole image, and if a pixel differs from that to its upper
    // left, color it my favorite color in the output
    for (unsigned y = 1; y < height; y++) {
        for (unsigned x = 1; x < width; x++) {
            // Calculate the pixel difference
            HSLAPixel& prev = original->getPixel(x - 1, y - 1);
            HSLAPixel& curr = original->getPixel(x, y);
            double diff = std::fabs(curr.h - prev.h);

            // If the pixel is an edge pixel,
            // color the output pixel with my favorite color
            HSLAPixel& currOutPixel = output->getPixel(x, y);
            if (diff > 20) {
                currOutPixel = *myPixel;
                // std::cout << currOutPixel << '\n';
            }
        }
    }

    // Save the output file
    output->writeToFile(outputFile);

    // Clean up memory
    delete myPixel;
    delete output;
    delete original;
}
