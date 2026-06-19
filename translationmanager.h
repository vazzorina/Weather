#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QSettings>

class TranslationManager : public QObject
{
    Q_OBJECT
public:
    explicit TranslationManager(QObject *parent = nullptr);
    void setLanguage();

    QSettings *env;
    void checkingEnvFile();
    void writeLangToEnvFile(const QString &localeCode);
    QString readLangFromEnvFile();
signals:
    void languageChanged(const QString &localeCode);
};

#endif // TRANSLATIONMANAGER_H
