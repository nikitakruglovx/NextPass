#include "PassControll.h"
#include "ui_passcontroll.h"
#include "ErrorWindow.h"

#include <QTimer>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QMessageBox>


PassControll::PassControll(QWidget *parent) : QWidget(parent), ui(new Ui::PassControll), datas(new data::DataConnection)
{

    ui->setupUi(this);

    setObjectName("borderframeless");
    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);

    ErrorWindow errorWindow;

    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    QString val;

    f.open(QIODevice::ReadOnly | QIODevice::Text);
    val = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QString load_id = json["id"].toString();

    db = QSqlDatabase::addDatabase(datas->DBMS);
    db.setDatabaseName(datas->DB_NAME);
    db.setUserName(datas->DB_USERNAME);
    db.setHostName(datas->DB_HOST);
    db.setPort(datas->DB_PORT);
    db.setPassword(datas->DB_PASSWORD);
    QSqlQuery query(db);

    if (db.open()) {
        query.exec("SELECT data_title FROM data_users WHERE fk_user_id =\'"+ load_id +"\'");
        while (query.next()) {
            QString marker = query.value(0).toString();

            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QIcon(":/new/userskey/Resources/assets/userkey/key16x16.png"));
            item->setText(marker);
            ui->listWidget->addItem(item);
        }
        query.exec("SELECT notes FROM data_users_notes WHERE fk_user_id =\'"+ load_id +"\'");
        while (query.next()) {
            QString marker = query.value(0).toString();

            QListWidgetItem *itemNotes = new QListWidgetItem;
            itemNotes->setIcon(QIcon(":/new/userskey/Resources/assets/userkey/sticky-note.png"));
            if (marker.length() > 20) {
                itemNotes->setText(marker.mid(0,20).append("..."));
                ui->listWidget_2->addItem(itemNotes);
            }
            else {
                itemNotes->setText(marker);
                ui->listWidget_2->addItem(itemNotes);
            }

        }
    }
    else {
        errorWindow.show();
    }

    QTimer::singleShot(1000000, [this] {
        ui->stackedWidget->setCurrentIndex(0);
    });

}

PassControll::~PassControll()
{
    delete ui;
}

void PassControll::mousePressEvent(QMouseEvent *event) {
    curs = event->globalPos();

}

void PassControll::mouseMoveEvent(QMouseEvent *event) {

    new_curs = QPoint(event->globalPos() - curs);
    move(x() + new_curs.x(), y() + new_curs.y());
    curs = event->globalPos();

}

void PassControll::on_closeButton_clicked() {
    this->close();
}

void PassControll::on_minimizeButton_clicked() {
    this->showMinimized();
}

void PassControll::on_hideandseeButton_clicked() {

    if (ui->hideandseeButton->isChecked()) {
        ui->passEdit->setEchoMode(QLineEdit::Password);
        ui->hideandseeButton->setIcon(QIcon(":/new/icon/Resources/assets/icon/hidden.png"));
    }
    else {
        ui->passEdit->setEchoMode(QLineEdit::Normal);
        ui->hideandseeButton->setIcon(QIcon(":/new/icon/Resources/assets/icon/eye.png"));
    }
}

