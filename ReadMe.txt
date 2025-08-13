# MiniDB

MiniDB e малка релационна база от данни, написана на C++, с поддръжка на основни SQL-подобни операции. Програмата работи в конзолата чрез REPL (read-eval-print loop), където потребителят може да въвежда команди.

---

## Възможности

* Създаване на нови таблици и колони
* Вмъкване, редактиране и изтриване на редове
* Селектиране на данни по стойност
* Експортиране в CSV файлове
* Запис и зареждане от текстови файлове
* Промяна на типа на колоните

---

## Поддържани типове данни

* `int`
* `double`
* `string` (задължително в кавички: "Ivan")
* `date` (формат: YYYY-MM-DD)
* `NULL`

---

## Поддържани команди

| Команда                                                                  | Описание                       |
| ------------------------------------------------------------------------ | ------------------------------ |
| `addtable <име>`                                                         | Създава нова таблица           |
| `addcolumn <таблица> <име> <тип>`                                        | Добавя колона към таблица      |
| `insert <таблица> <стойности...>`                                        | Добавя ред                     |
| `select <колона> <стойност> <таблица>`                                   | Извежда съвпадащи редове       |
| `update <таблица> <търсенаКолона> <стойност> <целКолона> <новаСтойност>` | Променя стойности              |
| `delete <таблица> <колона> <стойност>`                                   | Изтрива редове                 |
| `modify <таблица> <колона> <новТип>`                                     | Променя типа на колона         |
| `describe <таблица>`                                                     | Показва структурата на таблица |
| `print <таблица>`                                                        | Извежда всички редове          |
| `export <таблица> <файл>`                                                | Експортира в CSV               |
| `saveas <файл>`                                                          | Запазва базата във файл        |
| `load <файл>`                                                            | Зарежда база от файл           |
| `close`                                                                  | Затваря текущата база          |
| `exit`                                                                   | Излиза от програмата           |

---

## Компилация

```bash
g++ -std=c++17 -Wall -Wextra -o MiniDB \
main.cpp \
Table/Table.cpp \
Row/Row.cpp \
Column/Column.cpp \
Database/Database.cpp \
Cells/IntCell/IntCell.cpp \
Cells/DoubleCell/DoubleCell.cpp \
Cells/StringCell/StringCell.cpp \
Cells/DateCell/DateCell.cpp \
Cells/DateCell/Date.cpp \
Cells/NullCell/NullCell.cpp \
Cells/CellFactory/CellFactory.cpp
```

---

## Примерна сесия

```
addtable users
addcolumn users id int
addcolumn users name string
addcolumn users dob date
addcolumn users email string

insert users 1 "Ivan" 2000-01-01 "ivan@example.com"
insert users 2 "Maria" 1998-06-14 "maria@abv.bg"

print users
select 1 "Ivan" users
update users 1 "Ivan" 1 "John"
delete users 1 "Maria"

describe users
export users users.csv
saveas demo.txt
close
load demo.txt
print users
```

---

## Структура на проекта

```
MiniDB/
├── main.cpp
├── Database/
├── Table/
├── Row/
├── Column/
├── Cells/
│   ├── Cell.h
│   ├── CellFactory/
│   ├── IntCell/
│   ├── StringCell/
│   ├── DoubleCell/
│   ├── DateCell/
│   ├── NullCell/
└── README.md
```

---

