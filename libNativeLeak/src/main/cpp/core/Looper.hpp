#pragma once

#include <pthread.h>
#include <semaphore.h>

namespace dai {

struct LooperMessage;
typedef void(*obj_free)(void *obj);

class Looper {
private:
    bool           running;
    LooperMessage *head;
    pthread_t      worker;
    sem_t          headProtect;
    sem_t          headAvailable;

private:
    void add_message(LooperMessage *message, bool flush);
    void loop();

    static void *worker_route(void *context);

public:
    Looper();
    virtual ~Looper();

    void quit();
    void post(int what, void *data, obj_free func = nullptr, bool flush = false);

    virtual void handle(int what, void *data);

private:
    Looper &operator=(const Looper &) = delete;
    Looper(Looper &)                  = delete;

};


}//namespace dai



