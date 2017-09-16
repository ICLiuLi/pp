#pragma once
#include "model.h"
#include "window.h"

namespace pp {
class manager {

private:
    struct Private;
    Private* impl;
private:
    manager();
public:
    static void exit();
    static manager* getInstance();
    static QWidget* openLogin();
    static QWidget* openFriends();
    static QWidget* openTalk(userID uid);
    static void init();
private:
    template<class T>
    static QWidget* openUniqueWindow(T* window);
    template<class T>
    static QWidget* openRepetWindow(T* window);
    static QWidget* openMultiWindow(window::WINDOW_TYPE type);
    static QWidget* openSingleWindow(window::WINDOW_TYPE type);
};

template<class T>
inline QWidget * manager::openUniqueWindow(T * window) {
    const auto p = getInstance();
    delete p->impl->window[ window::UNIQUE_WINDOW ];
    p->impl->window[ window::UNIQUE_WINDOW ] = window;
    p->impl->window[ window::UNIQUE_WINDOW ]->show();
    return p->impl->window[ window::UNIQUE_WINDOW ];
}

template<class T>
inline QWidget* manager::openRepetWindow(T* window) {
    const auto p = getInstance();
    p->impl->window.emplace_back(window);
    return window;
}

}
