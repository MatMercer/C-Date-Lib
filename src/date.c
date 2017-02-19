#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

typedef enum week_days {
    UNDEFINED = 0,
    SUNDAY = 1,
    MONDAY = 2,
    TUESDAY = 3,
    WEDNESDAY = 4,
    THURSDAY = 5,
    FRIDAY = 6,
    SATURDAY = 7
} week_day;

typedef enum month_enum {
    JANUARY = 1,
    FEBRUARY = 2,
    MARCH = 3,
    APRIL = 4,
    MAY = 5,
    JUNE = 6,
    JULY = 7,
    AUGUST = 8,
    SEPTEMBER = 9,
    OCTOBER = 10,
    NOVEMBER = 11,
    DECEMBER = 12
} month;

bool month_31_list[] = {
    true, // jan
    false, // feb
    true, // mar
    false, // apr
    true, // may
    false, // jun
    true, // jul
    true, // aug
    false, // sep
    true, // oct
    false, // nov
    true // dec
};

typedef struct dates {
    week_day w; // week day
    unsigned int d; // day
    month m; // month
    unsigned int y; // year
} date;

// Based in https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
unsigned int schwerdtfeger_table[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

void calc_day_of_week(date *d) {
    unsigned int y = d->y - (d->m < 3);
    if (y > 1752) {
        d->w = (y + y/4 - y/100 + y/400 + schwerdtfeger_table[d->m-1] + d->d) % 7;
        d->w += 1;
    }
    else {
        errno = 22;
        perror("Error when calculating day of week! Year must be > 1752.");
        d->w = UNDEFINED;
    }
}

bool is_year_bissext(unsigned int y) {
    if (y % 400 == 0) {
        return true;
    }
    else if (y % 4 == 0 && y % 100 != 0) {
        return true;
    }
    return false;
}

bool valid_date_values(unsigned int d, month m, unsigned int y) {
    if ((d < 1 || d > 31) || (m < JANUARY || m > DECEMBER) || (y < 1)) {
        return false;
    }
    return true;
}

date error_date() {
    date d;
    return d;
}

int create_date(date *dt, unsigned int d, month m, unsigned int y) {
    bool bissext = is_year_bissext(y);
    bool has_31 = month_31_list[m - 1];

    // General value validation
    if (!valid_date_values(d, m, y)) {
        errno = 22;
        perror("Error when creating date! A value is invalid for date creation");
        return -1;
    }

    // 31 days validation
    if ((!has_31 && d > 30) || m == FEBRUARY && d > 29) {
        errno = 22;
        perror("The day for the month you specified is invalid");
        return -1;
    }

    // Bissext validation
    if (!bissext && m == FEBRUARY && d > 28) {
        errno = 22;
        perror("The year you specified is not bissext");
        return -1;
    }

    dt->d = d;
    dt->m = m;
    dt->y = y;
    calc_day_of_week(dt);

    return 0;
}

date sum_dates(date dtX, date dtY) {
    unsigned int y = dtX.y + dtY.y;
    unsigned int m = dtX.m + dtY.m;

    unsigned int d = dtX.d;
    unsigned int sumd = dtY.d;

    while (m > 12) {
        y += 1;
        m -= 12;
    }
    if (!is_year_bissext(y) && m == FEBRUARY && d == 29) {
        d = 28;
    }

    while (sumd > 0) {
        if ((d == 31 && month_31_list[m - 1])
                || (d == 30 && !month_31_list[m - 1])
                || (d == 28 && m == FEBRUARY && !is_year_bissext(m))
                || (d == 29 && m == FEBRUARY && is_year_bissext(m))
           ) {
            m += 1;
            if (m > 12) {
                m -= 12;
                y += 1;
            }
            d = 1;
        }
        else {
            d += 1;
        }
        sumd -= 1;
    }

    date dt;
    create_date(&dt, d, m, y);
    return dt;
}

void print_date(date dt) {
    printf("%02d/%02d/%04d - %1d\n", dt.d, dt.m, dt.y, dt.w);
}

int main(int argc, char *argv) {
    int err;
    unsigned int day;
    unsigned int mon;
    unsigned int year;

    printf("Insert day... ");
    scanf("%u", &day);
    printf("Insert month... ");
    scanf("%u", &mon);
    printf("Insert year... ");
    scanf("%u", &year);

    date dt;
    err = create_date(&dt, day, mon, year);
    print_date(dt);

    printf("Insert day... ");
    scanf("%u", &day);
    printf("Insert month... ");
    scanf("%u", &mon);
    printf("Insert year... ");
    scanf("%u", &year);

    date dtx;
    err = create_date(&dtx, day, mon, year);

    date sum = sum_dates(dt, dtx);

    if (err != 0) {
        return -1;
    }

    print_date(sum);

    return 0;
}

