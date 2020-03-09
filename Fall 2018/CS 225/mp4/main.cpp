
#include "cs225/PNG.h"
#include "FloodFilledImage.h"
#include "Animation.h"

#include "imageTraversal/DFS.h"
#include "imageTraversal/BFS.h"

#include "colorPicker/RainbowColorPicker.h"
#include "colorPicker/GradientColorPicker.h"
#include "colorPicker/GridColorPicker.h"
#include "colorPicker/SolidColorPicker.h"
#include "colorPicker/MyColorPicker.h"

using namespace cs225;

int main()
{

	// @todo [Part 3]
	// - The code below assumes you have an Animation called `animation`
	// - The code provided below produces the `myFloodFill.png` file you must
	//   submit Part 3 of this assignment -- uncomment it when you're ready.

	/*
	PNG lastFrame = animation.getFrame( animation.frameCount() - 1 );
	lastFrame.writeToFile("myFloodFill.png");
	animation.write("myFloodFill.gif");
	*/
	PNG png;
	png.readFromFile("baseWhite.png");
	PNG orig;
	orig.readFromFile("Wolf.png");

	FloodFilledImage image0(png);
	BFS bfs0(orig, Point(440, 435), 0.05);
	SolidColorPicker pk0(HSLAPixel(0, 1, 0));
	image0.addFloodFill(bfs0, pk0);
	Animation animation0 = image0.animate(2500);
	animation0.write("myFloodFill0.gif");
	PNG lastFrame0 = animation0.getFrame(animation0.frameCount() - 1);

	FloodFilledImage image1(lastFrame0);
	BFS bfs1(orig, Point(0, 0), 0.05);
	HSLAPixel color1(0, 0, 1);
	HSLAPixel color2(0, 0, .5);
	MyColorPicker pk1(color1, color2, Point(0, 0), 1000);
	image1.addFloodFill(bfs1, pk1);
	Animation animation = image1.animate(55000);
	animation.write("myFloodFill1.gif");
	PNG lastFrame1 = animation.getFrame(animation.frameCount() - 1);

	FloodFilledImage image2(lastFrame1);
	BFS bfs2(orig, Point(70, 500), 0.05);
	SolidColorPicker pk2(HSLAPixel(0, 0, .24));
	image2.addFloodFill(bfs2, pk2);
	animation = image2.animate(10000);
	animation.write("myFloodFill2.gif");
	PNG lastFrame2 = animation.getFrame(animation.frameCount() - 1);
	lastFrame2.writeToFile("myFloodFill.png");

	FloodFilledImage image3(lastFrame2);
	DFS dfs3(png, Point(0, 400), 0.05);
	SolidColorPicker pk3(HSLAPixel(0, 0, 1));
	image3.addFloodFill(dfs3, pk3);
	animation = image3.animate(10000);
	animation.write("myFloodFill3.gif");
	PNG lastFrame3 = animation.getFrame(animation.frameCount() - 1);
	system("convert myFloodFill0.gif myFloodFill1.gif myFloodFill2.gif myFloodFill3.gif myFloodFill.gif"); //myFloodFill3.gif
	return 0;
}
