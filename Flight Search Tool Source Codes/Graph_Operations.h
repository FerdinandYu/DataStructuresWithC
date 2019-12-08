//
//  Graph.h
//  Airlines Plan
//
//  Created by 余宏昊 on 2018/12/19.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef Graph_h
#define Graph_h

#include "AdjList.h"
#include "AdjMatrix.h"
#include "Stack_Queue.h"

//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//
ALGraph * CreateALGFromDF(Schedule schedule);
    //根据DataFrame创建以邻接表为存储结构的图
MGraph * CreateMGFromALG(ALGraph algraph);
    //根据ALG创建以邻接矩阵为存储结构的图
Airport FirstAdjPort(ALGraph algraph, Airport airport1);
    //在邻接表中，找出所给机场的第一个直飞可达机场
Airport NextAdjPort(ALGraph algraph, Airport airport1, Airport airport2);
    //在邻接表中，找出所给机场在airport1之后的下一个直飞可达机场
Status DFSTALGraph(ALGraph algraph, int departure);
    //深度优先遍历
Status BFSTALGraph(ALGraph algraph, int departure);
    //广度优先遍历
Status PrintMGraph(MGraph * M);
    //打印邻接矩阵
Status CheckRoute(MGraph * M, Airport Departure, Airport Arrival);
    //判断两个机场之间的连通性
//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//

ALGraph * CreateALGFromDF(Schedule schedule)
{
    ALGraph * algraph=(ALGraph *)malloc(sizeof(ALGraph));
    InitALGraph(algraph);
    int taken[MAX_VERTEX_NUM]={0};
    int num_flights=schedule.row-1;
    int departure=0;
    for(int i=0;i<num_flights;++i)
    {
        departure=schedule.records[i].departure;
        if(taken[departure]==0)
        {
            algraph->vexnum++;
        }
        ArcNode * newarc=CreateArcNode(&schedule.records[i]);
        AddArcNode(&algraph->vertices[departure], newarc);
        taken[departure]=1;
        algraph->arcnum++;
    }
    return algraph;
} //CreateALGFromDF

MGraph * CreateMGFromALG(ALGraph algraph)
{
    MGraph * mgraph=(MGraph *)malloc(sizeof(MGraph));
    InitMGraph(mgraph);
    
    mgraph->vexnum=algraph.vexnum;
    for(int i=1;i<=algraph.vexnum;i++)
        mgraph->vexs[i]=1;
    
    for(int i=1;i<=algraph.vexnum;i++)   //直飞
        for(int airport1=FirstAdjPort(algraph, i);airport1>0;airport1=NextAdjPort(algraph, i, airport1))
        {
            mgraph->arcs[i][airport1].adj=1;
            mgraph->arcs[i][airport1].transfers[0]=0;mgraph->arcnum++;
            
        }
    
    int num_lines[MAX_VERTEX_NUM][MAX_VERTEX_NUM]={{0}};
    
    for(int i=1;i<=algraph.vexnum;i++)   //转机一次
    {
        for(int airport1=FirstAdjPort(algraph, i);airport1>0;airport1=NextAdjPort(algraph, i, airport1))
            for(int airport2=FirstAdjPort(algraph, airport1);airport2>0;airport2=NextAdjPort(algraph, airport1, airport2))
            {
                if(airport2!=i&&mgraph->arcs[i][airport2].adj==-1)
                {
                    mgraph->arcs[i][airport2].adj=2;
                    mgraph->arcs[i][airport2].transfers[num_lines[i][airport2]++]=airport1;
                    mgraph->arcnum++;
                }
            }
    }
    
    for(int i=1;i<=algraph.vexnum;i++)   //转机两次
    {
        for(int airport1=FirstAdjPort(algraph, i);airport1>0;airport1=NextAdjPort(algraph, i, airport1))
            for(int airport2=FirstAdjPort(algraph, airport1);airport2>0;airport2=NextAdjPort(algraph, airport1, airport2))
                for(int airport3=FirstAdjPort(algraph, airport2);airport3>0;airport3=NextAdjPort(algraph, airport2, airport3))
                {
                    if(airport3!=i&&mgraph->arcs[i][airport3].adj==-1)
                    {
                        mgraph->arcs[i][airport3].adj=3;
                        mgraph->arcs[i][airport3].transfers[num_lines[i][airport3]++]=airport1*100+airport2;
                        mgraph->arcnum++;
                    }
                }
    }
    
    return mgraph;
} //CreateMGFromALGV1

