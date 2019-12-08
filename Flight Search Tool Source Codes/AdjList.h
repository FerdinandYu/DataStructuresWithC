//
//  AdjList.h
//  Airlines Plan
//
//  Created by 余宏昊 on 2018/12/19.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef AdjList_h
#define AdjList_h

#define MAX_VERTEX_NUM 100

#include "Schedule_DF.h"

typedef enum {DG, DN, UDG, UDN} GraphKind; //{有向图，有向网，无向图，无向网}
typedef int VertexType;
typedef struct ArcNode {
    VertexType         arrival;    //到达机场序号
    struct ArcNode    *nextarc;    //指向下一条弧的指针
    flight_record *flight_info;    //航班信息
} ArcNode;
typedef struct VNode {
    VertexType     departure;    //出发机场序号
    ArcNode        *firstarc;    //指向第一条依附该顶点的弧的指针
} VNode, AdjList[MAX_VERTEX_NUM];
typedef struct {
    AdjList         vertices;
    int        vexnum,arcnum;    //图的当前顶点数和弧数
    GraphKind           kind;    //图的种类标志
} ALGraph;

//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//
Status InitArcNode(ArcNode * arcnode);
    //初始化弧结点
Status InitVNode(VNode * vnode);
    //初始化表头结点
Status InitALGraph(ALGraph * graph);
    //初始化邻接表
ArcNode * CreateArcNode(flight_record * flight);
    //构建弧结点
Status AddArcNode(VNode * vnode, ArcNode * arcnode);
    //加入弧结点
//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//

Status InitArcNode(ArcNode * arcnode)
{
    arcnode->arrival=-1;
    arcnode->nextarc=NULL;
    arcnode->flight_info=NULL;
    return OK;
} //InitArcNode

Status InitVNode(VNode * vnode)
{
    vnode->departure=-1;
    vnode->firstarc=NULL;
    return OK;
} //InitVNode

Status InitALGraph(ALGraph * graph)
{
    memset(graph->vertices,0,sizeof(graph->vertices));
    graph->vexnum=graph->arcnum=0;
    graph->kind=DG;
    for(int i=0;i<MAX_VERTEX_NUM;++i)
        InitVNode(&graph->vertices[i]);
    return OK;
} //InitALGraph

ArcNode * CreateArcNode(flight_record * flight)
{
    ArcNode * newarc=(ArcNode *)malloc(sizeof(ArcNode));
    newarc->nextarc=NULL;
    newarc->arrival=flight->arrival;
    newarc->flight_info=flight;
    return newarc;
} //CreateArcNode

Status AddArcNode(VNode * vnode, ArcNode * arcnode)
{
    if(vnode->firstarc==NULL)
    {
        arcnode->nextarc=NULL;
        vnode->firstarc=arcnode;
    }
    else
    {
        if(arcnode->arrival<=vnode->firstarc->arrival)
        {
            arcnode->nextarc=vnode->firstarc;
            vnode->firstarc=arcnode;
        }
        else
        {
            ArcNode * pn = vnode->firstarc;
            for(;pn->nextarc!=NULL;pn=pn->nextarc)
            {
                if(arcnode->arrival<=pn->nextarc->arrival)
                {
                    arcnode->nextarc=pn->nextarc;
                    pn->nextarc=arcnode;
                    break;
                }
            }
            if(pn->nextarc==NULL)
            {
                arcnode->nextarc=NULL;
                pn->nextarc=arcnode;
            }
        }
    }
    return OK;
} //AddArcNode

#endif /* AdjList_h */
