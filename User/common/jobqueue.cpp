/*!
 *  @file       jobqueue.cpp
 *  @brief      Simple job queue.
 */

#include "jobqueue.hpp"

namespace Common::Util {

void
JOB_QUEUE::PopAndExecute()
{
    JOB job;
    {
        std::lock_guard lock{Lock_};
        job = std::move(Jobs_.front());
        Jobs_.pop_front();
    }
    job();
}

void
JOB_QUEUE::EnqueueInternal(
    JOB Job
)
{
    std::lock_guard lock{Lock_};
    Jobs_.push_back(std::move(Job));
}

}
