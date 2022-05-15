//
//  octree.hpp
//  littleGif
//
//  Created by 1234 on 2022/5/7.
//

#ifndef octree_h
#define octree_h

#include <iostream>
#include <map>
#include <vector>
struct RgbTuple {
  u_int8_t r;
  u_int8_t g;
  u_int8_t b;
};

struct TreeNode {
  bool is_leaf;        //是否为叶子节点
  bool is_reduce;        //是否可归并，将在第二步说到
  unsigned int count; //此节点记录了多少个像素的颜色
  unsigned int level; //当 level = 7就是叶子节点

  unsigned int redsum;   //红色分量总和
  unsigned int greensum; //绿色分量总和
  unsigned int bluesum;  //蓝色分量总和
  unsigned int mapto;    //映射到一个vector中方便根据索引寻找
  unsigned int childnum; //不为NULL的子节点数量
  TreeNode *ptr_child[8];     // 8个子节点，初始化为NULL
  TreeNode(unsigned int lev) {
    level = lev;
    redsum = greensum = bluesum = count = childnum = 0;
    mapto = -1;
    for (int i = 0; i < 8; i++) {
      ptr_child[i] = NULL;
    }
    is_leaf = false;
    is_reduce = true;
  }
};

class Octree {
private:
  std::vector<TreeNode *> leaf_nodes;
  std::vector<int> head_nodes[8];
  int currentcolor = 0;
  int maxcolor = 256;
  bool pri = false;
  unsigned char mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
  std::map<u_int32_t, u_int8_t> color_map;
  int tick = 0;

public:
  TreeNode *global_root_node;
  Octree();
  //    ~Octree();
  int QueryColor(TreeNode *&anode, unsigned int red, unsigned int green,
                 unsigned int blue);
  void PutColors(TreeNode *root_node, u_int8_t *rgb, int width, int height);
  void RGB2Index(TreeNode *root_node, u_int8_t *rgb, int width, int height,
                 u_int32_t *color_table, u_int8_t *rgb_index);
  void ReduceColor();
  void RecursiveReduce(int idx);
  void AddColor(TreeNode *&anode, unsigned int red, unsigned int green,
                unsigned int blue, bool newnode);
  void Quantizise(u_int8_t *rgb, int width, int height, u_int32_t *color_table,
                  u_int8_t *rgb_index);
};

#endif /* octree_h */
