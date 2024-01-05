
// observer.h

#ifndef OBSERVER_H
#define OBSERVER_H

typedef enum{
    PROPAGATION_COMPLETE_EVENT,
    LEARNING_COMPLETE_EVENT,
    CONFLICT_ANALYSIS_COMPLETE_EVENT,
    DECISION_COMPLETE_EVENT,
    BACKTRACK_COMPLETE_EVENT
} EventType;

typedef struct {
    EventType event_type;
    void (*notify)(EventType);
} Observable;

typedef struct {
    void (*handle_event)(EventType);
} Observer;

void notify_event(Observable* observable);

#endif /* OBSERVER_H */
