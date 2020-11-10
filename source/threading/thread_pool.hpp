// Copyright (C) 2020 luna_koly
//
// A simple implementation of a thread pool.


#pragma once

#include <queue>
#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>
#include <functional>


namespace threading {
    /**
     * A pool of workers that may execute
     * some queue of functions (tasks) concurrently.
     */
    class ThreadPool {
    public:
        /**
         * The general form of a task.
         */
        // using Task = void (*)();
        using Task = std::function<void()>;

        ThreadPool(
            int workers_count = std::thread::hardware_concurrency()
        );

        ~ThreadPool();

        /**
         * Adds the task to the inner queue.
         */
        void schedule(Task task);

        /**
         * Blocks until the tasks queue is empty.
         */
        void wait();

    private:
        /**
         * The vector of available workers.
         */
        std::vector<std::thread> workers;
        /**
         * The unprocessed tasks.
         */
        std::queue<Task> tasks;
        /**
         * Locked when accessing the queue.
         */
        std::mutex tasks_protector;
        /**
         * Used to distribute events about
         * unprocessed tasks left.
         */
        std::condition_variable notifier;
        /**
         * True on destruction.
         */
        bool should_stop = false;
        /**
         * Used to notify the user that
         * all tasks have been processed.
         */
        std::condition_variable all_tasks_processed_notifier;

        /**
         * Called by each single worker every time.
         */
        void process();
    };
}
