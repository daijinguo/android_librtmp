#pragma once

#include "namespace.hpp"
#include "core/safe_queue.hpp"

#include <dlfcn.h>
#include <unwind.h>
#include <inttypes.h>

#include <memory>
#include <list>

#if defined(__LP64__)
    #define HK_FMT "16" PRIxPTR
#else
    #define HK_FMT  "8" PRIxPTR
#endif

HOOK_NS_START

class StackElement {
private:
    std::shared_ptr<Dl_info> dlInfo_ = std::make_shared<Dl_info>();
    uintptr_t                pc_;
    uint32_t                 dlAddress_;

public:
    StackElement(uintptr_t pc) : pc_{pc}
    {
    }

    intptr_t get_pc() const
    {
        return pc_;
    }

    int32_t getDlAddr() const
    {
        return dlAddress_;
    }

    std::shared_ptr<Dl_info> get_DLInfo() const
    {
        return dlInfo_;
    }

    void dladdr();
};



class Backtrace {
private:
    std::shared_ptr<std::list<std::shared_ptr<StackElement>>> stackElementList = std::make_shared<std::list<std::shared_ptr<StackElement>>>();
    size_t                                                    max_size;
    size_t                                                    skip_size;
    bool                                                      isDlAddr{false};

public:
    explicit Backtrace(size_t max): max_size{max}, skip_size{0}
    {
    }

    explicit Backtrace(size_t max, size_t skip): max_size{max}, skip_size{skip}
    {
    }

    void dladdr();

    inline bool isFull() const
    {
        return stackElementList->size() >= max_size;
    }

    bool skip()
    {
        if(skip_size > 0)
        {
            skip_size--;
            return true;
        }
        return false;
    }

    void push(uintptr_t pc) const
    {
        stackElementList->push_back(std::make_shared<StackElement>(StackElement(pc)));
    }

};



HOOK_NS_END
