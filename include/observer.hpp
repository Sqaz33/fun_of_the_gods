/**
 * @file observer.hpp
 * @brief Header file for the Observer interface.
 * 
 * Defines the Observer interface used in the Observer design pattern
 * for event notification between components.
 */

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <memory>

namespace subject {
    struct ISubject;  ///< Forward declaration of the Subject interface
}

namespace observer {

/**
 * @interface IObserver
 * @brief Interface for objects that observe and react to subject changes.
 * 
 * This interface is part of the Observer design pattern. Classes implementing
 * this interface can register with subjects to receive notifications when
 * the subject's state changes.
 */
struct IObserver {
    /**
     */
    virtual void update(int event_t) = 0;
    
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~IObserver() = default;
};

} // namespace observer

#endif // OBSERVER_HPP