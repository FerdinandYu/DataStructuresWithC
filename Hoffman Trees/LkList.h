//
//  LkList.h
//  Trees
//
//  Created by 余宏昊 on 2018/11/30.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef LkList_h
#define LkList_h

#define OK     1
#define ERROR  0

#include <string.h>

typedef int status;
typedef struct Node {
    int         freq;       //每个字符出现的频次
    char       _char;       //字符
    char   _code[20];       //赫夫曼前缀编码
    int       length;       //字符的路径长度
    struct Node   *next;
} LNode;
typedef struct {
    LNode    *head;     //头指针
    int    numChar;     //字符数
} MsgInfo;              //短文信息

//-------------------------------------------------------------//
status InitMsgInfo(MsgInfo * Msg);
    //初始化
LNode * CreateNode(char _char);
    //创建结点
status ReadMsg(char * Msg, MsgInfo * MsgInfo);
    //读取文本，构建单链表以存储文本相关信息
status Sort(MsgInfo MsgInfo1, MsgInfo * MsgInfo2);
    //将单链表按升序排序
status GetSortedMsg(char * message, MsgInfo * SortedMsg);
    //获取排好序的单链表
status PrintMsgInfo(MsgInfo Info);
    //打印文本中各字符及其出现的频次
//-------------------------------------------------------------//

status InitMsgInfo(MsgInfo * Msg)
{   //初始化
    Msg->head=(LNode *)malloc(sizeof(LNode));
    if(!Msg->head) return ERROR;
    Msg->head->next=NULL;
    Msg->numChar=0;
    return OK;
} //InitMsgInfo

LNode * CreateNode(char _char)
{   //创建结点
    LNode * pNew=(LNode *)malloc(sizeof(LNode));
    if(!pNew) return ERROR;
    pNew->_char=_char;
    pNew->freq=1;
    memset(pNew->_code,0,sizeof(pNew->_code));
    pNew->length=0;
    pNew->next=NULL;
    return pNew;
} //CreateNode

status ReadMsg(char * Msg, MsgInfo * MsgInfo)
{   //读取短文，建立单链表以存储每个字符出现的频次
    int MsgLen=strlen(Msg);
    //printf("The length of the message is %d\n",MsgLen);
    char * p=Msg;    //p用来指示每个字符的地址
    //InitMsgInfo(MsgInfo);
    LNode * pInfo=MsgInfo->head->next;
    LNode * pre=MsgInfo->head;
    int match=0;
    for(int i=0;i<MsgLen;i++)
    {
        //printf("Reading Character: %c\n",*p);
        pInfo=MsgInfo->head->next;
        pre=MsgInfo->head;
        if(MsgInfo->numChar==0)   //如果已读取的字符数为0，则创建新结点，读入字符
        {
            LNode * pNew=CreateNode(*p);
            MsgInfo->head->next=pNew;
            //printf("Inserted New\n");
            MsgInfo->numChar+=1;
            ++p;
        } //if
        else      //否则，与已有的字符进行匹配，若匹配上，则将字符频次加一
        {
            match=0;
            for(;pInfo!=NULL;pInfo=pInfo->next)
            {
                if(*p==pInfo->_char)
                {
                    pInfo->freq+=1;
                    ++match;
                }
                pre=pre->next;
            }
            if(pInfo==NULL&&match==0)   //如果遍历完单链表仍不匹配，则创建新结点，读入字符
            {
                LNode * pNew=CreateNode(*p);
                pre->next=pNew;
                //printf("Inserted New\n");
                MsgInfo->numChar+=1;
            }
            ++p;  //读取下一个字符
        } //else
    } //for1
    return OK;
} //ReadMsg

status Sort(MsgInfo MsgInfo1, MsgInfo * MsgInfo2)
{   //根据MsgInfo1中各字符出现的频次大小进行升序排序，MsgInfo1为待排序的链表，MsgInfo2用来存放排好序的链表
    LNode * pTemp1=MsgInfo1.head->next;
    LNode * pTemp2=MsgInfo2->head;
    for(;pTemp1!=NULL;pTemp1=pTemp1->next)
    {
        LNode * pNew=CreateNode(pTemp1->_char);
        pNew->freq=pTemp1->freq;
        //printf("Inserting %c-%d\n",pNew->_char,pNew->freq);
        if(MsgInfo2->numChar==0)
        {
            MsgInfo2->head->next=pNew;
            MsgInfo2->numChar+=1;
            //printf("A.Insertd New into New:%c-%d\n",pNew->_char,pNew->freq);
        } //if
        else
        {
            pTemp2=MsgInfo2->head;
            for(;pTemp2->next!=NULL;pTemp2=pTemp2->next)
            {
                //printf("Looping\n");
                if(pNew->freq<=pTemp2->next->freq)
                {
                    //printf("YES-1\n");
                    pNew->next=pTemp2->next;
                    pTemp2->next=pNew;
                    MsgInfo2->numChar+=1;
                    //printf("B.Insertd New into New:%c-%d\n",pNew->_char,pNew->freq);
                    break;
                } //if
            } //for
            if(pTemp2->next==NULL&&pNew->freq>pTemp2->freq)
            {
                pTemp2->next=pNew;
                MsgInfo2->numChar+=1;
                //printf("C.Insertd New into New:%c-%d\n",pNew->_char,pNew->freq);
            } //if
        } //else
    } //for
    return OK;
}

status GetSortedMsg(char * message, MsgInfo * SortedMsg)
{   //将存入到MsgInfo里的字符按照出现频次升序排序
    InitMsgInfo(SortedMsg);
    MsgInfo temp;
    InitMsgInfo(&temp);
    ReadMsg(message, &temp);
    Sort(temp, SortedMsg);
    return OK;
} //GetSortedMsg

status PrintMsgInfo(MsgInfo Info)
{   //打印文本中各字符及其出现的频次
    LNode * pTemp = Info.head->next;
    int len=Info.numChar;
    if(len==0)
        return ERROR;
    else
    {
        while(pTemp!=NULL)
        {
            printf("%c:%d\n",pTemp->_char,pTemp->freq);
            pTemp = pTemp->next;
        }
    }
    return OK;
} //PrintMsgInfo

#endif /* LkList_h */
