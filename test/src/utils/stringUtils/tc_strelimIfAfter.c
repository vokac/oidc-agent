#include "tc_strelimIfAfter.h"

#include "utils/stringUtils.h"

// TODO
START_TEST(test_) { ck_assert_msg(1 == 1, ""); }
END_TEST

TCase* test_case_strelimIfAfter() {
  TCase* tc = tcase_create("strelimIfAfter");
  tcase_add_test(tc, test_);
  // TODO
  return tc;
}
