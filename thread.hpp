// 线程池类
#pragma once

#include <iostream>
#include <vector>
#include <queue>

#include <thread>
#include <functional>
#include <atomic>
#include <future>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : done(false) {
        // 启动线程池 (Start the thread pool)
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] { // 第一个工作线程: lambda函数: 不断从 [任务队列tasks] 拿任务出来执行,直到任务执行完或线程终止;
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        cv.wait(lock, [this] { return done || !tasks.empty(); });   //  [任务队列tasks] 不为空就拿任务出来做,如果线程池关闭且没任务就退出
                        if (done && tasks.empty()) {
                            return;  // 如果线程池被终止就退出线程
                        }
                        task = std::move(tasks.front());  
                        tasks.pop();  
                        ++workersBusyCount;  // 增加正在执行任务的线程数
                    }
                    task();  // 执行任务
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        --workersBusyCount;  // 任务完成后，减少正在执行任务的线程数
                        cv.notify_all();  // 唤醒整个等待队列
                    }
                }
            });
        }
    }

    // 提交任务到 [任务队列tasks]
    template <class F>
    void submit(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (done) {
                throw std::runtime_error("ThreadPool has been shutdown, cannot submit new tasks.");
            }
            tasks.push(std::forward<F>(f)); // 将任务放入任务队列
        }
        cv.notify_one();  // 唤醒一个正在等待条件变量的线程，使其获得锁并继续执行。
    }

    // 等待所有任务完成
    void waitForCompletion() {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [this] { return tasks.empty() && workersBusyCount == 0; });
    }

    // 停止线程池并等待线程结束
    void shutdown() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            done = true;
        }
        cv.notify_all();  // 通知所有线程退出

        for (std::thread& worker : workers) {
            worker.join();  // 等待每个线程结束
        }
    }

private:
    std::vector<std::thread> workers;  // 工作线程
    std::queue<std::function<void()>> tasks;  // 任务队列
    std::mutex queueMutex;  // 互斥锁保护任务队列
    std::condition_variable cv;  // 条件变量
    bool done;  // 是否停止线程池
    std::atomic<int> workersBusyCount{ 0 };  // 正在执行任务的线程数
};
