//
//  DataFrame.h
//  Airlines Plan
//
//  Created by 余宏昊 on 2018/12/19.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef DataFrame_h
#define DataFrame_h

#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define INFEASIBLE -1

#define MAX_FLIGHT_NUM 3000

typedef int Status;

typedef int Airport;
typedef struct {
    int            flight_id;
    char     depart_date[15];
    char      flight_type[5];    //Intl/Dome
    int            flight_no;
    int            departure;    //Departure Airport
    int              arrival;    //Arrival Airport
    char  departure_time[20];
    char    arrival_time[20];
    int      departure_time1;    //Departure time in numerical form
    int        arrival_time1;    //Arrival time in numerical form
    int          airplane_id;
    int       airplane_model;
    int             airfares;
} flight_record;
typedef struct {
    flight_record     * records;
    int                 row,col;
} Schedule;

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
Status InitRecord(flight_record * record);
    //初始化航线记录
Status InitSchedule(Schedule * schedule);
    //初始化航班安排表
int get_row(char *filename);
    //获取航班安排表行数
int get_col(char *filename);
    //获取航班安排表列数
Status read_csv(char * filename, Schedule * schedule);
    //将csv文件读入到航班安排表中
Status PrintSchedule(Schedule schedule);
    //打印航班安排表
Status PrintFlight(flight_record flight);
    //打印输出航班
Status GetFlightID(Schedule schedule, Airport Departure, Airport Arrival, int * ID);
    //获取两个机场之间的所有航班ID，存放到一个整型数组中
flight_record * GetRecord(Schedule schedule, int i);
    //根据航班ID获取航班记录
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//

Status InitRecord(flight_record * record)
{
    record->flight_id=-1;record->flight_no=-1;
    record->departure=-1;record->arrival=-1;
    record->departure_time1=-1;record->arrival_time1=-1;
    record->airplane_id=-1;record->airplane_model=-1;
    record->airfares=-1;
    memset(record->depart_date,0,sizeof(record->depart_date));
    memset(record->flight_type,0,sizeof(record->flight_type));
    memset(record->departure_time,0,sizeof(record->departure_time));
    memset(record->arrival_time,0,sizeof(record->arrival_time));
    return OK;
} //InitRecord

Status InitSchedule(Schedule * schedule)
{
    schedule->records=(flight_record *)malloc(MAX_FLIGHT_NUM*sizeof(flight_record));
    for(int i=0;i<MAX_FLIGHT_NUM;++i)
        InitRecord(&schedule->records[i]);
    schedule->col=schedule->row=0;
    return OK;
} //InitSchedule

int get_row(char *filename)
{
    char line[1024];
    int i=0;
    FILE* stream = fopen(filename, "r");
    while(fgets(line, 1024, stream)){
        i++;
    }
    fclose(stream);
    return i;
}

int get_col(char *filename)
{
    char line[1024];
    int i = 0;
    FILE* stream = fopen(filename, "r");
    fgets(line, 1024, stream);
    char* token = strtok(line, ",");
    while(token){
        token = strtok(NULL, ",");
        i++;
    }
    fclose(stream);
    return i;
}

Status read_csv(char * filename, Schedule * schedule)
{
    InitSchedule(schedule);
    char * line, * record;
    char buffer[2048];
    FILE* stream = fopen(filename, "r");
    if(stream!=NULL)
    {
        printf("READING...\n");
        char delims[] = ",";
        char delims2[] = "/ :";
        char ts1[20];memset(ts1,0,sizeof(ts1));
        char ts2[20];memset(ts2,0,sizeof(ts2));
        char *ts1b=NULL;
        char *ts2b=NULL;
        int j = 0;
        int days1,days2;
        int hours1,hours2;
        int minutes1,minutes2;
        int time1,time2;
        line=fgets(buffer, sizeof(buffer), stream);
        while ((line = fgets(buffer, sizeof(buffer), stream))!=NULL)//当没有读取到文件末尾时循环继续
        {
            record = strtok(line, delims);  //X~XXXX
            schedule->records[j].flight_id=atoi(record);
            record = strtok(NULL, delims);  //mm/dd/yyyy
            strcpy(schedule->records[j].depart_date, record);
            record = strtok(NULL, delims);  //Intl/Dome
            strcpy(schedule->records[j].flight_type, record);
            record = strtok(NULL, delims);  //XX~XXX
            schedule->records[j].flight_no=atoi(record);
            record = strtok(NULL, delims);  //X~XX
            schedule->records[j].departure=atoi(record);
            record = strtok(NULL, delims);  //X~XX
            schedule->records[j].arrival=atoi(record);
            record = strtok(NULL, delims);  //mm/dd/yyyy XX:XX
            strcpy(schedule->records[j].departure_time, record);
            record = strtok(NULL, delims);  //mm/dd/yyyy XX:XX
            strcpy(schedule->records[j].arrival_time, record);
            record = strtok(NULL, delims);  //X~XXX
            schedule->records[j].airplane_id=atoi(record);
            record = strtok(NULL, delims);  //X
            schedule->records[j].airplane_model=atoi(record);
            record = strtok(NULL, delims);  //XXX~XXXX
            schedule->records[j].airfares=atoi(record);
            
            strcpy(ts1, schedule->records[j].departure_time);
            ts1b = strtok(ts1,delims2);
            ts1b = strtok(NULL,delims2);
            days1=atoi(ts1b)-1;
            ts1b = strtok(NULL,delims2);
            ts1b = strtok(NULL,delims2);
            hours1=atoi(ts1b);
            ts1b = strtok(NULL,delims2);
            minutes1=atoi(ts1b);
            time1=days1*24*60+hours1*60+minutes1;
            schedule->records[j].departure_time1=time1;
            
            strcpy(ts2, schedule->records[j].arrival_time);
            ts2b = strtok(ts2,delims2);
            ts2b = strtok(NULL,delims2);
            days2=atoi(ts2b)-1;
            ts2b = strtok(NULL,delims2);
            ts2b = strtok(NULL,delims2);
            hours2=atoi(ts2b);
            ts2b = strtok(NULL,delims2);
            minutes2=atoi(ts2b);
            time2=days2*24*60+hours2*60+minutes2;
            schedule->records[j].arrival_time1=time2;
            
            ++j;
        } //while
        schedule->row=get_row(filename);
        schedule->col=get_col(filename);
    } //if
    return OK;
}