void PassControll::validationPass() {

    QSqlQuery query;
    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    QString val;

    if (ui->passEdit->text() == "") {
        ui->passEdit->setStyleSheet("border: 1px solid rgb(255, 74, 92); border-radius: 14px; font: 18pt 'Nirmala UI'; color: rgb(255, 74, 92);");
        QTimer::singleShot(3000, [this] {
            ui->passEdit->setStyleSheet("border: 1px solid '#30C193'; border-radius: 14px; font: 18pt 'Nirmala UI'; color: '#30C193';");
        });
    }
    else {
        QString pass = ui->passEdit->text();

        if(load_profile.exists() && load_profile.isFile()) {

            f.open(QIODevice::ReadOnly | QIODevice::Text);
            val = f.readAll();
            f.close();

            QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
            QJsonObject json = doc.object();
            QString load_id = json["id"].toString();

            QByteArray passByte = pass.toUtf8();
            QString HDpass_value = QCryptographicHash::hash(passByte, QCryptographicHash::Sha256).toHex();

            query.exec("SELECT user_password FROM users WHERE user_id =\'"+load_id+"\'");

            while (query.next()) {
                QString pass_value = query.value(0).toString();

                if (pass_value != HDpass_value) {
                    ui->passEdit->setStyleSheet("border: 1px solid rgb(255, 74, 92); border-radius: 14px; font: 18pt 'Nirmala UI'; color: rgb(255, 74, 92);");
                    QTimer::singleShot(3000, [this] {
                        ui->passEdit->setStyleSheet("border: 1px solid '#30C193'; border-radius: 14px; font: 18pt 'Nirmala UI'; color: '#30C193';");
                    });
                }
                else {
                    ui->stackedWidget->setCurrentIndex(1);
                    ui->passEdit->clear();
                    QTimer::singleShot(1000000, [this] {
                        ui->stackedWidget->setCurrentIndex(0);
                    });
                }
            }

        }

    }
}

void PassControll::on_passEdit_returnPressed() {
    validationPass();
}

void PassControll::on_passwordsButton_clicked() {

    ui->stackedWidget_2->setCurrentIndex(0);
    ui->passwordsButton->setStyleSheet("background-color: rgb(23, 33, 43); border: 1px solid rgb(23, 33, 43); border-radius: 13px;color: rgb(141, 151, 173);font: 700 12pt 'Nirmala UI';");
    ui->notesButton->setStyleSheet("#notesButton {background-color: rgb(31, 41, 54); border: none; color: rgb(141, 151, 173); font: 700 12pt 'Nirmala UI';} #notesButton:hover { background-color: rgb(23, 33, 43); border: 1px solid rgb(23, 33, 43);border-radius: 13px; }");
}

void PassControll::on_notesButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->notesButton->setStyleSheet("background-color: rgb(23, 33, 43); border: 1px solid rgb(23, 33, 43); border-radius: 13px;color: rgb(141, 151, 173);font: 700 12pt 'Nirmala UI';");
    ui->passwordsButton->setStyleSheet("#passwordsButton {background-color: rgb(31, 41, 54); border: none; color: rgb(141, 151, 173); font: 700 12pt 'Nirmala UI';} #passwordsButton:hover { background-color: rgb(23, 33, 43); border: 1px solid rgb(23, 33, 43);border-radius: 13px; }");
}

void PassControll::on_addItemButton_clicked() {

    ui->stackedWidget_3->setCurrentIndex(2);

}

void PassControll::on_clearButton_clicked() {

    ui->Titleedit->clear();
    ui->PasswordEdit->clear();
    ui->LinkUrlEdit->clear();
    ui->signEdit->clear();
    ui->stackedWidget_3->setCurrentIndex(0);
}

