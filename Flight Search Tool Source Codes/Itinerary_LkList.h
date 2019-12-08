//
//  SqList.h
//  Airlines Plan
//
//  Created by 余宏昊 on 2018/12/19.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef SqList_h
#define SqList_h

#include "Graph_Operations.h"
#include <math.h>

typedef int Status;

#define LIST_INIT_SIZE 100  //线性表存储空间的初始分配量
#define LISTINCREMENT  10   //线性表存储空间的分配增量
#define ID_LIST_SIZE   50

typedef struct Itis {
    flight_record    * first;   //第一段航程，若没有则为NULL
    flight_record   * second;   //第二段航程，若没有则为NULL
    flight_record    * third;   //第三段航程，若没有则为NULL
    int          num_flights;   //整个航线的航程数
    struct Itis       * next;   //下一个航线
} FullIti;
typedef struct {
    FullIti           * head;
    int       num_options[4];   //存放所有航线的数量信息，num_options[0]为总航线数，其他下标代表航程数
} Options;

//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//
Status InitFullIti(FullIti * fulliti);
    //初始化航线
Status InitOptions(Options * options);
    //初始化存放航线的链表
flight_record * CreateFlight(flight_record * record);
    //复制所给的航班信息，并创建一个新的航班记录
FullIti * CreateFullIti(flight_record * first, flight_record * second, flight_record * third);
    //根据所给的航程构建航线信息
Status PushHeadOps(Options * options, flight_record * first, flight_record * second, flight_record * third);
    //头插：插入航线信息
Status PushTailOps(Options * options, flight_record * first, flight_record * second, flight_record * third);
    //尾插：插入航线信息
Status AddIti(Options * options, FullIti * iti);
    //将航线按第一段航程航班ID升序插入到链表中
int GetShortestDuration(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival);
    //获取两个机场之间的最短飞行时间
Status QuickestFlight(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival);
    //将最短飞行时间进行格式化输出
Status AllRoutesV1(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival, Options * Choices);
    //获取连同两个机场的所有航线
Status PrintOptions(Options options, int num_flights);
    //根据航程数输出航线
Status LimitTransRouteV1(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival, int Transfers);
    //限定转机次数，输出航线
int GetCondition(int DepartureTime1, int DepartureTime2, int ArrivalTime1, int ArrivalTime2, int AirplaneModel, int FlightDuration, int TransferDuration);
    //判断用户输入的筛选条件
int ConditionSatisfiedV1(FullIti * iti, int DepartureTime1, int DepartureTime2, int ArrivalTime1, int ArrivalTime2, int AirplaneModel, int FlightDuration, int TransferDuration, int Condition);
    //判断所给航线是否符合用户输入的筛选条件
int AirFares(FullIti * iti);
    //计算所给航线的总花费
Status PrintFullIti(FullIti * iti);
    //打印输出航线
Status ProperRouteV1(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival, int DepartureTime1, int DepartureTime2, int ArrivalTime1, int ArrivalTime2, int AirplaneModel, int FlightDuration, int TransferDuration, int IS_MIN_COST);
    //根据用户输入的各种筛选条件找出符合条件的航线
int ConvertTS(char * text, int type);
    //将用户输入的时间戳转化为整数
Status GetOrder(MGraph M, Airport * Departure, Airport * Arrival, int * DepartureTime1, int * DepartureTime2, int * ArrivalTime1, int * ArrivalTime2, int * AirplaneModel, int * FlightDuration, int * TransferDuration, int * IS_MIN_COST);
    //获取用户输入（筛选条件）
//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//

Status InitFullIti(FullIti * fulliti)
{
    fulliti->first=NULL;
    fulliti->second=NULL;
    fulliti->third=NULL;
    fulliti->next=NULL;
    fulliti->num_flights=0;
    return OK;
} //InitFullIti

Status InitOptions(Options * options)
{
    options->head=(FullIti *)malloc(sizeof(FullIti));
    InitFullIti(options->head);
    for(int i=0;i<4;i++)
        options->num_options[i]=0;
    return OK;
} //InitOptions

flight_record * CreateFlight(flight_record * record)
{
    flight_record * flight = (flight_record *)malloc(sizeof(flight_record));
    if(!flight) exit(-2);
    flight->flight_id=record->flight_id;
    strcpy(flight->depart_date, record->depart_date);
    strcpy(flight->flight_type, record->flight_type);
    flight->flight_no=record->flight_no;
    flight->departure=record->departure;
    flight->arrival=record->arrival;
    strcpy(flight->departure_time, record->departure_time);
    strcpy(flight->arrival_time, record->arrival_time);
    flight->airplane_id=record->airplane_id;
    flight->airplane_model=record->airplane_model;
    flight->airfares=record->airfares;
    flight->departure_time1=record->departure_time1;
    flight->arrival_time1=record->arrival_time1;
    return flight;
} //CreateFlight

