//
//  octree.cpp
//  littleGif
//
//  Created by 1234 on 2022/5/7.
//

#include "octree.h"
#include <iomanip>

Octree::Octree() {
  global_root_node = new TreeNode(0);
  leaf_nodes.push_back(global_root_node);
  global_root_node->mapto = 0;
  head_nodes[0].push_back(0);
}

void Octree::RecursiveReduce(int idx) {
  if (leaf_nodes[idx]->is_reduce == false) {
    return;
  }
  leaf_nodes[idx]->is_reduce = false;
  for (int i = 0; i < 8; i++) {
    if (leaf_nodes[idx]->ptr_child[i] != NULL)
      RecursiveReduce(leaf_nodes[idx]->ptr_child[i]->mapto);
  }
}

int Octree::QueryColor(TreeNode *&anode, unsigned int red, unsigned int green,
                       unsigned int blue) {
  if (anode->is_leaf) {
    return anode->mapto;
  } else {
    unsigned int shift = 7 - anode->level;
    unsigned int Idx = (((red & mask[anode->level]) >> shift) << 2) |
                       (((green & mask[anode->level]) >> shift) << 1) |
                       ((blue & mask[anode->level]) >> shift);
    if (pri)
      printf("shift = %d,idx = %d，下一层数是%d\n", shift, Idx,
             anode->level + 1);
    return QueryColor(anode->ptr_child[Idx], red, green, blue);
  }
}

void Octree::AddColor(TreeNode *&anode, unsigned int red, unsigned int green,
                      unsigned int blue, bool newnode) {
  if (anode->level < 8 && newnode) //将该节点的索引放进所在层中，方便之后搜索
  {
    head_nodes[anode->level].push_back(anode->mapto);
  }
  anode->redsum += red;
  anode->greensum += green;
  anode->bluesum += blue;
  anode->count += 1; //即上图中的数量，即节点代表的像素数量
  if (anode->is_leaf)
    return;

  if (anode->level == 8) //如果是第8层
  {
    anode->is_leaf = true;
    anode->is_reduce = false;
    if (newnode) //新颜色，需要新建一个叶子节点
    {
      currentcolor += 1;
      anode->is_leaf = true;
      if (currentcolor > maxcolor) {
        //需要减去一个颜色！将在第二步中说到
        ReduceColor();
      }
    }
  } else {
    unsigned int shift = 7 - anode->level; //用于计算7颜色的7位二进制
    unsigned int Idx = (((red & mask[anode->level]) >> shift) << 2) |
                       (((green & mask[anode->level]) >> shift) << 1) |
                       ((blue & mask[anode->level]) >> shift);

    if (anode->ptr_child[Idx] == nullptr) //子节点不存在，需要新建一个
    {
      anode->ptr_child[Idx] = new TreeNode(anode->level + 1);
      leaf_nodes.push_back(anode->ptr_child[Idx]);
      anode->ptr_child[Idx]->mapto = leaf_nodes.size() - 1;
      anode->childnum += 1;
      AddColor(anode->ptr_child[Idx], red, green, blue, true);
    } else {
      AddColor(anode->ptr_child[Idx], red, green, blue, false);
    }
  }
}

void Octree::ReduceColor() {
  int minidx = -1;
  for (int i = 7; i >= 0; i--) {
    int mincount = 0x7f7f7f;
    for (int j = 0; j < head_nodes[i].size(); j++) {
      int idx = head_nodes[i][j];
      if (leaf_nodes[idx]->is_reduce && leaf_nodes[idx]->childnum >= 2 &&
          mincount > leaf_nodes[idx]->childnum) {
        mincount = leaf_nodes[idx]->childnum;
        minidx = idx;
      }
    }
    if (minidx != -1)
      break;
  }
  RecursiveReduce(minidx);
  leaf_nodes[minidx]->is_leaf = true;
  currentcolor -= (leaf_nodes[minidx]->childnum - 1);
  if (pri)
    printf("得到索引%d,目前颜色数量%d！\n", minidx, currentcolor);
}

void Octree::PutColors(TreeNode *root_node, u_int8_t *rgb, int width, int height) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      AddColor(root_node, rgb[(i * width + j) * 3],
               rgb[(i * width + j) * 3 + 1], rgb[(i * width + j) * 3 + 2],
               false);
    }
  }
}

void Octree::RGB2Index(TreeNode *root_node, u_int8_t *rgb, int width, int height,
                       u_int32_t *color_table, u_int8_t *rgb_index) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int vectoridx = QueryColor(root_node, rgb[(i * width + j) * 3],
                                 rgb[(i * width + j) * 3 + 1],
                                 rgb[(i * width + j) * 3 + 2]);
      if (pri)
        printf("vectoridx = %d\n", vectoridx);
      rgb[(i * width + j) * 3] =
          leaf_nodes[vectoridx]->redsum / leaf_nodes[vectoridx]->count;
      rgb[(i * width + j) * 3 + 1] =
          leaf_nodes[vectoridx]->greensum / leaf_nodes[vectoridx]->count;
      rgb[(i * width + j) * 3 + 2] =
          leaf_nodes[vectoridx]->bluesum / leaf_nodes[vectoridx]->count;
      uint32_t color = ((int)rgb[(i * width + j) * 3]) << 16 |
                       ((int)rgb[(i * width + j) * 3 + 1]) << 8 |
                       (int)rgb[(i * width + j) * 3 + 2];
      //                std::cout<<std::hex<<color<<std::endl;
      std::map<uint32_t, u_int8_t>::iterator item;
      item = color_map.find(color);
      if (item == color_map.end()) {
        color_table[tick] = color;       // 局部颜色表
        rgb_index[i * width + j] = tick; // rgb索引表
        color_map[color] = tick++;       // map记录颜色
      } else {
        rgb_index[i * width + j] = color_map[color];
      }
    }
  }
}

void Octree::Quantizise(u_int8_t *rgb, int width, int height,
                        u_int32_t *color_table, u_int8_t *rgb_index) {

  TreeNode *RootNode = new TreeNode(0);
  leaf_nodes.push_back(RootNode);
  RootNode->mapto = 0;
  head_nodes[0].push_back(0);

  PutColors(RootNode, rgb, width, height);

  std::cout << "octree build success..." << std::endl;
  RGB2Index(RootNode, rgb, width, height, color_table, rgb_index);
  delete RootNode;
}
