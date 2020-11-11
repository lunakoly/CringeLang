#include "thread_pool.hpp"

#include <iostream>


using namespace threading;


ThreadPool::ThreadPool(int workers_count) {
    if (workers_count == 0) {
        workers_count = 1;
    }

    // std::cout << "//Starting a pool with " << workers_count << " workers//" << std::endl;

    busy_workers_count = workers_count;

    for (size_t it = 0; it < workers_count; it++) {
        // `ThreadPool::` is required here
        workers.push_back(std::thread(&ThreadPool::process, this));
    }
}


ThreadPool::~ThreadPool() {
    should_stop = true;
    notifier.notify_all();

    for (auto & it : workers) {
        if (it.joinable()) {
            it.join();
        }
    }
}


void ThreadPool::schedule(Task task) {
    std::lock_guard lock(tasks_protector);
    tasks.push(task);
    notifier.notify_one();
}


// void ThreadPool::process() {
//     while (true) {
//         Task task = nullptr;

//         {
//             std::unique_lock lock(tasks_protector);

//             notifier.wait(lock, [&]() {
//                 return !tasks.empty() || should_stop;
//             });

//             // this is not the same as `should_stop`
//             // because we may still have something in the queue
//             // and `should_stop == true` at the same time
//             if (tasks.empty()) {
//                 break;
//             }

//             task = tasks.front();
//             tasks.pop();
//         }

//         task();
//     }
// }

void ThreadPool::process() {
    while (!should_stop) {
        Task task = nullptr;

        {
            std::unique_lock lock(tasks_protector);

            busy_workers_count -= 1;

            if (tasks.empty() && busy_workers_count == 0) {
                all_tasks_processed_notifier.notify_all();
            }

            notifier.wait(lock, [&]() {
                return !tasks.empty() || should_stop;
            });

            if (should_stop) {
                break;
            }

            busy_workers_count += 1;

            task = tasks.front();
            tasks.pop();
        }

        task();
    }
}


void ThreadPool::wait() {
    std::unique_lock lock(tasks_protector);

    all_tasks_processed_notifier.wait(lock, [&]() {
        return tasks.empty() && busy_workers_count == 0;
    });
}
