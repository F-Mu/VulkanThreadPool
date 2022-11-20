#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <future>
namespace crp {
    class ThreadPool {
    public:
        ThreadPool();

        ~ThreadPool();

        template<typename Fun, typename ...Args>
        std::future<typename std::invoke_result<Fun, Args...>::type> add(Fun &&, Args &&...);

        int activateNum() { return activateThreadNum; }

    private:
        using task_type = std::function<void()>;
        std::mutex _mut;
        std::queue<task_type> tasks;
        std::vector<std::thread> threads;
        std::atomic<bool> stop;
        std::condition_variable cond;
        int activateThreadNum;
    };

    ThreadPool::ThreadPool() : stop{false} {
        activateThreadNum = 0;
        int n = std::thread::hardware_concurrency();
        n = n ? n : 2;
//        std::cout<<n<<std::endl;
        for (int i = 0; i < n; ++i) {
//            std::cout << "tp" << activateNum() << '#' << std::endl;
            threads.emplace_back([this] {
                while (!this->stop) {
                    task_type task;
                    {
                        std::unique_lock<std::mutex> lock(this->_mut);
                        this->cond.wait(lock, [this] {
                            return this->stop || !tasks.empty();
                        });
                        if (stop)return;
                        task = std::move(tasks.front());
                        tasks.pop();
                        ++activateThreadNum;
//                        std::cout << "tp" << activateNum() << '#' << std::endl;
                    }
                    task();
                    {
                        std::unique_lock<std::mutex> lock(this->_mut);
                        --activateThreadNum;
                    }
                }
            });
        }
    }

    ThreadPool::~ThreadPool() {
        stop = false;
        for (auto &thread: threads) {
            if (thread.joinable())
                thread.join();
        }
    }

    template<typename Fun, typename... Args>
    std::future<typename std::invoke_result<Fun, Args...>::type>
    ThreadPool::add(Fun &&fun, Args &&...args) {
        using result_type = typename std::invoke_result<Fun, Args...>::type;
        using task = std::packaged_task<result_type()>;
        auto t = std::make_shared<task>(std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...));
        auto ret = t->get_future();
        {
            std::lock_guard<std::mutex> lock(this->_mut);
            if (stop)
                throw std::runtime_error("Thread pool has stopped");
            tasks.emplace([t] { (*t)(); });
        }
        cond.notify_one();
        return ret;
    }

}