FullIti * CreateFullIti(flight_record * first, flight_record * second, flight_record * third)
{
    FullIti * fulliti=(FullIti *)malloc(sizeof(FullIti));
    if(!fulliti) exit(-2);
    fulliti->num_flights=0;
    fulliti->first=first;
    fulliti->second=second;
    fulliti->third=third;
    fulliti->next=NULL;
    if(first!=NULL) fulliti->num_flights+=1;
    if(second!=NULL) fulliti->num_flights+=1;
    if(third!=NULL) fulliti->num_flights+=1;
    return fulliti;
} //CreateFullIti

Status PushHeadOps(Options * options, flight_record * first, flight_record * second, flight_record * third)
{
    FullIti * fulliti=CreateFullIti(first, second, third);
    if(fulliti->num_flights>0)
    {
        if(options->num_options[0]==0)
            options->head->next=fulliti;
        else
        {
            fulliti->next=options->head->next;
            options->head->next=fulliti;
        }
    
        options->num_options[fulliti->num_flights]++;
        options->num_options[0]++;
    }
    return OK;
} //PushHeadOps

Status PushTailOps(Options * options, flight_record * first, flight_record * second, flight_record * third)
{
    FullIti *pn = options->head;       //找到最后一个结点
    while (pn->next != NULL)
    {
        pn = pn->next;
    }
    FullIti * iti=CreateFullIti(first,second,third);
    if(iti->num_flights>0)
    {
        pn->next = iti;
        options->num_options[iti->num_flights]++;
        options->num_options[0]++;
    }
    return OK;
} //PushTailOps

Status AddIti(Options * options, FullIti * iti)
{
    if(options->num_options[0]==0)
        PushHeadOps(options, iti->first, iti->second, iti->third);
    else
    {
        int first_id = iti->first->flight_id;
        FullIti * pm = options->head;
        if(iti->num_flights==2||iti->num_flights==3)
            for(int i=0;i<options->num_options[1];i++)
                pm=pm->next;
        if(iti->num_flights==3)
            for(int i=0;i<options->num_options[2];i++)
                pm=pm->next;
        if(iti->num_flights==1)
        {
            int i=0;
            for(;i<options->num_options[1];i++)
            {
                if(first_id<=pm->next->first->flight_id)
                {
                    iti->next=pm->next;
                    pm->next=iti;
                    options->num_options[iti->num_flights]++;
                    break;
                }
                else
                    pm=pm->next;
            }
            if(i==options->num_options[1])
            {
                iti->next=pm->next;
                pm->next=iti;
                options->num_options[iti->num_flights]++;
            }
        }
        else if(iti->num_flights==2)
        {
            int j=0;
            for(;j<options->num_options[2];j++)
            {
                if(first_id<=pm->next->first->flight_id)
                {
                    iti->next=pm->next;
                    pm->next=iti;
                    options->num_options[iti->num_flights]++;
                    break;
                }
                else
                    pm=pm->next;
            }
            if(j==options->num_options[2])
            {
                iti->next=pm->next;
                pm->next=iti;
                options->num_options[iti->num_flights]++;
            }
        }
        else if(iti->num_flights==3)
        {
            int k=0;
            for(;k<options->num_options[3];k++)
            {
                if(first_id<=pm->next->first->flight_id)
                {
                    iti->next=pm->next;
                    pm->next=iti;
                    options->num_options[iti->num_flights]++;
                    break;
                }
                else
                    pm=pm->next;
            }
            if(k==options->num_options[3])
            {
                iti->next=pm->next;
                pm->next=iti;
                options->num_options[iti->num_flights]++;
            }
        }
        options->num_options[0]++;
    }
    return OK;
} //AddIti

