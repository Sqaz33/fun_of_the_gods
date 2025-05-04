#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <memory>

#include "subject.hpp"

namespace observer {

struct IObserver {
    virtual void update(
        std::weak_ptr<subject::ISubject> subj, int event_t) = 0; 
    virtual ~IObserver() {}
};

} // namespace observer


#endif // OBSERVER_HPP