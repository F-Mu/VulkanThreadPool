#pragma once

#include "window_system.hpp"
#include "render_device.hpp"
#include "render_swap_chain.hpp"
#include "function/framework/game_object.hpp"
#include "render_system.hpp"
#include "render_descriptors.hpp"
//std
#include <memory>
#include <vector>

namespace crp {
    class FirstApp {
    public:
        FirstApp();

        ~FirstApp();

        FirstApp(const FirstApp &) = delete;

        FirstApp &operator=(const FirstApp &) = delete;

        void run();

    private:
        void test();
    };
}