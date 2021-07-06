#include "../include/eventmonitor.h"

#include <unistd.h>
#include <iostream>
#include <poll.h>
#include <errno.h>

EventMonitor* EventMonitor::first = nullptr;
EventMonitor* EventMonitor::last = nullptr;
bool EventMonitor::changed = false;
bool EventMonitor::terminateFlag;
int EventMonitor::returnCode = -1;
size_t EventMonitor::count = 0;

EventMonitor::~EventMonitor()
{
    if (fd != -1)
        stop ();
}

void EventMonitor::start (int fd, bool read, bool write)
{
    // Уже запущен?
    if (this->fd != -1)
        return;

    // Сохраняем file descriptor
    this->fd = fd;
    this->read = read;
    this->write = write;

    // Добавляем в двухсвязный список
    if (last)
        last->next = this;
    else
        first = this;
    prev = last;
    next = nullptr;
    last = this;
    count++;
    changed = true;
}

void EventMonitor::stop()
{
    // Запущен?
    if (fd == -1)
        return;

    // Удаление из двухсвязного списка
    if (prev)
        prev->next = next;
    else
        first = next;

    if (next)
        next->prev = prev;
    else
        last = prev;

    fd = -1;
    count--;
    changed = true;
}

int EventMonitor::dispatch (unsigned timeout_ms)
{
    terminateFlag = false;
    while (!terminateFlag)
    {
        // Формируем список pollfd для функции poll
        struct pollfd fds[count];
        EventMonitor* i;
        unsigned j;
        for(i = first, j = 0; i; i = i->next, j++)
        {
            fds [j].fd = i->fd;
            fds [j].events = (i->read ? POLLIN : 0) | (i->write ? POLLOUT : 0);
        }

        // Ожидаем прихода событий
        int r = 0;
        do
        {
            r = poll (fds, count, timeout_ms);
        }
        while (r == 0 && !terminateFlag);

        // Проверка errno
        if (r == -1)
        {
            if ((errno == EINTR || errno == EAGAIN))
                continue;
            return -1;
        }

        // Проверка причины выхода из poll
        changed = false;
        for (i = first, j = 0; i; i = i->next, j++)
        {
            // Событие чтения?
            if (fds[j].revents & POLLIN)
            {
                // Сброс флага
                fds[j].revents &= ~POLLIN;
                // Запуск обработчика
                i->onREvent ();
                // Если список изменился, то выходим
                if (changed)
                    break;
            }
            // Событие записи?
            if (fds[j].revents & POLLOUT)
            {
                // Сброс флага
                fds[j].revents &= ~POLLOUT;
                // Запуск обработчика
                i->onWEvent ();
                // Если список изменился, то выходим
                if (changed)
                    break;
            }
        }
    }
    return returnCode;
}

void EventMonitor::quit(int _returnCode)
{
    returnCode = _returnCode;
    terminateFlag = true;
}