int GetShortestDuration(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival)
{
    int shortest_duration=1000000;
    int duration=1000000;
    int connected=M->arcs[Departure][Arrival].adj;
    
    int ID1[ID_LIST_SIZE]={0};
    int ID2[ID_LIST_SIZE]={0};
    int ID3[ID_LIST_SIZE]={0};
    
    if(connected==1)  //直飞
    {
        GetFlightID(schedule, Departure, Arrival, ID1);
        int ID=ID1[1];
        flight_record * pn=GetRecord(schedule, ID);
        shortest_duration=pn->arrival_time1-pn->departure_time1;
    }
    else if(connected==2)  //转一次机
    {
        for(int i=0;M->arcs[Departure][Arrival].transfers[i]!=0;i++)    //转机机场
        {
            Airport Transfer=M->arcs[Departure][Arrival].transfers[i];
            GetFlightID(schedule, Departure, Transfer, ID1);
            GetFlightID(schedule, Transfer, Arrival, ID2);
            for(int i=1;i<=ID1[0];i++)
            {
                flight_record * flight1=GetRecord(schedule, ID1[i]);
                for(int j=1;j<=ID2[0];j++)
                {
                    flight_record * flight2=GetRecord(schedule, ID2[j]);
                    if(flight1->arrival_time1<flight2->departure_time1)
                    {
                        duration=flight2->arrival_time1-flight1->departure_time1;
                    }
                    if(duration<shortest_duration)
                        shortest_duration=duration;
                }
            }
        }
    }
    else if(connected==3)  //转两次机
    {
        for(int i=0;M->arcs[Departure][Arrival].transfers[i]!=0;i++)    //转机机场
        {
            Airport Transfers=M->arcs[Departure][Arrival].transfers[i];
            Airport Transfer1=(int)floor(Transfers/100);
            Airport Transfer2=(int)Transfers%100;
            GetFlightID(schedule, Departure, Transfer1, ID1);
            GetFlightID(schedule, Transfer1, Transfer2, ID2);
            GetFlightID(schedule, Transfer2, Arrival,   ID3);
            for(int i=1;i<=ID1[0];i++)
            {
                flight_record * flightA=GetRecord(schedule, ID1[i]);
                for(int j=1;j<=ID2[0];j++)
                {
                    flight_record * flightB=GetRecord(schedule, ID2[j]);
                    for(int k=1;k<=ID3[0];k++)
                    {
                        flight_record * flightC=GetRecord(schedule, ID3[k]);
                        if(flightA->arrival_time1<flightB->departure_time1&&flightB->arrival_time1<flightC->departure_time1)
                        {
                            duration=flightC->arrival_time1-flightA->departure_time1;
                            if(duration<shortest_duration)
                                shortest_duration=duration;
                        }
                    }
                }
            }
        }
    }
    return shortest_duration;
} //GetShortestDuaration

Status QuickestFlight(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival)
{
    int duration = GetShortestDuration(schedule, M, Departure, Arrival);
    int hours = (int)floor(duration/60);
    int minutes = (int)duration%60;
    printf("Airport No.%d-->Airport No.%d: %d h %d min (Shortest Duration)\n",Departure,Arrival,hours,minutes);
    return OK;
} //QuickestFlight

