#pragma once

#include "crp_window.hpp"
#include "crp_device.hpp"
#include "crp_swap_chain.hpp"
#include "crp_game_obejct.hpp"
#include "crp_renderer.hpp"
#include "crp_descriptors.hpp"
#include "systems/thread_pool_system.hpp"
#include "systems/runtime_system.hpp"
#include "systems/task_queue_system.hpp"
//std
#include <memory>
#include <vector>

namespace crp {
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();

        ~FirstApp();

        FirstApp(const FirstApp &) = delete;

        FirstApp &operator=(const FirstApp &) = delete;

        void run();

    private:
        void test();

        CrpWindow crpWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        CrpDevice crpDevice{crpWindow};
        CrpRenderer crpRenderer{crpWindow, crpDevice};

        //note: order of declarations matters
        std::unique_ptr<CrpDescriptorPool> globalPool{};
//        CrpGameObject::Map gameObjects;
    };
}