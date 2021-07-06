#include "Looper.hpp"

#include "logger.hpp"

namespace dai {

struct LooperMessage;
typedef struct LooperMessage LooperMessage;

struct LooperMessage
{
    int            what{ 0 };
    void          *obj { nullptr };
    obj_free       free_func{ nullptr };
    LooperMessage *next{ nullptr };
    bool           quit{ false };

    ~LooperMessage()
    {
        if(obj && free_func)
        {
            free_func(obj);
            obj = nullptr;
        }
    }
};

void *Looper::worker_route(void *context)
{
    auto *ptr = reinterpret_cast<Looper *>(context);
    ptr->running = true;
    ptr->loop();
    return nullptr;
}

void Looper::add_message(LooperMessage *message, bool flush)
{
    sem_wait(&headProtect);
    auto *h = head;
    if (flush)
    {
        while (h)
        {
            auto *next = h->next;
            delete h;
            h = next;
        }
        h = nullptr;
    }

    if (h)
    {
        while (h->next)
        {
            h = h->next;
        }
        h->next = message;
    }
    else
    {
        head = message;
    }

#ifdef DEBUG
    LOGD("add message: what=%d obj=%p", message->what, message->obj);
#endif

    sem_post(&headProtect);
    sem_post(&headAvailable);
}

void Looper::loop()
{
    while (true)
    {
        // wait for available message
        sem_wait(&headAvailable);

        // get next available message
        sem_wait(&headProtect);
        auto *msg = head;
        if (nullptr == msg)
        {
            LOGW("no available message in looper");
            sem_post(&headProtect);
            continue;
        }
        head = msg->next;
        sem_post(&headProtect);

        if (msg->quit)
        {
            LOGI("will quit the message looper");
            delete msg;
            return;
        }

        handle(msg->what, msg->obj);
        delete msg;
    }
}

Looper::Looper(): running{ false }, head{ nullptr }
{
    sem_init(&headProtect, 0, 0);
    sem_init(&headAvailable, 0, 1);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&worker, &attr, worker_route, this);
    pthread_attr_destroy(&attr);
}

Looper::~Looper()
{
    if (running)
    {
        LOGI("start destroy Looper");
        quit();
        LOGI("finish destroy Looper");
    }
}

void Looper::quit()
{
    LOGI("quit this message looper");
    auto *message = new LooperMessage();
    message->quit = true;

    add_message(message, false);

    void *result = nullptr;
    pthread_join(worker, &result);

    sem_destroy(&headProtect);
    sem_destroy(&headAvailable);

    running = false;
}

void Looper::post(int what, void *data, obj_free func, bool flush)
{
    auto *message = new LooperMessage();
    message->what = what;
    if (!data)
    {
        message->obj = data;
    }
    if(func)
    {
        message->free_func = func;
    }

    add_message(message, flush);
}

void Looper::handle(int what, void *data)
{
    // use the subclass to implement this function
}


}//namespace dai
