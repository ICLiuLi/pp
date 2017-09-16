#include "login.h"
#include "client.h"
#include "friends.h"

#include "manager.h"

#include "icon.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
namespace pp {

struct login::Private {
    QLineEdit username;
    QLabel un;

    QLineEdit password;
    QLabel pw;

    QPushButton loginIn;
    QPushButton login_out;
    //QVBoxLayout mainLayout;

};
login::login(QWidget *parent)noexcept
    : super(LOGIN_WINDOW, parent) {
    impl = new Private;
    this->setObjectName("login");
    init();
}

login::~login() noexcept {
    delete impl;
}

void login::init() noexcept {
    // 网络初始化
    client::init();

    {// init layout
        auto mainLayout = new QVBoxLayout{ this };
        mainLayout->setMargin(ui::login::layoutMargin);
        mainLayout->addWidget(&impl->un, Qt::AlignLeft);
        mainLayout->addWidget(&impl->username);
        mainLayout->addWidget(&impl->pw, Qt::AlignLeft);
        mainLayout->addWidget(&impl->password);
        mainLayout->addStretch(1);
        mainLayout->addLayout([=]() {
            auto loginLayout = new QHBoxLayout;
            loginLayout->addWidget(&impl->loginIn);
            loginLayout->addWidget(&impl->login_out);
            return loginLayout;
        }());
    }

    {// setting model
        impl->password.setEchoMode(QLineEdit::Password);
        ///todo 界面字符从config读取
        impl->un.setText(QString::fromUtf16(u"<h1>:account</h1>"));
        impl->pw.setText(QString::fromUtf16(u"<h1>:password</h1>"));

#if 0
        auto pixmap = QPixmap("./icon/icon/ok_normal.png");
        impl->loginIn.setIconSize(pixmap.size());
        impl->loginIn.setIcon({ pixmap });
        impl->loginIn.setProperty("IconBackground", true);
#else
        impl->loginIn.setFixedSize({ ui::login::iconWidth,
                                   ui::login::iconHeight });
        impl->loginIn.setObjectName("login_in");

        impl->login_out.setFixedSize({ ui::login::iconWidth,
                                     ui::login::iconHeight });
        impl->login_out.setObjectName("login_out");
#endif
    }
    connect(&impl->loginIn, &QPushButton::clicked,
            this, [=]() {
        auto username = impl->username.text().trimmed();
        auto password = impl->password.text();


        ///点击登录
    });
    connect(&impl->login_out, &QPushButton::clicked,
            this, [=]() {
        manager::exit();
    });
    connect(client::getInstance(), &client::loginStatus,
            this, [=](client::LOGIN_STATUS status) {
        //登录状态
        switch (status) {
        case client::LOGIN_STATUS::SUCCESS:
            //登录成功
            manager::openFriends();
            break;
        default:
            break;
        }
    });
    this->setMinimumSize(ui::login::windowWidth,
                         ui::login::windowHeight);
}

}