void PassControll::AddItem() {

    if (ui->Titleedit->text() == "") {
        ui->Titleedit->setStyleSheet("border: 1.5px solid rgb(255, 74, 92); border-radius: 8px; font: 18pt 'Nirmala UI'; color: rgb(255, 74, 92);");
        QTimer::singleShot(2000, [this] {
            ui->Titleedit->setStyleSheet("border: 1.5px solid '#30C193'; border-radius: 8px; font: 18pt 'Nirmala UI'; color: '#30C193';");
        });
    }
    else if (ui->UsernameEdit->text() == "") {
        ui->label_8->setStyleSheet("font: 11pt 'Nirmala U'; color: rgb(255, 74, 92);");
        QTimer::singleShot(2000, [this] {
            ui->label_8->setStyleSheet("font: 11pt 'Nirmala UI'; color: rgb(141, 151, 173);");
        });
    }
    else {

        QFileInfo load_profile(PROFILEPATH);
        QFile f(PROFILEPATH);
        QString val;
        QSqlQuery query(db);
        QString title = ui->Titleedit->text();
        QString username = ui->UsernameEdit->text();
        QString password = ui->PasswordEdit->text();
        QString link = ui->LinkUrlEdit->text();
        QString sign = ui->signEdit->text();

        f.open(QIODevice::ReadOnly | QIODevice::Text);
        val = f.readAll();
        f.close();

        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject json = doc.object();
        QString load_id = json["id"].toString();

        query.exec("SELECT data_title FROM data_users WHERE data_title =\'" + title + "\' AND fk_user_id =\'" + load_id + "\' ");

        if (query.next() == false) {

            query.prepare("INSERT INTO data_users(data_title, data_username, data_password, data_link, data_signature, fk_user_id)VALUES(:data_title, :data_username, :data_password, :data_link, :data_signature, :fk_user_id);");
            query.bindValue(":data_title", title);
            query.bindValue(":data_username", username);
            query.bindValue(":data_password", password);
            query.bindValue(":data_link", link);
            query.bindValue(":data_signature", sign);
            query.bindValue(":fk_user_id", load_id);
            query.exec();

            ui->Titleedit->clear();
            ui->UsernameEdit->clear();
            ui->PasswordEdit->clear();
            ui->LinkUrlEdit->clear();
            ui->signEdit->clear();
            ui->listWidget->addItem(new QListWidgetItem(QIcon(":/new/userskey/Resources/assets/userkey/key16x16.png"), title));
            ui->stackedWidget_3->setCurrentIndex(0);
        }
        else {
           ui->label_17->setText("An entry with this title is already present!");
           QTimer::singleShot(2000, [this] {
               ui->label_17->clear();
           });
        }
    }
}

void PassControll::on_addsaveButton_clicked() {
    AddItem();
}

void PassControll::on_Titleedit_returnPressed() {
    AddItem();

}
void PassControll::on_UsernameEdit_returnPressed() {
    AddItem();

}
void PassControll::on_PasswordEdit_returnPressed() {
    AddItem();

}

void PassControll::on_hideandseeButton_2_clicked() {

    if (ui->hideandseeButton_2->isChecked()) {
        ui->PasswordEdit_2->setEchoMode(QLineEdit::Normal);
        ui->hideandseeButton_2->setIcon(QIcon(":/new/icon/Resources/assets/icon/hidden.png"));
    }
    else {
        ui->PasswordEdit_2->setEchoMode(QLineEdit::Password);
        ui->hideandseeButton_2->setIcon(QIcon(":/new/icon/Resources/assets/icon/eye.png"));
    }

}

void PassControll::on_hideandseeButton_3_clicked() {

    if (ui->hideandseeButton_3->isChecked()) {
        ui->PasswordEdit->setEchoMode(QLineEdit::Normal);
        ui->hideandseeButton_3->setIcon(QIcon(":/new/icon/Resources/assets/icon/hidden.png"));
    }
    else {
        ui->PasswordEdit->setEchoMode(QLineEdit::Password);
        ui->hideandseeButton_3->setIcon(QIcon(":/new/icon/Resources/assets/icon/eye.png"));
    }

}

void PassControll::on_listWidget_itemClicked(QListWidgetItem *item) {

    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    QString val;

    f.open(QIODevice::ReadOnly | QIODevice::Text);
    val = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QString load_id = json["id"].toString();
    QSqlQuery query(db);

    query.exec("SELECT data_title, data_username, data_password, data_link, data_signature FROM data_users WHERE data_title =\'"
               + item->text() + "\' AND fk_user_id =\'" + load_id + "\' ");

    while (query.next()) {

        QString title = query.value(0).toString();
        QString username = query.value(1).toString();
        QString pass = query.value(2).toString();
        QString link = query.value(3).toString();
        QString sign = query.value(4).toString();

        ui->stackedWidget_3->setCurrentIndex(1);
        ui->Titleedit_2->setText(title);
        ui->UsernameEdit_2->setText(username);
        ui->PasswordEdit_2->setText(pass);
        ui->LinkUrlEdit_2->setText(link);
        ui->signEdit_2->setText(sign);

    }
}

