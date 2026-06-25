# Weather
Виджет рабочего стола с прогнозом погоды на С++, Qt, с использованием API ключа [WeatherAPI](https://www.weatherapi.com/) предоставляет информацию о текущей погоде, а также почасовой прогноз погоды для текушего дня. Есть возможность выбрать локацию и язык программы (русский/английский).

<p align="center">
  <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white" />
  <img alt="Static Badge" src="https://img.shields.io/badge/WeatherAPI-white?style=for-the-badge&link=https%3A%2F%2Fwww.weatherapi.com%2F">
</p>

---
### Основные возможности

Виджет имеет следующий функционал:
- Выбор локации через интерактивную карту
- Ввод собственного API-ключа
- Выбор языка интерфейса
- Перемещение основного окна (свободное позиционировние) с автоматическим переносом иконок, попадающих под окно
- Игнорирование горячих клавиш Win+D для сохранения окна всегда на рабочем столе
- Меню в системном трее с возможностью открытия настроек, выбора локации, а также закрытия виджета

> [!IMPORTANT]
> Виджет работает в фоновом режиме и не имеет иконки на панели задач, поэтому закрыть его можно через иконку в системном трее.

---

### 🛠 Стек технологий
* **Framework:** Qt Creator 
* **Языки:** С++, QML
* **Система сборки:** CMake
* **Модули:** QtLocation, QtPositioning, QtQuick, QtWidgets

---

### Демонстрация программы

Виджет выполнен в пиксельной стилистике, имеет огромное количество уникальных иконок состояния погоды. 


<img width="462" height="402" alt="image" src="https://github.com/user-attachments/assets/25a7ab2a-977a-4f17-a78b-c17c3535d793" />

<details> 
<img width="502" height="482" alt="image" src="https://github.com/user-attachments/assets/d93e608b-907a-4a4c-82d1-b049aab18056" />
<img width="502" height="482" alt="image" src="https://github.com/user-attachments/assets/510d3b50-2d27-4287-bab1-65518c1f68c2" />
<img width="229" height="158" alt="image" src="https://github.com/user-attachments/assets/3d5ba37b-4a7a-4b33-8f27-161796c18b0c" />

</details> 

---
### Как собрать проект в Qt?
Требования к платформе: Qt Creator 6.11.X (компилятор MinGW)

Инструкция:
1. Клонируйе репозиторий в нужный вам каталог с помощью команды:
```text
git clone https://github.com/vazzorina/Weather
```
2. В Qt Creator откройте проект
3. Выберите нужный профиль сборки (Debug/Release)
4. Запустите сборку



---

## 📄 Лицензия
Этот проект распространяется под лицензией MIT. Подробности в файле [LICENSE](LICENSE).