Status AllRoutesV1(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival, Options * Choices)
{
    flight_record * flight1 = NULL;
    flight_record * flight2 = NULL;
    flight_record * flight3 = NULL;
    flight_record * flightI = NULL;
    flight_record * flightII = NULL;
    flight_record * flightIII = NULL;
    if(M->arcs[Departure][Arrival].adj==1)    //可以直飞
    {
        int A_ID1[ID_LIST_SIZE]={0};
        GetFlightID(schedule, Departure, Arrival, A_ID1);
        for(int i=1;i<=A_ID1[0];i++)            //直飞路径
        {
            flight1 = GetRecord(schedule, A_ID1[i]);
            flightI = CreateFlight(flight1);
            FullIti * fulliti = CreateFullIti(flightI, flightII, flightIII);
            AddIti(Choices, fulliti);
            flightI=flightII=flightIII=NULL;
        }
        
        for(int i=1;i<=M->vexnum;i++)         //转一次机的路径
            if(Departure!=i&&i!=Arrival&&Departure!=Arrival&&M->arcs[Departure][i].adj==1&&M->arcs[i][Arrival].adj==1)
            {
                int A_IDa[ID_LIST_SIZE]={0};
                int A_IDb[ID_LIST_SIZE]={0};
                GetFlightID(schedule, Departure, i, A_IDa);
                GetFlightID(schedule, i, Arrival, A_IDb);
                for(int j=1;j<=A_IDa[0];j++)
                {
                    flight1=GetRecord(schedule, A_IDa[j]);
                    flightI = CreateFlight(flight1);
                    for(int k=1;k<=A_IDb[0];k++)
                    {
                        flight2=GetRecord(schedule, A_IDb[k]);
                        flightII = CreateFlight(flight2);
                        if(flightI->arrival_time1<flightII->departure_time1)
                        {
                            FullIti * fulliti = CreateFullIti(flightI, flightII, flightIII);
                            AddIti(Choices, fulliti);
                        }
                        flightII=NULL;
                    }
                    flightI=NULL;
                }
            }
        
        for(int i=1;i<=M->vexnum;i++)         //转两次机的路径
            for(int j=1;j<=M->vexnum;j++)
                if(Departure!=i&&i!=j&&j!=Arrival&&i!=Arrival&&Departure!=j&&Departure!=Arrival&&M->arcs[Departure][i].adj==1&&M->arcs[i][j].adj==1&&M->arcs[j][Arrival].adj==1)
                {
                    int A_IDI[ID_LIST_SIZE]={0};
                    int A_IDII[ID_LIST_SIZE]={0};
                    int A_IDIII[ID_LIST_SIZE]={0};
                    GetFlightID(schedule, Departure, i, A_IDI);
                    GetFlightID(schedule, i, j, A_IDII);
                    GetFlightID(schedule, j, Arrival, A_IDIII);
                    for(int i=1;i<=A_IDI[0];i++)
                    {
                        flight1=GetRecord(schedule, A_IDI[i]);
                        flightI = CreateFlight(flight1);
                        for(int j=1;j<=A_IDII[0];j++)
                        {
                            flight2=GetRecord(schedule, A_IDII[j]);
                            flightII = CreateFlight(flight2);
                            for(int k=1;k<=A_IDIII[0];k++)
                            {
                                flight3=GetRecord(schedule, A_IDIII[k]);
                                flightIII = CreateFlight(flight3);
                                if(flightI->arrival_time1<flightII->departure_time1&&flightII->arrival_time1<flightIII->departure_time1)
                                {
                                    FullIti * fulliti = CreateFullIti(flightI, flightII, flightIII);
                                    AddIti(Choices, fulliti);
                                }
                                flightIII=NULL;
                            }
                            flightII=NULL;
                        }
                        flightI=NULL;
                    }
                }
    }
    
    else if(M->arcs[Departure][Arrival].adj==2)    //需要转一次机
    {
        for(int i=1;i<=M->vexnum;i++)         //转一次机的路径
            if(Departure!=i&&i!=Arrival&&Departure!=Arrival&&M->arcs[Departure][i].adj==1&&M->arcs[i][Arrival].adj==1)
            {
                int B_IDa[ID_LIST_SIZE]={0};
                int B_IDb[ID_LIST_SIZE]={0};
                GetFlightID(schedule, Departure, i, B_IDa);
                GetFlightID(schedule, i, Arrival, B_IDb);
                for(int j=1;j<=B_IDa[0];j++)
                {
                    flight1=GetRecord(schedule, B_IDa[j]);
                    flightI = CreateFlight(flight1);
                    for(int k=1;k<=B_IDb[0];k++)
                    {
                        flight2=GetRecord(schedule, B_IDb[k]);
                        flightII = CreateFlight(flight2);
                        if(flightI->arrival_time1<flightII->departure_time1)
                        {
                            FullIti * fulliti = CreateFullIti(flightI, flightII, flightIII);
                            AddIti(Choices, fulliti);
                        }
                        flightII=NULL;
                    }
                    flightI=NULL;
                }
            }
        
        for(int i=1;i<=M->vexnum;i++)         //转两次机的路径
            for(int j=1;j<=M->vexnum;j++)
                if(Departure!=i&&i!=j&&j!=Arrival&&i!=Arrival&&Departure!=j&&Departure!=Arrival&&M->arcs[Departure][i].adj==1&&M->arcs[i][j].adj==1&&M->arcs[j][Arrival].adj==1)
                {
                    int B_IDI[ID_LIST_SIZE]={0};
                    int B_IDII[ID_LIST_SIZE]={0};
                    int B_IDIII[ID_LIST_SIZE]={0};
                    GetFlightID(schedule, Departure, i, B_IDI);
                    GetFlightID(schedule, i, j, B_IDII);
                    GetFlightID(schedule, j, Arrival, B_IDIII);
                    for(int i=1;i<=B_IDI[0];i++)
                    {
                        flight1=GetRecord(schedule, B_IDI[i]);
                        flightI = CreateFlight(flight1);
                        for(int j=1;j<=B_IDII[0];j++)
                        {
                            flight2=GetRecord(schedule, B_IDII[j]);
                            flightII = CreateFlight(flight2);
                            for(int k=1;k<=B_IDIII[0];k++)
                            {
                                flight3=GetRecord(schedule, B_IDIII[k]);
                                flightIII = CreateFlight(flight3);
                                if(flightI->arrival_time1<flightII->departure_time1&&flightII->arrival_time1<flightIII->departure_time1)
                                {
                                    FullIti * fulliti = CreateFullIti(flightI, flightII, flightIII);
                                    AddIti(Choices, fulliti);
                                }
                                flightIII=NULL;
                            }
                            flightII=NULL;
                        }
                        flightI=NULL;
                    }
                }
    }
    
    else if(M->arcs[Departure][Arrival].adj==3)    //需要转两次机
    {
        for(int i=1;i<=M->vexnum;i++)         //转两次机的路径
            for(int j=1;j<=M->vexnum;j++)
                if(Departure!=i&&i!=j&&j!=Arrival&&i!=Arrival&&Departure!=j&&Departure!=Arrival&&M->arcs[Departure][i].adj==1&&M->arcs[i][j].adj==1&&M->arcs[j][Arrival].adj==1)
                {
                    int C_IDI[ID_LIST_SIZE]={0};
                    int C_IDII[ID_LIST_SIZE]={0};
                    int C_IDIII[ID_LIST_SIZE]={0};
                    GetFlightID(schedule, Departure, i, C_IDI);
                    GetFlightID(schedule, i, j, C_IDII);
                    GetFlightID(schedule, j, Arrival, C_IDIII);
                    for(int i=1;i<=C_IDI[0];i++)
                    {
                        flight1=GetRecord(schedule, C_IDI[i]);
                        flightI = CreateFlight(flight1);
                        for(int j=1;j<=C_IDII[0];j++)
                        {
                            flight2=GetRecord(schedule, C_IDII[j]);
                            flightII = CreateFlight(flight2);
                            for(int k=1;k<=C_IDIII[0];k++)
                            {
                                flight3=GetRecord(schedule, C_IDIII[k]);
                                flightIII = CreateFlight(flight3);
                                if(flightI->arrival_time1<flightII->departure_time1&&flightII->arrival_time1<flightIII->departure_time1)
                                {
                                    FullIti * fulliti = CreateFullIti(flightI, flightII, flightIII);
                                    AddIti(Choices, fulliti);
                                }
                                flightIII=NULL;
                            }
                            flightII=NULL;
                        }
                        flightI=NULL;
                    }
                }
    }
    
    else if(M->arcs[Departure][Arrival].adj==-1)
        return ERROR;
    return OK;
} //AllRoutesV1

