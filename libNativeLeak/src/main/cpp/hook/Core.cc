#include "Core.hpp"

HOOK_NS_START

    int hook_register(const char* regex_path, const char* symbol, void* func_new, void** func_old)
    {
    }

    int hook_ignore(const char* regex_path, const char* symbol)
    {
    }

    int hook_refresh(bool async)
    {
    }

    int hook_clear()
    {
    }


HOOK_NS_END
