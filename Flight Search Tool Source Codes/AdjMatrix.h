//
//  AdjMatrix.h
//  Airlines Plan
//
//  Created by 余宏昊 on 2018/12/19.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef AdjMatrix_h
#define AdjMatrix_h

typedef int VRType;
typedef char InfoType;
typedef int VertexType;
typedef struct ArcCell {
    VRType  adj;    //-1表示不连通，1表示只能直飞，2表示1次中转，3表示2次中转
    int   transfers[MAX_VERTEX_NUM];    //中转组合
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
typedef struct {
    VertexType   vexs[MAX_VERTEX_NUM];   //顶点向量
    AdjMatrix    arcs;                   //邻接矩阵
    int          vexnum,arcnum;          //图的当前顶点数和弧数
    GraphKind    kind;                   //图的种类标志
} MGraph;

//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//
Status InitArcCell(ArcCell * arccell);
    //初始化弧
Status InitMGraph(MGraph * mgraph);
    //初始化邻接矩阵
//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//

Status InitArcCell(ArcCell * arccell)
{
    arccell->adj=-1;
    //memset(arccell->transfers,0,sizeof(arccell->transfers));
    for(int i=0;i<MAX_VERTEX_NUM;i++)
        arccell->transfers[i]=0;
    return OK;
} //InitArcCell

Status InitMGraph(MGraph * mgraph)
{
    for(int i=0;i<MAX_VERTEX_NUM;++i)
        mgraph->vexs[i]=0;
    mgraph->vexnum=mgraph->arcnum=0;
    mgraph->kind=DN;
    for(int i=0;i<MAX_VERTEX_NUM;++i)
        for(int j=0;j<MAX_VERTEX_NUM;++j)
            InitArcCell(&mgraph->arcs[i][j]);
    return OK;
} //InitMGraph


#endif /* AdjMatrix_h */