void PassControll::on_listWidget_2_itemClicked(QListWidgetItem *itemNotes) {
    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    QString val;

    f.open(QIODevice::ReadOnly | QIODevice::Text);
    val = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QString load_id = json["id"].toString();
    QSqlQuery query;
    if (itemNotes->text().length() > 20) {
        QString streck = itemNotes->text().mid(0,20);

        query.exec("SELECT notes FROM data_users_notes WHERE notes LIKE \'%" + streck + "%\' AND fk_user_id =\'" + load_id + "\' ");

        while (query.next()) {
            QString notes = query.value(0).toString();
            ui->stackedWidget_4->setCurrentIndex(0);
            ui->textEdit_2->setText(notes);
        }
    }
    else {
        query.exec("SELECT notes FROM data_users_notes WHERE notes =\'" + itemNotes->text() + "\' AND fk_user_id =\'" + load_id + "\' ");

        while (query.next()) {
            QString notes = query.value(0).toString();
            ui->stackedWidget_4->setCurrentIndex(0);
            ui->textEdit_2->setText(notes);
        }
    }

}

void PassControll::on_deleteButton_clicked() {

    QSqlQuery query(db);
    QString title = ui->Titleedit_2->text();
    QList<QListWidgetItem*> item = ui->listWidget->selectedItems();

    foreach(QListWidgetItem * item, item)
    {
        query.exec("DELETE FROM data_users WHERE data_title =\'" + item->text() + "\' ");
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
        ui->stackedWidget_3->setCurrentIndex(0);
    }
}

void PassControll::Updater() {

    QSqlQuery query(db);
    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    QString val;

    f.open(QIODevice::ReadOnly | QIODevice::Text);
    val = f.readAll();
    f.close();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QString load_id = json["id"].toString();
    QList<QListWidgetItem*> item = ui->listWidget->selectedItems();

    foreach(QListWidgetItem * item, item)
    {

        query.exec("UPDATE data_users SET data_title = \'" + ui->Titleedit_2->text() +
                   "\', data_username = \'" + ui->UsernameEdit_2->text() + "\', data_password =\'"
                   + ui->PasswordEdit_2->text() + "\', data_link = \'" + ui->LinkUrlEdit_2->text()
                   + "\', data_signature = \'" + ui->signEdit_2->text() + "\' WHERE fk_user_id = \'"
                   + load_id + "\' AND data_title = \'" + item->text() + "\' ");
        item->setText(ui->Titleedit_2->text());

    }

}

void PassControll::on_updateButton_clicked() {
    Updater();

}

void PassControll::on_Titleedit_2_returnPressed() {
    Updater();
}

void PassControll::on_UsernameEdit_2_returnPressed() {
    Updater();
}

void PassControll::on_PasswordEdit_2_returnPressed() {
    Updater();
}

void PassControll::on_lineEdit_returnPressed() {

    QSqlQuery query(db);
    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    QString val;

    f.open(QIODevice::ReadOnly | QIODevice::Text);
    val = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QString load_id = json["id"].toString();
    QString SearchQuery = ui->lineEdit->text();
    SearchQuery = SearchQuery.trimmed();

    query.exec("SELECT data_title FROM data_users WHERE data_title LIKE \'%" + SearchQuery + "%\' AND fk_user_id =\'" + load_id + "\' ");
    if (SearchQuery == "") {

        ui->listWidget->clear();

        query.exec("SELECT data_title FROM data_users WHERE fk_user_id =\'"+ load_id +"\'");
        while (query.next()) {
            QString marker = query.value(0).toString();
            QListWidgetItem *item = new QListWidgetItem;

            item->setIcon(QIcon(":/new/userskey/Resources/assets/userkey/key16x16.png"));
            item->setText(marker);

            ui->listWidget->addItem(item);
        }
    }
    else {
        while (query.next()) {
            ui->listWidget->clear();
            QString searchingItem = query.value(0).toString();
            QListWidgetItem *item = new QListWidgetItem;

            item->setIcon(QIcon(":/new/userskey/Resources/assets/userkey/key16x16.png"));
            item->setText(searchingItem);
            ui->listWidget->addItem(item);
        }
    }
}

