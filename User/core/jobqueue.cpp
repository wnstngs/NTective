/*!
 *  @file       jobqueue.cpp
 *  @brief      Simple job queue.
 */

#include "jobqueue.hpp"

namespace Core {

void
JOB_QUEUE::PopAndExecute()
{
    JOB job;
    {
        std::lock_guard lock{Lock_};
        job = std::move(JobQueue_.front());
        JobQueue_.pop_front();
    }
    job();
}

void
JOB_QUEUE::EnqueueInternal(
    JOB Job
)
{
    std::lock_guard lock{Lock_};
    JobQueue_.push_back(std::move(Job));
}

}
