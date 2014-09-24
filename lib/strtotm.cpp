#if defined TEST_STRTOTM
  #include <stdio.h>
  #include <time.h>
#else 
  #include "tm.h" // my poor man's time.h
#endif //defined TEST_STRTOTM

#include <stdlib.h> // atoi()
#include <string.h> // strtok_r()

// reads a string of the form: "YYYY-MM-DD HH:MM:SS" and fills in the 
// relavant fields in the time struct, tm. Assumes time and date are UTC.
// uses strtok() from the c standard library and so, the input string
// is modified. 

int strtotm(char* s, struct tm* tm)
{
    char* token = 0;
    char* p = 0;
    const char* delims = ".- :T";
    int year, month, day, hour, min, sec;
    int n;

    n = 0;
    year = month = day = hour = min = sec = 0;
    token = strtok_r(s, delims, &p);
    while (token != 0) {
        ++n;
        switch (n) {
        case 1: // the year
            year = atoi(token);
            break;

        case 2: // month
            month = atoi(token);
            break;

        case 3: // day of month
            day = atoi(token);
            break;

        case 4: // hour
            hour = atoi(token);
            break;

        case 5: // minute
            min = atoi(token);
            break;

        case 6: // second
            sec = atoi(token);
            break;

        default:
            // this is an error. should break out of while loop.
            break;
        }

        token = strtok_r(0, delims, &p);
    }

    if (tm != 0) {
        tm->tm_year = year - 1900;
        tm->tm_mon = month - 1;
        tm->tm_mday = day;
        tm->tm_hour = hour;
        tm->tm_min = min;
        tm->tm_sec = sec;
    }

    return n;
}

#if defined TEST_STRTOTM

int main(int argc, char** argv)
{
    struct tm tm;
    time_t t;
    char buf[128];

    memset(&tm, 0, sizeof tm);
    memset(buf, 0, sizeof buf);
 
    t = time(&t);
    printf("asctime(): %s", asctime(gmtime(&t)));

    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", gmtime(&t));
    printf("buf:        %s\n", buf);

    strtotm(buf, &tm);
    printf("asctime(): %s", asctime(&tm));



    return 0;
}

#endif // defined TEST_STRTOTM


