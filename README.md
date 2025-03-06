# Sorting-utility

Консольная утилита на языке C с открытым исходным кодом. Предназначена для сортировки указанного количества строк из файла с выбором алгоритма сортировки и порядка сортировки.
Поддерживаемые алгоритмы сортировки:
- `bubble` — сортировка пузырьком
- `insertion` — сортировка вставками
- `merge` — сортировка слиянием
- `quick` — быстрая сортировка
- `radix` — поразрядная сортировка

Поддерживаемые компараторы:
- `asc` — по возрастанию 
- `des` — по убыванию

Способы чтения файла:
- Через API файлового ввода-вывода (установлено по умолчанию)
- Через отображение файла в память (устанавливается опционально при сборке)

## Требования

Перед исползованием убедитесь, что у вас установлены:
1. CMake. Установка (для Ubuntu/Debian):

```sh
sudo apt install cmake
```

2. GCC или Clang. Установка на примере gcc (для Ubuntu/Debian):

```sh
sudo apt install gcc
```

3. Make. Установка (для Ubuntu/Debian):

```sh
sudo apt install make
```

## Сборка

Для выбора стандартного способа чтения файла в папке с проектом запустите скрипт:

```sh
./scripts/build.sh
```

Для выбора чтения через отображение файла в память используйте флаг `--mmap`:

```sh
./scripts/build.sh --mmap
```

Исполняемый файл должен появится в директории `build`

## Запуск

Запустите утилиту из `build` передав ей нужные аргументы:

```sh
./sort_strings <число строк> <путь к файлу> <сортировка> <компаратор>
```

Пример:

test1.txt:

```sh
Hello World!
Hello!
Hello
```

```sh
./sort_strings 3 ../tests/input/test1.txt bubble asc
```

Вывод:

```sh
Hello
Hello World!
Hello!
```

## Тестирование

*В разработке...*
