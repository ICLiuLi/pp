#pragma once

#include<QWidget>

namespace pp {
class window :public QWidget {
public:
    enum WINDOW_TYPE {
        UNIQUE_WINDOW = 0,
        NORMAL_WINDOW,
        //Ψһ����
        LOGIN_WINDOW,
        FRIENDS_WINDOW,

        //�ظ�����
        TALK_WINDOW,

        WINDOW_COUNT,
    };
private:
    struct Private;
    Private* impl;
    using super = QWidget;
public:
    explicit window(WINDOW_TYPE type = NORMAL_WINDOW, QWidget* parent = nullptr)noexcept;
    constexpr WINDOW_TYPE getWindowType()const noexcept;
    virtual ~window()noexcept;
};

}