Status PrintOptions(Options options, int num_flights)
{
    FullIti * pn = options.head->next;
    if(num_flights==1)
    {
        if(options.num_options[1]==0)
            return ERROR;
        else
        {
            for(int i=0;i<options.num_options[1];i++)
            {PrintFlight(*pn->first);pn=pn->next;printf("\n");}
            return OK;
        }
    }
    else if(num_flights==2)
    {
        if(options.num_options[2]==0)
            return ERROR;
        else
        {
            if(options.num_options[1]>0)
                for(int i=0;i<options.num_options[1];i++)
                    pn=pn->next;
            for(int j=0;j<options.num_options[2];j++)
            {PrintFlight(*pn->first);PrintFlight(*pn->second);pn=pn->next;printf("\n");}
            return OK;
        }
    }
    else if(num_flights==3)
    {
        if(options.num_options[3]==0)
            return ERROR;
        else
        {
            if(options.num_options[1]>0)
                for(int i=0;i<options.num_options[1];i++)
                    pn=pn->next;
            if(options.num_options[2]>0)
                for(int j=0;j<options.num_options[2];j++)
                    pn=pn->next;
            for(int k=0;k<options.num_options[3];k++)
            {PrintFlight(*pn->first);PrintFlight(*pn->second);PrintFlight(*pn->third);pn=pn->next;printf("\n");}
            return OK;
        }
    }
    return OK;
} //PrintOptions

Status LimitTransRouteV1(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival, int Transfers)
{
    Options options;
    InitOptions(&options);
    
    AllRoutesV1(schedule, M, Departure, Arrival, &options);
    
    printf("\nFROM: Airport No.%d\t\tTO: Airport No.%d\t\t%d Transfer\n",Departure,Arrival,Transfers);
    printf("Flight NO.\tDeparture\tDeparture Time\tArrival\tArrival Time\tDome/Intl\tAirplane Model\tFares\n");
    PrintOptions(options, Transfers+1);
    return OK;
} //LimitTransRouteV1

