
#include "create_gif.h"
#include <iostream>

int main() {
  std::cout << "HelloWorld" << std::endl;
  //    separate();
  GeneratorBuilder *builder =
      new GeneratorBuilder("/Users/a1234/Desktop/testGif.gif", 384, 216);
  GifGenerator *gif_generator = new GifGenerator(builder);
  delete builder;
  gif_generator->CreateGif();
  return 0;
}