Airport FirstAdjPort(ALGraph algraph, Airport airport1)
{
    Airport airport2=-1;
    if(algraph.vertices[airport1].firstarc!=NULL)
        airport2=algraph.vertices[airport1].firstarc->arrival;
    return airport2;
} //FirstAdjPort

Airport NextAdjPort(ALGraph algraph, Airport airport1, Airport airport2)
{
    Airport airport3=-1;
    ArcNode * pn = algraph.vertices[airport1].firstarc;
    for(;pn!=NULL;pn=pn->nextarc)
    {
        if(pn->arrival==airport2)
            break;
    }
    for(;pn!=NULL;pn=pn->nextarc)
    {
        if(pn->arrival!=airport2)
        {
            airport3=pn->arrival;
            break;
        }
    }
    return airport3;
} //NextAdjPort

Status DFSTALGraph(ALGraph algraph, Airport departure)
{   //利用栈从任意结点出发实现深度优先遍历
    int visited[MAX_VERTEX_NUM]={-1};
    for(int v=1;v<=algraph.vexnum;++v) visited[v]=0;  //访问标志数组初始化
    Stack S; InitStack(&S);
    visited[departure]=1;
    printf("Airport No.%d\n",departure);
    Push(&S, departure);
    Airport port;
    Airport w;
    while(StackEmpty(S)==0)
    {
        GetTop(&S, &port);
        for(w=FirstAdjPort(algraph, port);w>=0;w=NextAdjPort(algraph, port, w))
            if(visited[w]==0)
            {
                visited[w]=1;Push(&S, w);printf("Airport No.%d\n",w);
                break;
            }
        if(w<0) Pop(&S, &port);
    }
    return OK;
} //DFTALGraph

Status BFSTALGraph(ALGraph algraph, int departure)
{   //利用队列从任意结点出发实现广度优先遍历
    int visited[MAX_VERTEX_NUM]={-1};
    for(int v=1;v<=algraph.vexnum;++v) visited[v]=0;  //访问标志数组初始化
    LkQueue LQ;InitLkQueue(&LQ);
    visited[departure]=1;
    printf("Airport No.%d\n",departure);
    EnQueue(&LQ, departure);
    Airport port;
    Airport w;
    while(QueueEmpty(LQ)==0&&GetHead(&LQ, &port))
    {
        for(w=FirstAdjPort(algraph, port);w>=0;w=NextAdjPort(algraph, port, w))
            if(visited[w]==0)
            {
                visited[w]=1;EnQueue(&LQ, w);printf("Airport No.%d\n",w);
                break;
            }
        if(w<0) DeQueue(&LQ, &port);
    }
    return OK;
} //BFTALGraph

Status PrintMGraph(MGraph * M)
{
    for(int i=1;i<=M->vexnum;i++)
    {
        for(int j=1;j<=M->vexnum;j++)
            printf("%d ",M->arcs[i][j].adj);
        printf("\n");
    }
    return OK;
} //PrintMGraph

Status CheckRoute(MGraph * M, Airport Departure, Airport Arrival)
{
    int connected=M->arcs[Departure][Arrival].adj;
    if(connected==1)
        printf("Airport No.%d-->Airport No.%d: Direct Flight\n",Departure,Arrival);
    else if(connected==2)
        printf("Airport No.%d-->Airport No.%d: One Transfer\n",Departure,Arrival);
    else if(connected==3)
        printf("Airport No.%d-->Airport No.%d: Two Transfers\n",Departure,Arrival);
    else if(connected==-1)
        printf("Airport No.%d-->Airport No.%d: No Flight\n",Departure,Arrival);
    return OK;
} //CheckRoute

#endif /* Graph_h */