int GetCondition(int DepartureTime1, int DepartureTime2, int ArrivalTime1, int ArrivalTime2, int AirplaneModel, int FlightDuration, int TransferDuration)
{
    int Condition=0;
    if(DepartureTime1>0&&DepartureTime2>0)
    {
        if(ArrivalTime1>0&&ArrivalTime2>0)
        {if(AirplaneModel>0) Condition=123;else Condition=12;}
        else
        {if(AirplaneModel>0) Condition=13;else Condition=1;}
    }
    else
    {
        if(ArrivalTime1>0&&ArrivalTime2>0)
        {if(AirplaneModel>0) Condition=23;else Condition=2;}
        else
        {if(AirplaneModel>0) Condition=3;else Condition=0;}
    }
    
    if(FlightDuration>0)
    {
        if(TransferDuration>0)
            Condition=45;
        else
            Condition=4;
    }
    else
        if(TransferDuration>0)
            Condition=5;
    return Condition;
} //GetCondition

int ConditionSatisfiedV1(FullIti * iti, int DepartureTime1, int DepartureTime2, int ArrivalTime1, int ArrivalTime2, int AirplaneModel, int FlightDuration, int TransferDuration, int Condition)
{
    int satisfied=0;
    if(iti!=NULL)
    {
        int departuretime1=iti->first->departure_time1;
        int arrivaltime1=iti->first->arrival_time1;
        int flightduration=iti->first->arrival_time1-iti->first->departure_time1;
        int transferduration=0;
        if(iti->second!=NULL)
        {arrivaltime1=iti->second->arrival_time1;flightduration+=iti->second->arrival_time1-iti->second->departure_time1;transferduration+=iti->second->departure_time1-iti->first->arrival_time1;}
        if(iti->third!=NULL)
        {arrivaltime1=iti->third->arrival_time1;flightduration+=iti->third->arrival_time1-iti->third->departure_time1;transferduration+=iti->third->departure_time1-iti->second->arrival_time1;}
    
        switch (Condition) {
            case 0:
                satisfied=1;
                break;
            case 1:
                if(DepartureTime1<=departuretime1&&DepartureTime2>=departuretime1)
                    satisfied=1;
                break;
            case 12:
                if(DepartureTime1<=departuretime1&&DepartureTime2>=departuretime1&&ArrivalTime1<=arrivaltime1&&ArrivalTime2>=arrivaltime1) satisfied=1;
                break;
            case 13:
                if(DepartureTime1<=departuretime1&&DepartureTime2>=departuretime1&&AirplaneModel==iti->first->airplane_model) satisfied=1;
                break;
            case 123:
                if(DepartureTime1<=departuretime1&&DepartureTime2>=departuretime1&&ArrivalTime1<=arrivaltime1&&ArrivalTime2>=arrivaltime1&&AirplaneModel==iti->first->airplane_model) satisfied=1;
                break;
            case 2:
                if(ArrivalTime1<=arrivaltime1&&ArrivalTime2>=arrivaltime1)
                    satisfied=1;
                break;
            case 23:
                if(ArrivalTime1<=arrivaltime1&&ArrivalTime2>=arrivaltime1&&AirplaneModel==iti->first->airplane_model) satisfied=1;
                break;
            case 3:
                if(AirplaneModel==iti->first->airplane_model) satisfied=1;
                break;
            case 4:
                if(flightduration<=FlightDuration) satisfied=1;
                break;
            case 45:
                if(flightduration<=FlightDuration&&transferduration<=TransferDuration) satisfied=1;
                break;
            case 5:
                if(transferduration<=TransferDuration) satisfied=1;
                break;
            default:
                break;
        }
    }
    return satisfied;
} //ConditionSatisfiedV1

int AirFares(FullIti * iti)
{
    int airfares=0;
    
    if(iti->first!=NULL) airfares+=iti->first->airfares;
    if(iti->second!=NULL) airfares+=iti->second->airfares;
    if(iti->third!=NULL) airfares+=iti->third->airfares;
    
    return airfares;
} //AirFares

Status PrintFullIti(FullIti * iti)
{
    if(iti->num_flights==0)
        printf("N/A\n");
    if(iti->num_flights>=1)
        PrintFlight(*iti->first);
    if(iti->num_flights>=2)
        PrintFlight(*iti->second);
    if(iti->num_flights>=3)
        PrintFlight(*iti->third);
    return OK;
} //PrintFullIti

