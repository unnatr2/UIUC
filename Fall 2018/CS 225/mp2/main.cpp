#include "Image.h"
#include "StickerSheet.h"

int main()
{
  Image background, bridge_back, bridge_right, bridge_left, tree, brother_1, brother_2, brother_3;

  background.readFromFile("Background.png");
  bridge_back.readFromFile("Bridge_back.png");
  bridge_left.readFromFile("Bridge_left.png");
  bridge_right.readFromFile("Bridge_right.png");
  tree.readFromFile("Tree.png");
  brother_1.readFromFile("Brother_1.png");
  brother_2.readFromFile("Brother_2.png");
  brother_3.readFromFile("Brother_3.png");
  StickerSheet sheet(background, 7);
  sheet.addSticker(brother_3, 187, 243);
  sheet.addSticker(brother_2, 34, 35);
  sheet.addSticker(brother_1, 333, 87);
  sheet.addSticker(bridge_back, 1279, 660);
  sheet.addSticker(bridge_left, 0, 314);
  sheet.addSticker(bridge_right, 1446, 491);
  sheet.addSticker(tree, 1345, 0);
  Image output = sheet.render();
  output.writeToFile("myImage.png");

  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //

  return 0;
}
