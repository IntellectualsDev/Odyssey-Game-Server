//
// Created by Anshul Gowda on 2/24/24.
//

#ifndef ODYSSEYGAMESERVER_THREADSAFEDATA_H
#define ODYSSEYGAMESERVER_THREADSAFEDATA_H

#include <shared_mutex>
#include <mutex>

using namespace std;

class ThreadSafeData {
public:
    ThreadSafeData() = default;
    virtual ~ThreadSafeData() = default;

    // delete both the copy constructor and copy assignment operator to prevent copying
    ThreadSafeData(const ThreadSafeData&) = delete;
    ThreadSafeData& operator=(const ThreadSafeData&) = delete;

protected:
    mutable std::shared_mutex mutex;

    template<typename Action>
    auto readAction(Action action) const -> decltype(action()) {
        shared_lock<shared_mutex> lock(mutex);
        return action();
    }

    template <typename Action>
    auto writeAction(Action action) -> decltype(action()) {
        unique_lock<shared_mutex> lock(mutex);
        return action();
    }
};


#endif //ODYSSEYGAMESERVER_THREADSAFEDATA_H