Status ProperRouteV1(Schedule schedule, MGraph * M, Airport Departure, Airport Arrival, int DepartureTime1, int DepartureTime2, int ArrivalTime1, int ArrivalTime2, int AirplaneModel, int FlightDuration, int TransferDuration, int IS_MIN_COST)
{
    //Condition为0无任何筛选，为1限制起飞时段，为2限制降落时段，为3限制机型，为12限制起飞时段和降落时段，为23限制降落时段和机型，为13限制起飞时段和机型，为123限制起飞时段和降落时段和机型
    //IS_MIN_COST为1则输出航费最低路径，为0则不限制航费
    int Condition=GetCondition(DepartureTime1, DepartureTime2, ArrivalTime1, ArrivalTime2, AirplaneModel, FlightDuration, TransferDuration);
    Options AllOptions, ProperOptions;
    InitOptions(&AllOptions); InitOptions(&ProperOptions);
    
    printf("\nFROM: Airport No.%d\t\tTO: Airport No.%d\n",Departure,Arrival);
    int HaveRoutes=AllRoutesV1(schedule, M, Departure, Arrival, &AllOptions);

    if(!HaveRoutes)
        printf("No Flights Available\n");
    else
    {
        FullIti * pm = AllOptions.head->next;
        if(pm==NULL)
            printf("No Flights Available\n");
        else
        {
            int Min_Cost=100000;
            FullIti * MinCostIti = NULL;
    
            int airfares=100000;
            FullIti * pn = AllOptions.head->next;
            flight_record * flightI=NULL;
            flight_record * flightII=NULL;
            flight_record * flightIII=NULL;
            if(AllOptions.num_options[1]>0)
            {
                for(int i=0;i<AllOptions.num_options[1];i++)
                {
                    if(ConditionSatisfiedV1(pn, DepartureTime1, DepartureTime2, ArrivalTime1, ArrivalTime2, AirplaneModel, FlightDuration, TransferDuration, Condition)==1)
                    {
                        if(pn->first!=NULL) flightI=CreateFlight(pn->first);
                        if(pn->second!=NULL) flightII=CreateFlight(pn->second);
                        if(pn->third!=NULL) flightIII=CreateFlight(pn->third);
                        FullIti * iti = CreateFullIti(flightI, flightII, flightIII);
                        AddIti(&ProperOptions, iti);
                        airfares=AirFares(pn);
                        if(airfares<Min_Cost)
                        {
                            Min_Cost=airfares;
                            MinCostIti=pn;
                        }
                        flightI=flightII=flightIII=NULL;
                    }
                    pn=pn->next;
                }
            }
            if(AllOptions.num_options[2]>0)
            {
                for(int j=0;j<AllOptions.num_options[2];j++)
                {
                    if(ConditionSatisfiedV1(pn, DepartureTime1, DepartureTime2, ArrivalTime1, ArrivalTime2, AirplaneModel, FlightDuration, TransferDuration, Condition)==1)
                    {
                        if(pn->first!=NULL) flightI=CreateFlight(pn->first);
                        if(pn->second!=NULL) flightII=CreateFlight(pn->second);
                        if(pn->third!=NULL) flightIII=CreateFlight(pn->third);
                        FullIti * iti = CreateFullIti(flightI, flightII, flightIII);
                        AddIti(&ProperOptions, iti);
                        airfares=AirFares(pn);
                        if(airfares<Min_Cost)
                        {
                            Min_Cost=airfares;
                            MinCostIti=pn;
                        }
                        flightI=flightII=flightIII=NULL;
                    }
                    pn=pn->next;
                }
            }
            if(AllOptions.num_options[3]>0)
            {
                for(int k=0;k<AllOptions.num_options[3];k++)
                {
                    if(ConditionSatisfiedV1(pn, DepartureTime1, DepartureTime2, ArrivalTime1, ArrivalTime2, AirplaneModel, FlightDuration, TransferDuration, Condition)==1)
                    {
                        if(pn->first!=NULL) flightI=CreateFlight(pn->first);
                        if(pn->second!=NULL) flightII=CreateFlight(pn->second);
                        if(pn->third!=NULL) flightIII=CreateFlight(pn->third);
                        FullIti * iti = CreateFullIti(flightI, flightII, flightIII);
                        AddIti(&ProperOptions, iti);
                        airfares=AirFares(pn);
                        if(airfares<Min_Cost)
                        {
                            Min_Cost=airfares;
                            MinCostIti=pn;
                        }
                        flightI=flightII=flightIII=NULL;
                    }
                    pn=pn->next;
                }
            }
            
            if(IS_MIN_COST==1)
            {
                printf("Flight Schedule with the Lowest Fares\n");
                printf("Flight NO.\tDeparture\tDeparture Time\tArrival\tArrival Time\tDome/Intl\tAirplane Model\tFares\n");
                PrintFullIti(MinCostIti);
                printf("The Total Cost is $%d\n",Min_Cost);
                printf("\n");
            }
            else
            {
                printf("Flight NO.\tDeparture\tDeparture Time\tArrival\tArrival Time\tDome/Intl\tAirplane Model\tFares\n");
                if(ProperOptions.num_options[0]>10)
                {
                    FullIti * pm = ProperOptions.head->next;
                    printf("TOP TEN OPTIONS:\n");
                    for(int i=0;i<10;i++)
                    {
                        PrintFullIti(pm);
                        printf("\n");
                        pm=pm->next;
                    }
                }
                else if(ProperOptions.num_options[0]>0)
                {
                    FullIti * pm = ProperOptions.head->next;
                    for(int i=0;i<ProperOptions.num_options[0];i++)
                    {
                        PrintFullIti(pm);
                        printf("\n");
                        pm=pm->next;
                    }
                }
                else
                    printf("No Flights Available\n");
            }
        }
    }
    return OK;
} //ProperRouteV1

