//
//  octree.cpp
//  littleGif
//
//  Created by 1234 on 2022/5/7.
//

#include "octree.h"
#include <iomanip>
void Octree::RecursiveReduce(int idx)
{
    if (LeafNodes[idx]->Reduce == false)
    {
        return;
    }
    LeafNodes[idx]->Reduce = false;
    for (int i = 0; i < 8; i++)
    {
        if (LeafNodes[idx]->ptrChild[i] != NULL)
            RecursiveReduce(LeafNodes[idx]->ptrChild[i]->mapto);
    }
}
int Octree::QueryColor(node*& anode, unsigned int red, unsigned int green, unsigned int blue)
{
    if (anode->IsLeaf)
    {
        return anode->mapto;
    }
    else
    {
        unsigned int shift = 7 - anode->level;
        unsigned int Idx = (((red & mask[anode->level]) >> shift) << 2) |
            (((green & mask[anode->level]) >> shift) << 1) |
            ((blue & mask[anode->level]) >> shift);
        if (pri) printf("shift = %d,idx = %d，下一层数是%d\n", shift, Idx, anode->level + 1);
        return QueryColor(anode->ptrChild[Idx], red, green, blue);
    }
}

void Octree::addColor(node* &anode,unsigned int red, unsigned int green, unsigned int blue,bool newnode)
{
    if (anode->level < 8 && newnode)//将该节点的索引放进所在层中，方便之后搜索
    {
        HeadNodes[anode->level].push_back(anode->mapto);
    }
    anode->redsum += red;
    anode->greensum += green;
    anode->bluesum += blue;
    anode->count += 1;//即上图中的数量，即节点代表的像素数量
    if (anode->IsLeaf)
        return;

    if (anode->level == 8)//如果是第8层
    {
        anode->IsLeaf = true;
        anode->Reduce = false;
        if (newnode)//新颜色，需要新建一个叶子节点
        {
            currentcolor += 1;
            anode->IsLeaf = true;
            if (currentcolor > maxcolor)
            {
                //需要减去一个颜色！将在第二步中说到
                ReduceColor();
            }
         }
    }
    else
    {
        unsigned int shift = 7 - anode->level;//用于计算7颜色的7位二进制
        unsigned int Idx = (((red & mask[anode->level]) >> shift) << 2) |
            (((green & mask[anode->level]) >> shift) << 1) |
          ((blue & mask[anode->level]) >> shift);
       
        if (anode->ptrChild[Idx] == nullptr)//子节点不存在，需要新建一个
        {
            anode->ptrChild[Idx] = new node(anode->level + 1);
            LeafNodes.push_back(anode->ptrChild[Idx]);
            anode->ptrChild[Idx]->mapto = LeafNodes.size() - 1;
            anode->childnum += 1;
            addColor(anode->ptrChild[Idx], red, green, blue,true);
        }
        else
        {
            addColor(anode->ptrChild[Idx], red, green, blue, false);
        }
    }
}
void Octree::ReduceColor()
{
    int minidx = -1;
    for (int i = 7; i >=0 ; i--)
    {
        int mincount = 0x7f7f7f;
        for (int j = 0; j < HeadNodes[i].size(); j++)
        {
            int idx = HeadNodes[i][j];
            if (LeafNodes[idx]->Reduce && LeafNodes[idx]->childnum >= 2 && mincount > LeafNodes[idx]->childnum)
            {
                mincount = LeafNodes[idx]->childnum;
                minidx = idx;
            }
        }
        if (minidx != -1)
            break;
    }
    RecursiveReduce(minidx);
    LeafNodes[minidx]->IsLeaf = true;
    currentcolor -= (LeafNodes[minidx]->childnum - 1);
    if (pri)printf("得到索引%d,目前颜色数量%d！\n", minidx,currentcolor);
}

void Octree::Quantizise(u_int8_t* rgb,int width,int height,u_int32_t* color_table,u_int8_t* rgb_index){
    
    node* RootNode = new node(0);
    LeafNodes.push_back(RootNode);
    RootNode->mapto = 0;
    HeadNodes[0].push_back(0);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            addColor(RootNode, rgb[(i * width + j) * 3], rgb[(i * width + j) * 3+1], rgb[(i * width + j) * 3+2], false);
        }
    }
    std::cout<<"octree build success..."<<std::endl;
    for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int vectoridx =  QueryColor(RootNode, rgb[(i * width + j) * 3], rgb[(i * width + j) * 3+1], rgb[(i * width + j) * 3+2]);
                if (pri)  printf("vectoridx = %d\n", vectoridx);
                rgb[(i * width + j) * 3] = LeafNodes[vectoridx]->redsum / LeafNodes[vectoridx]->count;
                rgb[(i * width + j) * 3+1] = LeafNodes[vectoridx]->greensum / LeafNodes[vectoridx]->count;
                rgb[(i * width + j) * 3+2] = LeafNodes[vectoridx]->bluesum / LeafNodes[vectoridx]->count;
//                std::cout << std::hex << std::setfill('0') << std::setw(2) <<   (int)rgb[(i * width + j) * 3];
//                std::cout << std::hex << std::setfill('0') << std::setw(2)  <<(int)rgb[(i * width + j) * 3+1];
//                std::cout << std::hex <<  std::setfill('0') << std::setw(2)<<(int)rgb[(i * width + j) * 3+2]<<std::endl;
                
                uint32_t color = ((int)rgb[(i * width + j) * 3])<<16|((int)rgb[(i * width + j) * 3+1])<<8|(int)rgb[(i * width + j) * 3+2];
//                std::cout<<std::hex<<color<<std::endl;
                std::map<uint32_t ,u_int8_t>::iterator item;
                item = color_map.find(color);
                if(item == color_map.end()){
                    color_table[tick] = color; // 局部颜色表
                    rgb_index[i*width+j] = tick; // rgb索引表
//                    std::cout<<"tick: "<<tick<<std::endl;
//                    std::cout<<(rgb_index[i*width+j])<<std::endl;
                    color_map[color] = tick++; // map记录颜色
                }
                else{
                    rgb_index[i*width+j] = color_map[color];
                }
            }
        }
}
