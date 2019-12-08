//
//  Variables.h
//  NewTest
//
//  Created by 余宏昊 on 2018/11/16.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef Variables_h
#define Variables_h

#include <string.h>
#include <math.h>

#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define INFEASIBLE -1

#define VarListNum 20

typedef int Status;
typedef struct {
    char   VarName[20];   //变量名
    int     IsAssigned;   //是否已经被赋值
    float       VarVal;   //函数值
} Var;                    //变量
typedef struct {
    Var    *VarList[VarListNum];   //用一组连续的存储单元存储一组Var
    int                  NumVar;   //变量个数
} Vars;                            //变量库

//----------------------------------------------------------------------//
Status InitVars(Vars * Vars);
    //初始化变量数组
Status InitVar(Var * Var);
    //初始化变量
int Max(int a, int b);
    //求两数中的较大值
float ConvertNum(char * Num);
    //将字符串转化为数字
float AssignVar(Vars * Vars, char * var);
    //为给定变量分配值
//----------------------------------------------------------------------//

Status InitVars(Vars * Vars)
{   //为变量数组分配存储空间，并将变量个数设置为1
    Vars->NumVar=0;
    memset(Vars->VarList, 0, sizeof(Vars->VarList));
    for(int i=0;i<VarListNum;++i)
    {
        Var * newVar=(Var *)malloc(sizeof(Var));
        if(!newVar) return ERROR;
        InitVar(newVar);
        Vars->VarList[i]=newVar;
    }
    return OK;
} //InitVars

Status InitVar(Var * Var)
{   //为变量分配存储空间，设定其为未分配值，将其初始值设定为0
    Var->IsAssigned=0;
    Var->VarVal=0.0;
    memset(Var->VarName,0,sizeof(Var->VarName));
    return OK;
} //InitVar

int Max(int a, int b)
{   //求两数中的较大值
    int c;
    if(a>=b) c=a;
    else     c=b;
    return c;
} //Max

float ConvertNum(char * Num)
{   //将字符串转化为数字
    unsigned long len=strlen(Num);
    char *c=Num; int b;
    int temp[30]={0};int temp_pos=0;    //用于存放数字部分
    float sum=0;
    int IsDot=0,DotPos=0; //计算是否出现小数点，并记录其位置
    int IsMinus=0,MinusPos=0; //记录是否出现负号，并记录最末尾的负号的位置
    int IsE=0,EPos=0; //记录是否出现e或E，并记录其位置
    int IsNegative=1;  //用于最后处理正负，默认为1，若为负则为-1
    int TenPower=0; int EPower=0;//
    
    //对数字字符串进行预处理，得到IsMinus/MinusPos,IsDot/DotPos,IsE/EPos,digit
    for(int i=1;i<len+1;++i)
    {
        if(*c=='-')
        {
            IsMinus+=1;MinusPos=i;
        }
        else if(*c=='.')
        {
            IsDot+=1;DotPos=i;
        }
        else if(*c=='e'||*c=='E')
        {
            IsE+=1;EPos=i;
        }
        else if(isdigit(*c))
        {
            b=*c-'0';temp[temp_pos]=b;++temp_pos;
        }
        ++c;
    }
    
    //转化为数字
    if((IsMinus==1&&MinusPos==1)||IsMinus==2) //转换正负
    {IsNegative=-1;}
    if(IsE==0) //非科学计数法表示的数
    {
        //printf("RUNNING CONVERTNUM A\n");
        for(int pos=0;pos<len-IsDot-IsMinus;++pos)
        {
            sum*=10.0;sum+=temp[pos];
        }
        if(IsDot==1) TenPower=DotPos-len;
    }
    else if(IsE==1) //科学计数法表示的数
    {   //
        //printf("RUNNING CONVERTNUM B\n");
        for(int pos=0;pos<EPos-IsDot-ceil(IsMinus/2.0)-1;++pos)
        {
            sum*=10.0;sum+=temp[pos];
        }
        if(IsDot==1) TenPower+=DotPos-EPos+1;
        for(int pos=len-IsMinus-IsDot-IsE-1;pos>EPos-IsDot-ceil(IsMinus/2.0)-2;--pos)
        {EPower*=10;EPower+=temp[pos];}
        if(MinusPos<=1)
        {TenPower+=EPower;}
        else if(MinusPos>1)
        {TenPower-=EPower;}
    }
    sum=IsNegative*sum*pow(10, TenPower);
    return sum;
} //ConvertNum

float AssignVar(Vars * Vars, char * var)
{   //为给定变量分配值
    int pos=0;int IsAssigned=0;
    float val=0.0,output=0.0;
    char input[15];
    memset(input,0,sizeof(input));
    
    if(Vars->NumVar==0)   //变量库为空
    {
        //printf("RUNNING ASSIGNVAR A\n");
        strcpy(Vars->VarList[0]->VarName, var);
        printf("Enter a value to define %s:\n",Vars->VarList[0]->VarName);
        scanf("%s",input);
        val=ConvertNum(input);
        printf("Input: %f\n",val);
        Vars->VarList[0]->VarVal=val;
        Vars->VarList[0]->IsAssigned=1;
        Vars->NumVar+=1;
        output=val;
    }
    else
    {
        //printf("RUNNING ASSIGNVAR B\n");
        for(pos=0;pos<Vars->NumVar;++pos)
        {
            //printf("%s %s\n",var,Vars->VarList[pos]->VarName);
            if(strcmp(var, Vars->VarList[pos]->VarName)==0) //如果该变量已经出现过
            {
                //printf("RUNNING ASSIGNVAR B-1\n");
                output=Vars->VarList[pos]->VarVal;
                IsAssigned=1;
            }
            else IsAssigned=0;
        }
        //printf("%d\n",pos);
        if(IsAssigned==0)
        {
            //printf("RUNNING ASSIGNVAR B-2\n");
            strcpy(Vars->VarList[Vars->NumVar]->VarName, var);
            printf("Enter a value to define %s:\n",Vars->VarList[Vars->NumVar]->VarName);
            scanf("%s",input);
            val=ConvertNum(input);
            printf("Input: %f\n",val);
            Vars->VarList[Vars->NumVar]->VarVal=val;
            Vars->VarList[Vars->NumVar]->IsAssigned=1;
            Vars->NumVar+=1;
            output=val;
        }
    }
    return output;
} //AssignVar

#endif /* Variables_h */
