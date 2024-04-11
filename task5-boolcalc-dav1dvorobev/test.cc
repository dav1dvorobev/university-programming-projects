#include <string>
#include <gtest/gtest.h>

#include "boolexpr.h"

TEST(create, cnf) {
    EXPECT_EQ(std::string(BooleanExpression("x1 & x2").cnf()), "(((x1 v x2) & (x1 v ~x2)) & (~x1 v x2))");
}
TEST(create, dnf) {
    EXPECT_EQ(std::string(BooleanExpression("x1 = x2").dnf()), "((~x1 & ~x2) v (x1 & x2))");
}
TEST(create, zhegalkin) {
    EXPECT_EQ(std::string(BooleanExpression("x1 + x2 + x3").zhegalkin()), "((x3 + x2) + x1)");
}