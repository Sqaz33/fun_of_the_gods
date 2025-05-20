/**
 * @file subject.hpp
 * @brief Header file for the Subject interface in the Observer pattern.
 * 
 * Defines the interface for subjects that can be observed, allowing observers
 * to register for and receive notifications about specific events.
 */

#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <memory>
#include <unordered_map>

namespace observer {
    struct IObserver;  ///< Forward declaration of the Observer interface
}

namespace subject {

/**
 * @interface ISubject
 * @brief Interface for subjects in the Observer design pattern.
 * 
 * Classes implementing this interface can have observers register to be notified
 * when specific events occur. Maintains a collection of observers for each event type.
 */
struct ISubject {
    /**
     * @brief Attaches an observer to receive notifications for a specific event type.
     * @param obs Shared pointer to the observer to attach
     * @param event_t The event type to observe (from game_event::event_t)
     */
    virtual void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) = 0;
    
    /**
     * @brief Detaches an observer from receiving notifications for a specific event type.
     * @param obs Weak pointer to the observer to detach
     * @param event_t The event type to stop observing
     */
    virtual void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) = 0;    

    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~ISubject() = default;

protected:
    /**
     * @brief Notifies all observers registered for a specific event type.
     * @param event_t The event type that occurred
     * @param slf Weak pointer to this subject (to avoid shared_from_this in interface)
     */
    virtual void notify(int event_t, std::weak_ptr<ISubject> slf) = 0;

private:  
    std::unordered_multimap<
        int, std::weak_ptr<observer::IObserver>> obs_;  ///< Map of event types to observers
};

} // namespace subject

#endif // SUBJECT_HPP