
#include "create_gif.h"
#include <iostream>

int main() {
  std::cout << "HelloWorld" << std::endl;
  //    separate();
  GifGenerator *gif_generator =
      new GifGenerator("/Users/a1234/Desktop/testGif.gif", 384, 216);
  gif_generator->CreateGif();
  return 0;
}
