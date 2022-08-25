#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <stack>
#include <thread>

template<typename T>
class threadsafe_queue {

private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:

    threadsafe_queue();

    threadsafe_queue(const threadsafe_queue &);

    void push(T);

    void wait_and_pop(T &);

    std::shared_ptr<T> wait_and_pop();

    bool try_pop(T &);

    std::shared_ptr<T> try_pop();

    bool empty() const;

};

struct empty_stack : std::exception {};

template<typename T>
class threadsafe_stack {

private:
    std::stack<T> data;
    mutable std::mutex mut;

public:

    threadsafe_stack();

    threadsafe_stack(const threadsafe_stack &);

    threadsafe_stack &operator=(const threadsafe_stack &);

    void push(T);

    std::shared_ptr<T> pop();

    void pop(T &);

    bool empty() const;

};
