//
//  main.c
//  Airlines Plan
//
//  Created by 余宏昊 on 2018/12/19.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "Itinerary_LkList.h"

int main()
{
    Schedule schedule;
    InitSchedule(&schedule);
    read_csv("schedule.csv", &schedule);
    PrintSchedule(schedule);
    ALGraph * algraph=CreateALGFromDF(schedule);
    printf("VexNum:%d; ArcNum:%d\n",algraph->vexnum,algraph->arcnum);
    printf("\nDepth First Traverse:\n");
    DFSTALGraph(*algraph, 2);
    printf("\nBreadth First Traverse:\n");
    BFSTALGraph(*algraph, 2);
    printf("\nBreadth First Traverse:\n");
    BFSTALGraph(*algraph, 4);
    printf("\n");
    MGraph * mgraph=CreateMGFromALG(*algraph);
    CheckRoute(mgraph, 3, 49);
    QuickestFlight(schedule, mgraph, 3, 49);
    CheckRoute(mgraph, 6, 49);
    QuickestFlight(schedule, mgraph, 6, 49);
    CheckRoute(mgraph, 50, 30);
    QuickestFlight(schedule, mgraph, 50, 30);
    CheckRoute(mgraph, 2, 16);
    QuickestFlight(schedule, mgraph, 2, 16);
    LimitTransRouteV1(schedule, mgraph, 3, 49, 1);
    LimitTransRouteV1(schedule, mgraph, 6, 49, 0);
    LimitTransRouteV1(schedule, mgraph, 6, 49, 1);
    LimitTransRouteV1(schedule, mgraph, 50, 30, 0);
    LimitTransRouteV1(schedule, mgraph, 2, 16, 1);
    Airport Departure=0; Airport Arrival=0;
    int DepartureTime1=0; int DepartureTime2=0; int ArrivalTime1=0; int ArrivalTime2=0;
    int AirplaneModel=0; int FlightDuration=0; int TransferDuration=0; int IS_MIN_COST=0;
    GetOrder(*mgraph, &Departure, &Arrival, &DepartureTime1, &DepartureTime2, &ArrivalTime1, &ArrivalTime2, &AirplaneModel, &FlightDuration, &TransferDuration, &IS_MIN_COST);
    ProperRouteV1(schedule, mgraph, Departure, Arrival, DepartureTime1, DepartureTime2, ArrivalTime1, ArrivalTime2, AirplaneModel, FlightDuration, TransferDuration, IS_MIN_COST);
    
    return 0;
}