int ConvertTS(char * text, int type)
{   //type表示时间戳类型，1为“mm/dd hh:mm”格式，2为“hh:mm”格式
    char ts[20];memset(ts, 0, sizeof(ts));
    strcpy(ts, text);
    char delims1[] = "/ :";
    char delims2[] = ":";
    char * record;
    int time=0;
    
    if(type==1)
    {
        record = strtok(ts, delims1);
        record = strtok(NULL, delims1);
        int day=atoi(record)-1;
        record = strtok(NULL, delims1);
        int hour=atoi(record);
        record = strtok(NULL, delims1);
        int min=atoi(record);
        time=day*24*60+hour*60+min;
    }
    else if(type==2)
    {
        record = strtok(ts, delims2);
        int hours=atoi(record);
        record = strtok(NULL, delims2);
        int mins=atoi(record);
        time=hours*60+mins;
    }
    
    return time;
} //ConvertTS

Status GetOrder(MGraph M, Airport * Departure, Airport * Arrival, int * DepartureTime1, int * DepartureTime2, int * ArrivalTime1, int * ArrivalTime2, int * AirplaneModel, int * FlightDuration, int * TransferDuration, int * IS_MIN_COST)
{
    printf("Please Enter Your Starting Point & Destination (From 1 to %d):\n",M.vexnum);
    scanf("%d %d", Departure, Arrival);
    printf("Please Enter A Range For Your Preferred Departure Time:\n");
    printf("Format: mm/dd/hh:mm;\tAvailable Range: 05/05/00:00 -- 05/08/23:59\n");
    printf("Please Enter 00/00 00:00 If You Don't Have Preference\n");
    char DeTime1[20];memset(DeTime1, 0, sizeof(DeTime1));
    char DeTime2[20];memset(DeTime2, 0, sizeof(DeTime2));
    scanf("%s %s", DeTime1, DeTime2);
    *DepartureTime1=ConvertTS(DeTime1, 1);
    *DepartureTime2=ConvertTS(DeTime2, 1);
    printf("Please Enter A Range For Your Preferred Arrival Time:\n");
    printf("Format: mm/dd hh:mm;\tAvailable Range: 05/05/00:00 -- 05/09/23:59\n");
    printf("Please Enter 00/00/00:00 If You Don't Have Preference\n");
    char ArTime1[20];memset(ArTime1, 0, sizeof(ArTime1));
    char ArTime2[20];memset(ArTime2, 0, sizeof(ArTime2));
    scanf("%s %s", ArTime1, ArTime2);
    *ArrivalTime1=ConvertTS(ArTime1, 1);
    *ArrivalTime2=ConvertTS(ArTime2, 1);
    printf("Please Enter Your Preferred Airplane Model:\n");
    printf("Available Options: 1 2 3 4 5\n");
    printf("Please Enter 0 If You Don't Have Preference\n");
    scanf("%d",AirplaneModel);
    printf("Please Enter The Maximum Flight Duration:\n");
    printf("Format: hh:mm\n");
    printf("Please Enter 00:00 If You Don't Have Requirement\n");
    char FDur[10];memset(FDur, 0, sizeof(FDur));
    scanf("%s",FDur);
    *FlightDuration=ConvertTS(FDur, 2);
    printf("Please Enter The Maximum Transfer Duration:\n");
    printf("Format: hh:mm\n");
    printf("Please Enter 00:00 If You Don't Have Requirement\n");
    char TDur[10];memset(TDur, 0, sizeof(TDur));
    scanf("%s",TDur);
    *TransferDuration=ConvertTS(TDur, 2);
    printf("Are You Looking For the Cheapest Flight? Enter 1 if YES, 0 if NO\n");
    scanf("%d",IS_MIN_COST);
    
    return OK;
} //GetOrder

#endif /* SqList_h */
