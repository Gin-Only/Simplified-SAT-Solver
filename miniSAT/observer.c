// observer.c

#include "observer.h"
#include <stdlib.h>

// 在这里定义观察者对象的结构
typedef struct {
    void (*handle_event)(EventType);
} ObserverImpl;

// 在这里定义被观察者对象的结构
typedef struct {
    EventType event_type;
    ObserverImpl* observer;
} ObservableImpl;

// 创建被观察者对象并初始化
Observable* create_observable(EventType event_type) {
    ObservableImpl* observable = (ObservableImpl*)malloc(sizeof(ObservableImpl));
    observable->event_type = event_type;
    observable->observer = NULL;
    return (Observable*)observable;
}

// 创建观察者对象并初始化
Observer* create_observer(void (*handle_event)(EventType)) {
    ObserverImpl* observer = (ObserverImpl*)malloc(sizeof(ObserverImpl));
    observer->handle_event = handle_event;
    return (Observer*)observer;
}

// 将观察者注册到被观察者
void register_observer(Observable* observable, Observer* observer) {
    if (observable != NULL && observer != NULL) {
        ((ObservableImpl*)observable)->observer = (ObserverImpl*)observer;
    }
}

// 通知观察者事件发生
void notify_event(Observable* observable) {
    if (observable != NULL) {
        EventType event_type = ((ObservableImpl*)observable)->event_type;
        ObserverImpl* observer = ((ObservableImpl*)observable)->observer;

        if (observer != NULL && observer->handle_event != NULL) {
            observer->handle_event(event_type);
        }
    }
}

// 销毁观察者
void destroy_observer(Observer* observer) {
    if (observer != NULL) {
        free(observer);
    }
}

// 销毁被观察者
void destroy_observable(Observable* observable) {
    if (observable != NULL) {
        free(observable);
    }
}
