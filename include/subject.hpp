#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <memory>
#include <unordered_map>

namespace observer {
    struct IObserver;
}

namespace subject {

struct ISubject {
    virtual void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) = 0;
    virtual void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) = 0;    

    virtual ~ISubject() {}

protected:
    virtual void notify(int event_t, std::weak_ptr<ISubject> slf) = 0;

private:  
    std::unordered_multimap<
        int, std::weak_ptr<observer::IObserver>> obs_;
};

} // namespace subject


#endif // SUBJECT_HPP