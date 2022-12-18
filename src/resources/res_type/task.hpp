#pragma once

#include "optional"
#include "future"
#include "function/framework/rectangle.hpp"
#include "function/framework/component/number_component.hpp"
#include <glm/glm.hpp>

namespace crp {
    template<typename Fun, typename... Args>
    class TaskResult {
    public:
        TaskResult(std::future<std::invoke_result_t<Fun, Args...>> &result,
                   std::shared_ptr<bool> &r, bool &lock)
                : result{std::move(result)}, ready{r}, lockTaskQueue{lock} {
        }

        std::shared_ptr<bool> ready;
        bool &lockTaskQueue;
        bool finished{};

        std::future<typename std::invoke_result<Fun, Args...>::type> result;

        std::optional<typename std::invoke_result<Fun, Args...>::type> getResult(bool shouldWait) {
            if (!*ready && shouldWait)
                lockTaskQueue = true;
            if (finished)return std::optional<typename std::invoke_result<Fun, Args...>::type>();
            if (*ready) {
                lockTaskQueue = false;
                finished = true;
                return result.get();
            }
            return std::optional<typename std::invoke_result<Fun, Args...>::type>();
        }
    };


    class Task : public Rectangle {
    public:
        explicit Task(std::function<void()> fun) :
                task{std::move(fun)}, Rectangle{Rectangle::MakeRectangle(meshPoints, TASK_COLOR, true, true)} {
            ready = std::make_shared<bool>(false);
            setPosition(initPosition);
        };

        Task() : Rectangle{Rectangle::MakeRectangle(meshPoints, TASK_COLOR, true, true)} {
            ready = std::make_shared<bool>(false);
            setPosition(initPosition);
        }

        static Task makeSleepTask(long long time) {
            Task now;
            now.gameObject->addComponent(NumberComponent, now.width - 0.15, now.height - 0.025, time);
            auto number = now.gameObject->tryGetComponent(NumberComponent);
            using task = std::packaged_task<void()>;
            auto t = std::make_shared<task>(
                    [time = time, number = number]() {
                        number->setBegin();
                        std::this_thread::sleep_for(std::chrono::seconds(time));
                    });
            now.task = std::move([t] { (*t)(); });
            return now;
        }

        std::function<void()> task;

        std::shared_ptr<bool> ready{};

        void run(glm::vec3 &point);

        static glm::vec3 initPosition;
        static float taskWidth, taskHeight;

        static float l, r, u, d;
        static std::vector<glm::vec3> meshPoints;
    };
}