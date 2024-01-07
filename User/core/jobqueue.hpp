/*!
 *  @file       jobqueue.hpp
 *  @brief      Simple job queue.
 */

#pragma once

#include <functional>
#include <mutex>
#include <deque>
#include <future>

#include "../common/log.hpp"
#include "../common/macros.h"

namespace Core {

/*!
 * @brief A simple job (generic task) queue implementation.
 */
class JOB_QUEUE {
public:
    /**
     * @brief Enqueues a function to be executed asynchronously.
     * @tparam FunctionType Type of the invocable function.
     * @param Function The function to be enqueued for execution.
     * @return A future object representing the result of the enqueued function.
     */
    template<std::invocable FunctionType>
    auto
    Enqueue(FunctionType &&Function)
    {
        using PACKAGED_JOB = std::packaged_task<std::invoke_result_t<FunctionType>()>;

        PACKAGED_JOB packagedJob{std::forward<FunctionType>(Function)};
        auto future = packagedJob.get_future();

        EnqueueInternal([packagedJob_ = std::move(packagedJob)]() mutable {
            packagedJob_();
        });

        return future;
    }

    /**
     * @brief Retrieves and executes a job from the queue.
     */
    void
    PopAndExecute();

private:
    using JOB = std::move_only_function<void()>;

    void
    EnqueueInternal(
        JOB Job
    );

    std::mutex Lock_;
    std::deque<JOB> Jobs_;
};

}
