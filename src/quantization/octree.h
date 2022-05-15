//
//  octree.hpp
//  littleGif
//
//  Created by 1234 on 2022/5/7.
//

#ifndef octree_h
#define octree_h

#include <iostream>
#include <vector>
#include <map>
struct RgbTuple{
    u_int8_t r;
    u_int8_t g;
    u_int8_t b;
};
struct node
{
   bool IsLeaf;//是否为叶子节点
   bool Reduce;//是否可归并，将在第二步说到
   unsigned int count;//此节点记录了多少个像素的颜色
   unsigned int level;//当 level = 7就是叶子节点

   unsigned int redsum;//红色分量总和
   unsigned int greensum;//绿色分量总和
   unsigned int bluesum;//蓝色分量总和
   unsigned int mapto;//映射到一个vector中方便根据索引寻找
   unsigned int childnum;//不为NULL的子节点数量
   node* ptrChild[8];//8个子节点，初始化为NULL
   node(unsigned int lev)
   {
       level = lev;
       redsum = greensum = bluesum =  count = childnum =  0;
       mapto = -1;
       for (int i = 0; i < 8; i++)
       {
           ptrChild[i] = NULL;
       }
       IsLeaf =  false;
       Reduce = true;
   }
   
};
class Octree{
private:
    std::vector<node*> LeafNodes;
    std::vector<int> HeadNodes[8];
    int currentcolor = 0;
    int maxcolor = 256;
    bool pri = false;
    unsigned char mask[8] = { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };
    std::map<u_int32_t,u_int8_t> color_map;
    int tick=0;
public:
    int QueryColor(node*& anode, unsigned int red, unsigned int green, unsigned int blue);
    void ReduceColor();
    void RecursiveReduce(int idx);
    void addColor(node* &anode,unsigned int red, unsigned int green, unsigned int blue,bool newnode);
    void Quantizise(u_int8_t* rgb,int width,int height,u_int32_t* color_table,u_int8_t* rgb_index);
};

#endif /* octree_h */
