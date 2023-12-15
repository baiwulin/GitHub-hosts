#include <QApplication>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class AutoStartProgram : public QWidget {
public:
    AutoStartProgram(QWidget *parent = nullptr) : QWidget(parent) {
        // 创建界面控件
        autoStartCheckBox = new QCheckBox("开机自启", this);
        selectComboBox = new QComboBox(this);
        selectComboBox->addItem("选择");
        selectComboBox->addItem("自动");
        cancelBtn = new QPushButton("取消", this);
        confirmBtn = new QPushButton("确认", this);

        // 设置控件位置
        autoStartCheckBox->move(10, 10);
        selectComboBox->move(10, 40);
        cancelBtn->move(10, 80);
        confirmBtn->move(90, 80);

        // 信号与槽连接
        connect(confirmBtn, &QPushButton::clicked, this, &AutoStartProgram::onConfirmClicked);

        // 创建网络请求管理器
        networkManager = new QNetworkAccessManager(this);

        // 定时器，每小时请求一次更新数据
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &AutoStartProgram::checkForUpdates);
        timer->start(60 * 60 * 1000);  // 1小时

        // 设置窗口标题和大小
        setWindowTitle("自动启动程序");
        resize(200, 120);
    }

private slots:
    void onConfirmClicked() {
        if (autoStartCheckBox->isChecked()) {
            // TODO: 设置开机自启

            // 申请管理员权限

            // 如果无权限，自动退出
            // QMessageBox::critical(this, "错误", "无法申请管理员权限");
            // qApp->quit();
        }

        if (selectComboBox->currentText() == "自动") {
            // 请求更新数据
            checkForUpdates();
        }
    }

    void checkForUpdates() {
        // 发送GET请求获取更新数据
        QUrl url("http://api");
        QNetworkRequest request(url);
        QNetworkReply *reply = networkManager->get(request);
        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
                QJsonObject jsonObj = jsonDoc.object();

                if (jsonObj.value("version").toInt() != 0) {
                    QMessageBox::information(this, "更新", "有新版本可用！");
                }
            }
            reply->deleteLater();
        });
    }

private:
    QCheckBox *autoStartCheckBox;
    QComboBox *selectComboBox;
    QPushButton *cancelBtn;
    QPushButton *confirmBtn;
    QNetworkAccessManager *networkManager;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    AutoStartProgram program;
    program.show();

    return app.exec();
}
