#ifndef LOCK_STATE_H
#define LOCK_STATE_H

#include "list/list.h"
#include "utils/oidc_error.h"

struct lock_state {
  short          locked;
  struct hashed* hash;
};

oidc_error_t unlock(list_t* loaded, const char* password);
oidc_error_t lock(list_t* loaded, const char* password);

#endif  // LOCK_STATE_H
