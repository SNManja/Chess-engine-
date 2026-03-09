#include <catch2/catch_test_macros.hpp>
#include "core/engine.hpp"

TEST_CASE("engine::add suma correctamente") {
    REQUIRE(engine::add(2, 3) == 5);
    REQUIRE(engine::add(-1, 1) == 0);
}