void PassControll::AddNotes() {
    if (ui->textEdit->toPlainText() == "") {
        ui->textEdit->setStyleSheet("border: 1.5px solid '#e04f5f'; border-radius: 8px;font: 18pt 'Nirmala UI'; color: rgb(255, 255, 255);");
        QTimer::singleShot(2000, [this] { ui->textEdit->setStyleSheet("border: 1.5px solid '#30C193'; border-radius: 8px;font: 18pt 'Nirmala UI'; color: rgb(255, 255, 255);"); });
    }
    else {
        QFileInfo load_profile(PROFILEPATH);
        QFile f(PROFILEPATH);
        QString val;
        QSqlQuery query(db);
        QString notes = ui->textEdit->toPlainText(); notes = notes.trimmed();
        int notesLen = ui->textEdit->toPlainText().length();

        f.open(QIODevice::ReadOnly | QIODevice::Text);
        val = f.readAll();
        f.close();

        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject json = doc.object();
        QString load_id = json["id"].toString();

        query.exec("SELECT notes FROM data_users_notes WHERE notes =\'" + notes + "\' AND fk_user_id =\'" + load_id + "\' ");

        if (query.next() == false) {
            query.prepare("INSERT INTO data_users_notes(notes, fk_user_id)VALUES(:notes, :fk_user_id);");
            query.bindValue(":notes", notes);
            query.bindValue(":fk_user_id", load_id);
            query.exec();

            ui->textEdit->clear();
            if (notesLen > 15) {
                ui->listWidget_2->addItem(new QListWidgetItem(QIcon(":/new/userskey/Resources/assets/userkey/sticky-note.png")
                                                              , notes.mid(0,20).append("...")));
                ui->stackedWidget_4->setCurrentIndex(1);
            }
            else {
                ui->listWidget_2->addItem(new QListWidgetItem(QIcon(":/new/userskey/Resources/assets/userkey/sticky-note.png"), notes));
                ui->stackedWidget_4->setCurrentIndex(1);
            }
        }
        else {
            ui->textEdit->setStyleSheet("border: 1.5px solid '#e04f5f'; border-radius: 8px;font: 18pt 'Nirmala UI'; color: rgb(255, 255, 255);");
            QTimer::singleShot(2000, [this] { ui->textEdit->setStyleSheet("border: 1.5px solid '#30C193'; border-radius: 8px;font: 18pt 'Nirmala UI'; color: rgb(255, 255, 255);"); });
        }
    }
}

void PassControll::on_addNotesButton_clicked() {
    ui->stackedWidget_4->setCurrentIndex(2);
}

void PassControll::on_clearButton_2_clicked() {
    ui->stackedWidget_4->setCurrentIndex(1);
    ui->textEdit->clear();
}

void PassControll::on_addsaveButton_2_clicked() {
    AddNotes();
}

