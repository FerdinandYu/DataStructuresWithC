//
//  Stack_Queue.h
//  Airlines Plan
//
//  Created by 余宏昊 on 2018/12/23.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef Stack_Queue_h
#define Stack_Queue_h

#define STACK_INIT_SIZE 100      //栈的初始大小
#define STACKINCREMENT  10       //栈的增量大小

typedef struct {
    Airport    * base;
    Airport    *  top;
    int     stacksize;
    int           num;
} Stack;

typedef struct QNode {
    Airport     airport;
    struct QNode * next;
} QNode;
typedef struct {
    QNode        * head;
    QNode        * tail;
    int          length;
} LkQueue;

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
Status InitStack(Stack * S);
    //初始化栈
Status DestroyStack(Stack * S);
    //销毁栈
Status Push(Stack * S, Airport airport);
    //压栈
Status Pop(Stack * S, Airport * airport);
    //弹栈
Status GetTop(Stack * S, Airport * airport);
    //获取栈顶元素
int StackEmpty(Stack S);
    //判空
Status Traverse(Stack * S);
    //遍历

Status InitLkQueue(LkQueue * LQ);
    //初始化链式队列
Status DestroyLkQueue(LkQueue * LQ);
    //销毁链式队列
Status EnQueue(LkQueue * LQ, Airport airport);
    //入列
Status DeQueue(LkQueue * LQ, Airport * airport);
    //出列
Status GetHead(LkQueue * LQ, Airport * airport);
    //获取队列头元素
int QueueEmpty(LkQueue LQ);
    //判空
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//

Status InitStack(Stack * S)
{
    S->base = (Airport *)malloc(STACK_INIT_SIZE*sizeof(Airport));
    if(! S->base) exit(-2);
    S->top=S->base;
    S->stacksize=STACK_INIT_SIZE;
    S->num=0;
    return OK;
} //InitStack

Status DestroyStack(Stack * S)
{
    free(S);
    return OK;
} //Destroy

Status Push(Stack * S, Airport airport)
{
    if((S->top-S->base)>=S->stacksize)   //如果栈满，追加存储空间
    {
        S->base=(Airport *)realloc(S->base,(S->stacksize + STACKINCREMENT)*sizeof(Airport));
        if(!S->base) exit(-2);
        S->top=S->base+S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *(S->top) = airport;     //将栈顶元素设为airport
    S->top++;          //将栈顶指针向上挪动
    S->num++;
    return OK;
} //Push

Status Pop(Stack * S, Airport * airport)
{
    if(S->top==S->base) return ERROR;
    S->top--;          //将栈顶指针向下挪动
    *airport = *(S->top);
    S->num--;
    return OK;
} //Pop

Status GetTop(Stack * S, Airport * airport)
{
    *airport = *(S->top-1);
    return OK;
} //GetTop

int StackEmpty(Stack S)
{
    if(S.base==S.top)
        return 1;
    else
        return 0;
} //StackEmpty

Status Traverse(Stack *S)
{
    int len=S->num;
    Airport *dot=S->base;
    for(int i=0;i<len;i++)
    {
        printf("%d ",*dot);
        ++dot;
    }
    printf("\n");
    return OK;
} //Traverse

Status InitLkQueue(LkQueue * LQ)
{
    LQ->length=0;
    LQ->head=(QNode *)malloc(sizeof(QNode));
    LQ->head->next=NULL;
    LQ->tail=LQ->head;
    return OK;
} //InitLkQueue

Status DestroyLkQueue(LkQueue * LQ)
{
    QNode * pn = LQ->head;
    for(;LQ->head->next!=NULL;)
    {
        pn=LQ->head->next;
        LQ->head->next=LQ->head->next->next;
        free(pn);
    }
    free(LQ->head);
    free(LQ);
    return OK;
} //DestroyLkQueue

Status EnQueue(LkQueue * LQ, Airport airport)
{
    QNode * newdot = (QNode *)malloc(sizeof(QNode));
    newdot->airport=airport;
    newdot->next=NULL;
    
    if(LQ->head->next==NULL)
    {
        LQ->head->next=newdot;
        LQ->tail=newdot;
    }
    else
    {
        LQ->tail->next=newdot;
        LQ->tail=newdot;
    }
    LQ->length++;
    return OK;
} //EnQueue

Status DeQueue(LkQueue * LQ, Airport * airport)
{
    if(LQ->head->next==NULL)
        return ERROR;
    else
    {
        QNode * pn=LQ->head->next;
        LQ->head->next=LQ->head->next->next;
        *airport=pn->airport;
        free(pn);
        LQ->length--;
    }
    return OK;
} //DeQueue

Status GetHead(LkQueue * LQ, Airport * airport)
{
    if(LQ->head->next!=NULL)
    {
        *airport=LQ->head->next->airport;
        return OK;
    }
    else
        return ERROR;
} //GetHead

int QueueEmpty(LkQueue LQ)
{
    if(LQ.head==LQ.tail)
        return 1;
    else
        return 0;
} //QueueEmpty

#endif /* Stack_Queue_h */
