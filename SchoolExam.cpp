/*
Exercise:

With the following typedef structs make the output be something like:
"From Helsinki arrives at 10-01-2020"

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int hours, mins;
} DURATION;

typedef struct {
    int day;
    char month[4];
    int year;
    int Hour, Min;
} DEPARTURE;

typedef struct {
    const char* pStartingPoint;
    DEPARTURE Departure;
    DURATION Duration;
    int TimeZone;
} FLIGHT;

char** Exam(FLIGHT* pFlights, int nFlights);

int main()
{
    FLIGHT flights[] = { {"Buenos Aires", {1, "Mar", 2020, 19, 40} , {18,0}, -3},
                        {"Guatemala City", {20, "Feb", 2020, 12, 20}, {19,15}, -6},
                        {"Bangkok", {20,"Jan",2020, 18, 30}, {10,25}, 7},
                        {"Helsinki", {27, "Dec", 2020, 23, 55}, {0, 35}, 2},
                        {"Stockholm", {30, "Dec", 2020, 17, 40 }, {1,0}, 1} };

    Exam(flights, 5);
    return 0;
}

char** Exam(FLIGHT* pFlights, int nFlights) {
    for (int i = 0; i < nFlights; i++) {
        int ttlhr = (pFlights[i].Departure.Hour + pFlights[i].Duration.hours); // First add the durations
        int ttlmn = pFlights[i].Departure.Min + pFlights[i].Duration.mins;
        if (pFlights[i].TimeZone < 0) {
            ttlhr -= pFlights[i].TimeZone;
            ttlhr += 2; // Estonia
        }
        else if (pFlights[i].TimeZone > 2) {
            ttlhr -= pFlights[i].TimeZone - 2;
        }
        else if (pFlights[i].TimeZone < 2) {
            ttlhr += pFlights[i].TimeZone;
        }

        int ttld = pFlights[i].Departure.day;
        char* ttlmon = pFlights[i].Departure.month;

        if (ttlmn > 60) {
            ttlhr += 1;
            ttlmn -= 60;
            if (ttlhr > 24) {
                ttlhr -= 24;
                ttld += 1;
            } 
        } else {
            if (ttlhr > 24) {
                ttlhr -= 24;
                ttld += 1;
            }
        }

        // Now lets define mktime stuff
        struct tm time;
        
        char const* months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

        int x = 0;
        while (x < 12) {
            if (strcmp(ttlmon, months[x])==0) {
                time.tm_mon = x;
                break;
            }
            else {
                x++;
            }
        }
        
        time.tm_year = pFlights[i].Departure.year - 1900;
        time.tm_mday = ttld;
        time.tm_hour = ttlhr;
        time.tm_min = ttlmn;
        time.tm_sec = 0;
        time.tm_isdst = -1;


        char buffer[80];
        int ret = mktime(&time);

        // Verify that the mktime function is able to perform
        if (ret == -1) {
            printf("Error in making mktime function.");
            printf("\n");
        }
        else {
            strftime(buffer, sizeof(buffer), "%c", &time);
        }

        printf("From %s arrives at ", pFlights[i].pStartingPoint);
        printf(buffer);
        printf("\n");
    }
    return 0;
}