void PassControll::UpdateNotes() {

    QSqlQuery query(db);
    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    QString val;

    f.open(QIODevice::ReadOnly | QIODevice::Text);
    val = f.readAll();
    f.close();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QString load_id = json["id"].toString();
    QList<QListWidgetItem*> notesItem = ui->listWidget_2->selectedItems();
    QString NotesValue = ui->textEdit_2->toPlainText();
    NotesValue = NotesValue.trimmed();

    foreach(QListWidgetItem * notesItem, notesItem)
    {
        if (notesItem->text().length() < 20) {
            query.exec("SELECT notes FROM data_users_notes WHERE notes =\'" + NotesValue + "\' AND fk_user_id =\'" + load_id + "\' ");
            if (query.next() == false) {
                query.exec("UPDATE data_users_notes SET notes = \'" + NotesValue + "\' WHERE fk_user_id = \'" + load_id + "\' AND notes = \'" + notesItem->text() + "\' ");
                if (ui->textEdit_2->toPlainText().length() < 20) {
                    notesItem->setText(NotesValue);
                }
                else {
                    notesItem->setText(NotesValue.mid(0,20).append("..."));
                }
            }
            else {
                ui->textEdit_2->setStyleSheet("border: 1.5px solid '#e04f5f'; border-radius: 8px;font: 18pt 'Nirmala UI'; color: rgb(255, 255, 255);");
                QTimer::singleShot(2000, [this] { ui->textEdit_2->setStyleSheet("border: 1.5px solid '#30C193'; border-radius: 8px;font: 18pt 'Nirmala UI'; color: rgb(255, 255, 255);"); });
            }
        }
        else {
            query.exec("SELECT notes FROM data_users_notes WHERE notes =\'" + NotesValue + "\' AND fk_user_id =\'" + load_id + "\' ");
            if (query.next() == false) {
                query.exec("UPDATE data_users_notes SET notes = \'" + NotesValue + "\' WHERE fk_user_id = \'" + load_id + "\' AND notes LIKE \'%" + notesItem->text().mid(0,20) + "%\' ");
                if (ui->textEdit_2->toPlainText().length() < 20) {
                    notesItem->setText(NotesValue);
                }
                else {
                    notesItem->setText(NotesValue.mid(0,20).append("..."));
                }
            }
            else {
                ui->textEdit_2->setStyleSheet("border: 1.5px solid '#e04f5f'; border-radius: 8px;font: 18pt 'Nirmala UI'; color: rgb(255, 255, 255);");
                QTimer::singleShot(2000, [this] { ui->textEdit_2->setStyleSheet("border: 1.5px solid '#30C193'; border-radius: 8px;font: 18pt 'Nirmala UI'; color: rgb(255, 255, 255);"); });
            }
        }
    }

}

void PassControll::on_updateNotesButton_clicked() {
    UpdateNotes();
}

void PassControll::on_deleteButton_3_clicked() {
    QSqlQuery query(db);
    QString notes = ui->textEdit_2->toPlainText();
    QList<QListWidgetItem*> notesItem = ui->listWidget_2->selectedItems();

    foreach(QListWidgetItem * notesItem, notesItem)
    {
        if (notesItem->text().length() < 20) {
            query.exec("DELETE FROM data_users_notes WHERE notes =\'" + notesItem->text() + "\' ");
            delete ui->listWidget_2->takeItem(ui->listWidget_2->row(notesItem));
            ui->stackedWidget_4->setCurrentIndex(1);
        }
        else {
            query.exec("DELETE FROM data_users_notes WHERE notes LIKE \'%" + notesItem->text().mid(0,20) + "%\' ");
            delete ui->listWidget_2->takeItem(ui->listWidget_2->row(notesItem));
            ui->stackedWidget_4->setCurrentIndex(1);
        }
    }
}

void PassControll::on_lineEdit_2_returnPressed() {
    QSqlQuery query(db);
    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    QString val;

    f.open(QIODevice::ReadOnly | QIODevice::Text);
    val = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QString load_id = json["id"].toString();
    QString SearchQuery = ui->lineEdit_2->text();
    SearchQuery = SearchQuery.trimmed();

    query.exec("SELECT notes FROM data_users_notes WHERE notes LIKE \'%" + SearchQuery + "%\' AND fk_user_id =\'" + load_id + "\' ");
    if (SearchQuery == "") {

        ui->listWidget_2->clear();

        query.exec("SELECT notes FROM data_users_notes WHERE fk_user_id =\'"+ load_id +"\'");
        while (query.next()) {
            QString marker = query.value(0).toString();
            QListWidgetItem *notesItem = new QListWidgetItem;

            notesItem->setIcon(QIcon(":/new/userskey/Resources/assets/userkey/sticky-note.png"));
            notesItem->setText(marker);

            ui->listWidget_2->addItem(notesItem);
        }
    }
    else {
        while (query.next()) {
            ui->listWidget_2->clear();
            QString searchingItem = query.value(0).toString();
            QListWidgetItem *notesItem = new QListWidgetItem;

            notesItem->setIcon(QIcon(":/new/userskey/Resources/assets/userkey/sticky-note.png"));
            notesItem->setText(searchingItem);
            if (searchingItem.length() < 20) {
                ui->listWidget_2->addItem(notesItem);
            }
            else {
                ui->listWidget_2->addItem(notesItem->text().mid(0,20).append("..."));
            }
        }
    }
}
