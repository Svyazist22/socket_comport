#include "../include/timer.h"

#include <sys/timerfd.h>
#include <unistd.h>
#include <iostream>

bool Timer::start(unsigned msec)
{
    stop ();

    struct timespec now;
    if (clock_gettime(CLOCK_REALTIME, &now) != 0)
    {
        std::cerr << "Ошибка выполнения clock_gettime в Timer.start. Код ошибки " << errno << "." << std::endl;
        return false;
    }

    struct itimerspec new_value;
    new_value.it_interval.tv_sec = msec / 1000;
    new_value.it_interval.tv_nsec = msec % 1000 * 1000000;
    new_value.it_value.tv_nsec = now.tv_nsec + new_value.it_interval.tv_nsec;
    new_value.it_value.tv_sec = now.tv_sec + new_value.it_interval.tv_sec;
    if (new_value.it_value.tv_nsec >= 1000000000)
    {
        new_value.it_value.tv_nsec -= 1000000000;
        new_value.it_value.tv_sec++;
    }

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1)
    {
        std::cerr << "Ошибка выполнения timerfd_create в Timer.start. Код ошибки " << errno << "." << std::endl;
        return false;
    }

    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) != 0)
    {
        std::cerr << "Ошибка выполнения timerfd_settime в Timer.start. Код ошибки " << errno << "." << std::endl;
        close(fd);
        fd = -1;
        return false;
    }

    eventMonitor.onREvent = method(this, &Timer::onEvent);
    eventMonitor.start (fd);

    return true;
}

void Timer::stop()
{
    if (fd != -1)
    {
        eventMonitor.stop();
        close(fd);
        fd = -1;
    }
}

void Timer::onEvent ()
{
    uint64_t exp;
    ssize_t r = read (fd, &exp, sizeof(uint64_t));
    if (r != (ssize_t)sizeof(uint64_t))
    {
        std::cerr << "Ошибка выполнения read в Timer.onSelect. Результат " << r << ". Код ошибки " << errno << "." << std::endl;
        abort();
    }
    (void)r;
    onTick();
}
