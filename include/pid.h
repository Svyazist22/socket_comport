/**
  @brief Прочитать PID текущего потока
  @author Nikita Shishkov
  @date 2021-05-05
  */

#pragma once

#include <unistd.h>
#include <sys/syscall.h>

/**
 * @brief getPid Прочитать PID текущего потока
 * @return PID
 */
static inline pid_t getPid ()
{
    return syscall(__NR_gettid);
}
