#pragma once

#include "namespace.hpp"
#include "Error.hpp"

HOOK_NS_START

int hook_register(const char *regex_path, const char *symbol, void *func_new, void **func_old) HOOK_API;
int hook_ignore  (const char *regex_path, const char *symbol) HOOK_API;
int hook_refresh (bool async) HOOK_API;
int hook_clear   () HOOK_API;

HOOK_NS_END

