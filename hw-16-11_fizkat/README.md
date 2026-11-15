# ФизКат — консольный учёт парка

## Сборка и запуск

```bash
mkdir -p build
g++ -std=c++17 -Wall -Wextra -o build/fizkat *.cpp
./build/fizkat            # команды с клавиатуры
./build/fizkat < demo.txt # демо-сценарий со всеми командами и ошибками
```

## Команды

| Команда | Смысл |
|---|---|
| `add scooter id name energy score` | электросамокат |
| `add bike id name score` | велосипед (энергии нет) |
| `add ebike id name energy score` | электровелосипед |
| `add monowheel id name energy score` | моноколесо (креатив) |
| `add helmet id name` | шлем |
| `add dock id name energy` | док-станция |
| `list` | все позиции, отсортированные по id |
| `novice` | техника для новичков (`id name`) |
| `energy` | суммарная суточная энергия, 1 знак |
| `report` | число единиц и суммарная энергия |
| `focus id` / `show_focus` | витрина; после удаления — `removed`, если не ставили — `no focus` |
| `remove id` | убрать позицию |
| `log` | журнал техосмотров (креатив) |
| `quit` | выход |

Транспорт (`scooter`, `bike`, `ebike`, `monowheel`) всегда проходит техосмотр.
Правило техосмотра: если в имени есть подстрока `broken` без учёта регистра, то
устройство отклоняется. Вещи (`helmet`, `dock`) техосмотр не проходят.

Правило новичка: простота `>= kNoviceMinScore` (= 6, [Transport.hpp](Transport.hpp)).
Чтобы поменять правило, нужно поменять константу или переопределить
`isNoviceFriendly()`, как это сделано у моноколеса.

## Схема наследования

```
InventoryItem (абстрактный)                EnergyConsumer (абстрактный)
├── Transport (абстрактный)                        │
│   ├── Bike                                       │
│   └── ElectricTransport ── public ───────────────┤   множественное
│       ├── ElectricScooter                        │
│       ├── ElectricBike                           │
│       └── Monowheel                              │
└── Thing (абстрактный)                            │
    ├── Helmet                                     │
    └── Dock ─────────────── public ───────────────┘   множественное
```

- `ElectricTransport : public Transport, public EnergyConsumer` и
  `Dock : public Thing, public EnergyConsumer` — множественное наследование
  ([Transport.hpp](Transport.hpp), [Things.hpp](Things.hpp)).
- Ромба нет: `EnergyConsumer` не наследуется от `InventoryItem`, поэтому
  `virtual`-наследование не нужно.
- `InventoryItem` ([Inventory.hpp](Inventory.hpp)): чисто виртуальные
  `inventoryId()`, `name()`, `describe()` и виртуальный деструктор.
  `EnergyConsumer`: чисто виртуальный `dailyEnergy()`.

## Полиморфизм

`Fleet` ([Fleet.hpp](Fleet.hpp)) хранит всё как `shared_ptr<InventoryItem>`.
Команды `list`, `novice`, `energy` и `show_focus` вызывают только виртуальные
методы базиса, без `dynamic_cast`:
- `operator<<` и `describe()` используются для печати;
- `isNoviceFriendly()` для `novice`: по умолчанию `false`, `Transport` проверяет
  простоту, `Monowheel` всегда возвращает `false`;
- `asEnergyConsumer()` для `energy`: по умолчанию `nullptr`, потребители энергии
  возвращают `this`. Так сумма считается через указатель на базис.

## Операторы

- `operator<<(ostream&, const InventoryItem&)` печатает `id name`
  ([Inventory.cpp](Inventory.cpp)).
- `operator<(const InventoryItem&, const InventoryItem&)` сравнивает по id,
  `list` сортирует через него (`Fleet::sortById`).
- `operator<<` и `operator[]` есть у шаблона `Registry`.

## Smart-указатели

- `unique_ptr`: новая единица создаётся через `make_unique` и до техосмотра
  принадлежит только вызывающему коду. `ServiceCenter::inspect(unique_ptr<Transport>)`
  ([ServiceCenter.hpp](ServiceCenter.hpp)) забирает владение. При отказе
  бросается исключение, `unique_ptr` уничтожает объект, а парк не меняется.
  При успехе объект возвращается и `Fleet::addTransport` передаёт его в парк.
- `shared_ptr`: парк хранит принятые единицы в
  `vector<shared_ptr<InventoryItem>>`.
- `weak_ptr`: витрина (`Fleet::focus_`) только указывает на позицию и не
  продлевает ей жизнь. После `remove` `lock()` возвращает пустой указатель, и
  `show_focus` печатает `removed`.
- `new` и `delete` вручную нигде не используются.

## Исключения

[Errors.hpp](Errors.hpp), все наследуются от `FleetError : std::runtime_error`:

| Тип | Когда бросается |
|---|---|
| `InspectionError` | техосмотр отклонил устройство (`ServiceCenter::inspect`) |
| `CommandError` | неизвестная команда или тип, неверное число аргументов, не число вместо числа (`main.cpp`) |
| `InvalidValueError` | простота вне 1..10, энергия < 0, пустой id или имя (конструкторы), повторный id (`Fleet`) |
| `NotFoundError` | `remove` или `focus` для несуществующего id |

`error: ...` печатается только в `catch` в `main`. Все проверки выполняются до
изменения парка: аргументы разбираются, объект создаётся и проверяется, id
проверяется на уникальность, проходит техосмотр, и только после этого делается
`push_back`. Поэтому после ошибки парк не портится.

## Шаблон

`template <typename T, int Cap> class Registry` ([Registry.hpp](Registry.hpp))
хранит журнал из последних `Cap` записей: `add`, `size`, `operator[]`,
`operator<<`. При переполнении удаляется самая старая запись.

Где используется: `ServiceCenter` записывает каждый техосмотр в
`InspectionLog = Registry<InspectionRecord, 16>`, команда `log` его печатает.

Явная специализация: для `InspectionLog` специализирован `operator<<`
([ServiceCenter.cpp](ServiceCenter.cpp)). У `InspectionRecord` нет своего
`operator<<`, поэтому специализация сама печатает `id accepted/rejected`, а для
пустого журнала выводит `no inspections yet`.

## Креативная часть

1. Новый вид транспорта — **моноколесо** (`Monowheel`, `add monowheel ...`).
   Оно потребляет энергию, но переопределяет `isNoviceFriendly()` и никогда не
   выдаётся новичкам, даже при простоте ≥ 6.
2. Новая команда — **`log`**, журнал техосмотров (принят/отклонён) на шаблоне
   `Registry`.

## Использование ИИ

1. Использование Claude для планирования структуры выполнения проекта. 

2. Дебаггинг кода, вопросы по теории, если забыл как писать конкретную часть функции. 

3. Приведение к красивому формату и написание README.  

TODO
