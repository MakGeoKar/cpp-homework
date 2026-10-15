# ДЗ 1 по C++

- `task1_weather` - дневник погоды
- `task2_piggy_bank` - копилка, своя команда `double` (удваивает сумму)
- `task3_board` - табло объявлений
- `task4_pointers` - журнал оценок на `int***` и `void*`

Сборка:

```
g++ -std=c++17 -Wall task1_weather/main.cpp -o weather
```

Пример ввода для task4:

```
3
2 3 0
3 2 2 4 1
5 4 5
4 5
3 3
5 5 5 4
2
```

Вывод:

```
group 1: average 4.60, best student 1 (4.67)
group 2: average 3.86, best student 2 (4.75)
group 3: no marks
```
