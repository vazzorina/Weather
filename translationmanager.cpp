#include "translationmanager.h"
#include <QApplication>

TranslationManager::TranslationManager(QObject *parent)
    : QObject{parent}
{
    env = new QSettings(".env", QSettings::IniFormat);
}

void TranslationManager::checkingEnvFile() {
    if(!env->contains("LANGUAGE")) writeLangToEnvFile("ru");
}

void TranslationManager::writeLangToEnvFile(const QString &localeCode) {
    env->setValue("LANGUAGE", localeCode);
}

QString TranslationManager::readLangFromEnvFile() {
    return env->value("LANGUAGE").toString();
}

void TranslationManager::setLanguage() {
    static QTranslator *translator = new QTranslator(qApp);
    QString localeCode = readLangFromEnvFile();
    qApp->removeTranslator(translator);

    if(translator->load(QString(":qt/qml/Weather//locales/%1.qm").arg(localeCode))) {
        qDebug() << "Загружен перевод на " + localeCode;
        qApp->installTranslator(translator);
    }
    else qWarning() << "Перевод не загрузился!";

    emit languageChanged(localeCode);
}