Status PrintSchedule(Schedule schedule)
{
    printf("\nFlight ID\tDeparture Date\tIntl/Dome\tFlight.No\tDepature Airport\tArrival Airport\t\n");
    for(int i=0;i<schedule.row-1;++i)
        printf("%-4d\t\t%-9s\t\t%-4s\t\t%-4d\t\t%-2d\t\t\t\t\t%-2d\n",schedule.records[i].flight_id,schedule.records[i].depart_date,schedule.records[i].flight_type,schedule.records[i].flight_no,schedule.records[i].departure,schedule.records[i].arrival);
    printf("\nFlight ID\tDeparture Time\tArrival Time\tAirplane ID\tAirplane Model\tAir Fares\t\n");
    for(int i=0;i<schedule.row-1;++i)
        printf("%-4d\t\t%-15s\t%-15s\t%-4d\t\t%d\t\t\t\t%d\n",schedule.records[i].flight_id,schedule.records[i].departure_time,schedule.records[i].arrival_time,schedule.records[i].airplane_id,schedule.records[i].airplane_model,schedule.records[i].airfares);
    printf("\n");
    printf("Size: %d rows * %d columns\n",schedule.row,schedule.col);
    return OK;
}

Status PrintFlight(flight_record flight)
{
    printf("%d\t\t\t%d\t\t\t%s\t%d\t\t%s\t%s\t\t%d\t\t\t\t%d\n",flight.flight_no,flight.departure,flight.departure_time,flight.arrival,flight.arrival_time,flight.flight_type,flight.airplane_model, flight.airfares);
    return OK;
} //PrintFlight

Status GetFlightID(Schedule schedule, Airport Departure, Airport Arrival, int * ID)
{
    int num=0;
    for(int i=0;i<schedule.row-1;++i)
    {
        if(Departure==schedule.records[i].departure&&Arrival==schedule.records[i].arrival)
            ID[++num]=schedule.records[i].flight_id;
    }
    ID[0]=num;
    return OK;
} //GetFlightID

flight_record * GetRecord(Schedule schedule, int i)
{
    if(i<1||i>schedule.row-1) return NULL;
    flight_record * record=(flight_record *)malloc(sizeof(flight_record));
    InitRecord(record);
    record->flight_id=schedule.records[i-1].flight_id;
    strcpy(record->depart_date, schedule.records[i-1].depart_date);
    strcpy(record->flight_type, schedule.records[i-1].flight_type);
    record->flight_no=schedule.records[i-1].flight_no;
    record->departure=schedule.records[i-1].departure;
    record->arrival=schedule.records[i-1].arrival;
    strcpy(record->departure_time, schedule.records[i-1].departure_time);
    strcpy(record->arrival_time, schedule.records[i-1].arrival_time);
    record->airplane_id=schedule.records[i-1].airplane_id;
    record->airplane_model=schedule.records[i-1].airplane_model;
    record->airfares=schedule.records[i-1].airfares;
    record->departure_time1=schedule.records[i-1].departure_time1;
    record->arrival_time1=schedule.records[i-1].arrival_time1;
    return record;
} //GetRecord

#endif /* DataFrame_h */
