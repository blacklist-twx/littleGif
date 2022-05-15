//
//  gif_util.cpp
//  littleGif
//
//  Created by 1234 on 2022/5/4.
//

#include "gif_util.h"

YUV2RGB::YUV2RGB() {
  for (int i = 0; i < 256; i++) {
    __s_r_1370705v[i] = (1.370705 * (i - 128));
    __s_b_1732446u[i] = (1.732446 * (i - 128));
    __s_g_337633u[i] = (0.337633 * (i - 128));
    __s_g_698001v[i] = (0.698001 * (i - 128));
  }
}
void YUV2RGB::YUV420P2RGB(int width, int height, int stride, const u_int8_t *Y,
                          const u_int8_t *U, const u_int8_t *V, u_int8_t *rgb) {
  int r, g, b;
  int y, u, v;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      y = Y[i * stride + j];
      u = U[(i / 2 * stride / 2 + j / 2)];
      v = V[(i / 2 * stride / 2 + j / 2)];

      r = (int)y + __s_r_1370705v[v];
      g = (int)y - __s_g_337633u[u] - __s_g_698001v[v];
      b = (int)y + __s_b_1732446u[u];

      if (r > 255)
        r = 255;
      if (g > 255)
        g = 255;
      if (b > 255)
        b = 255;
      if (r < 0)
        r = 0;
      if (g < 0)
        g = 0;
      if (b < 0)
        b = 0;

      rgb[(i * width + j) * 3 + 0] = (u_int8_t)r;
      rgb[(i * width + j) * 3 + 1] = (u_int8_t)g;
      rgb[(i * width + j) * 3 + 2] = (u_int8_t)b;
    }
  }
}
