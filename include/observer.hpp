#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <memory>

namespace subject {
    struct ISubject;  
}

namespace observer {

struct IObserver {
    virtual void update(int event_t) = 0;
    
    virtual ~IObserver() = default;
};

} // namespace observer

#endif // OBSERVER_HPP