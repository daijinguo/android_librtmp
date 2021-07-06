#include "Stack.hpp"

HOOK_NS_START

void StackElement::dladdr()
{
    ::dladdr((void *)pc_, dlInfo_.get());
}

void Backtrace::dladdr()
{
    if(isDlAddr)
        return;

    isDlAddr = true;
    for(const auto &element : *stackElementList)
    {
        element->dladdr();
    }
}

HOOK_NS_END
