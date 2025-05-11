#include "subject.hpp"
#include "observer.hpp"

#include <algorithm>

namespace subject {

void ISubject::attach(
    std::shared_ptr<observer::IObserver> obs, int event_t) 
{ obs_.emplace(event_t, obs); }

void ISubject::detach(
    std::weak_ptr<observer::IObserver> obs, int event_t) 
{   
    if (obs.expired()) return;
    auto lk = obs.lock();
    auto [begin, end] = obs_.equal_range(event_t);
    auto pred = [lk] (auto&& p) { 
        auto sec = p.second;
        if (sec.expired()) return false; 
        return sec.lock() == lk; 
    };
    if (auto el = std::find_if(begin, end, pred); el != end) {
        obs_.erase(el);
    }
}

void ISubject::notify(int event_t, std::weak_ptr<ISubject> slf) {
    auto [it, end] = obs_.equal_range(event_t);
    while (it != end) {
        if (it->second.expired())  {
            it = obs_.erase(it);
        } else { 
            auto sptr = it->second.lock();
            sptr->update(slf, event_t);
            ++it;
        } 
    }
}


} // namespace subject
 