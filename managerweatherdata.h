#ifndef MANAGERWEATHERDATA_H
#define MANAGERWEATHERDATA_H

#include <QObject>
#include <QAbstractListModel>
#include <QVector>

struct WeatherItem {
    QString time;   // время
    int temp;       // температура
    QString icon;   // путь до иконки
};

class ManagerWeatherData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentDate READ currentDate NOTIFY currentDateChanged) // обновление даты

    Q_PROPERTY(QString cityName READ cityName WRITE setCityName NOTIFY cityNameChanged) //название города
    Q_PROPERTY(QString tempNow READ tempNow WRITE setTempNow NOTIFY tempNowChanged) //текущая температура
    Q_PROPERTY(QString condition READ condition WRITE setCondition NOTIFY conditionChanged) //состояние погоды
    Q_PROPERTY(QString iconPath READ iconPath WRITE setIconPath NOTIFY iconPathChanged FINAL) //путь к иконке для текущей погоды

public:
    explicit ManagerWeatherData(QObject *parent = nullptr);

    //геттеры
    QString cityName() const { return m_cityName; }
    QString tempNow() const { return m_tempNow; }
    QString condition() const { return m_condition; }
    QString iconPath() const { return m_iconPath; }
    QString currentDate() const { return m_currentDate; }

public slots:
    //сеттеры
    void setCityName(const QString newCityName);
    void setTempNow(const QString newTempNow);
    void setCondition(const QString newCondition);
    void setIconPath(const QString newIconPath);

    void updateDate();

signals:
    void cityNameChanged();
    void tempNowChanged();
    void conditionChanged();
    void iconPathChanged();
    void currentDateChanged();

private:
    QString m_cityName;
    QString m_tempNow;
    QString m_condition;
    QString m_iconPath;
    QString m_currentDate;
};



class WeatherModel : public QAbstractListModel
{
    Q_OBJECT

public:
    // перечисляем роли, которые будут доступны в QML
    enum WeatherRoles {
        TimeRole = Qt::UserRole + 1,
        TempRole,
        IconRole
    };

    explicit WeatherModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    // возвращает количество элементов в списке
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return m_data.size();
    }

    // возвращает данные для QML по индексу и роли
    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid() || index.row() >= m_data.size())
            return QVariant();

        const WeatherItem &item = m_data[index.row()];

        switch (role) {
        case TimeRole: return item.time;
        case TempRole: return item.temp;
        case IconRole: return item.icon;
        default: return QVariant();
        }
    }

    // связывает числовые роли C++ со строковыми именами в QML
    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[TimeRole] = "time"; // Теперь в QML доступно свойство model.time
        roles[TempRole] = "temp"; // model.temp
        roles[IconRole] = "icon"; // model.icon
        return roles;
    }

    // метод для динамического добавления
    void addWeatherItem(const WeatherItem &item) {
        // оповещаем QML-представление о начале вставки данных
        beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
        m_data.append(item);
        endInsertRows(); // оповещаем об окончании вставки. QML сразу перерисует список!
    }

    // метод для отчиски модели
    void clear() {
        if (m_data.isEmpty()) return;
        beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
        m_data.clear();
        endRemoveRows();
    }

private:
    QVector<WeatherItem> m_data; // внутреннее хранилище данных в C++
};

#endif // MANAGERWEATHERDATA_H
