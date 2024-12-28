// �̳߳���
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
        // �����̳߳� (Start the thread pool)
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] { // ��һ�������߳�: lambda����: ���ϴ� [�������tasks] ���������ִ��,ֱ������ִ������߳���ֹ;
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        cv.wait(lock, [this] { return done || !tasks.empty(); });   //  [�������tasks] ��Ϊ�վ������������,����̳߳عر���û������˳�
                        if (done && tasks.empty()) {
                            return;  // ����̳߳ر���ֹ���˳��߳�
                        }
                        task = std::move(tasks.front());  
                        tasks.pop();  
                        ++workersBusyCount;  // ��������ִ��������߳���
                    }
                    task();  // ִ������
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        --workersBusyCount;  // ������ɺ󣬼�������ִ��������߳���
                        cv.notify_all();  // ���������ȴ�����
                    }
                }
            });
        }
    }

    // �ύ���� [�������tasks]
    template <class F>
    void submit(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (done) {
                throw std::runtime_error("ThreadPool has been shutdown, cannot submit new tasks.");
            }
            tasks.push(std::forward<F>(f)); // ����������������
        }
        cv.notify_one();  // ����һ�����ڵȴ������������̣߳�ʹ������������ִ�С�
    }

    // �ȴ������������
    void waitForCompletion() {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [this] { return tasks.empty() && workersBusyCount == 0; });
    }

    // ֹͣ�̳߳ز��ȴ��߳̽���
    void shutdown() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            done = true;
        }
        cv.notify_all();  // ֪ͨ�����߳��˳�

        for (std::thread& worker : workers) {
            worker.join();  // �ȴ�ÿ���߳̽���
        }
    }

private:
    std::vector<std::thread> workers;  // �����߳�
    std::queue<std::function<void()>> tasks;  // �������
    std::mutex queueMutex;  // �����������������
    std::condition_variable cv;  // ��������
    bool done;  // �Ƿ�ֹͣ�̳߳�
    std::atomic<int> workersBusyCount{ 0 };  // ����ִ��������߳���
};
