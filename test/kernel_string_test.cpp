#include "../src/lib/kernel_string.h"
#include <gtest/gtest.h>

TEST(KernelStringTest, AppendWorks) {
  char buffer[64];
  lib::KernelString ks(buffer, sizeof(buffer));

  ks.append("Fabio");
  ks.append(" rocks!");

  EXPECT_STREQ(ks.c_str(), "Fabio rocks!");
  EXPECT_EQ(ks.length(), strlen("Fabio rocks!"));
}
