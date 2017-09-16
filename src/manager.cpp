#include "manager.h"

#include "login.h"
#include "friends.h"
#include "talk.h"

#include <QApplication>

#include <map>
#include <vector>
namespace pp {

static manager* ptr = nullptr;

struct manager::Private {
    std::vector<QWidget*>window;
};
manager::manager(){
    impl = new Private;
    impl->window = std::move(
        std::vector<QWidget*>(window::WINDOW_COUNT, nullptr)
    );
}

void manager::exit() {
    QApplication::exit();
}

manager * manager::getInstance() {
    if (ptr == nullptr)
        ptr = new manager;
    return ptr;
}

void manager::init() {
    openLogin();
}

QWidget * manager::openMultiWindow(window::WINDOW_TYPE type) {
    switch (type) {
    case window::LOGIN_WINDOW:
        return openLogin();
    case window::FRIENDS_WINDOW:
        return openFriends();
    default:
        return nullptr;
    }
}


QWidget * manager::openSingleWindow(window::WINDOW_TYPE type) {
    const auto p = getInstance();
    if (p->impl->window[ type ])
        return p->impl->window[ type ];
    else
        return openMultiWindow(type);
}

QWidget * manager::openLogin() {
    return openUniqueWindow(new login);
}

QWidget * manager::openFriends() {
    return openUniqueWindow(new friends);
}

QWidget * manager::openTalk(userID uid) {
    return openRepetWindow(new talk(uid));
}

}
