#pragma once

#include "window_system.hpp"
#include "crp_device.hpp"
#include "crp_swap_chain.hpp"
#include "function/framework/crp_game_obejct.hpp"
#include "render_system.hpp"
#include "crp_descriptors.hpp"
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