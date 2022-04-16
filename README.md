# Пример простой операционной системы для x86

![alt text](https://raw.githubusercontent.com/swdevel/unio/master/entry.png)

## Реализовано:
1. Загрузка с образа дискеты
2. Исполнение кода в защищённом режиме
3. Система обработки прерываний
4. Текстовый VGA-режим
5. Поддержка клавиатуры
6. Сканирование шины PCI и вывод списка устройств
7. Страничная адресация памяти
8. Поддержка памяти типа heap

## Планируется реализовать:
1. Поддержка файловой системы
2. Многозадачность
3. Графический VGA-режим

## Быстрый старт

### Сборка проекта под Windows
----
Для сборки проекта необходимо установить следующий набор программ:

1. MinGW
2. NASM под Windows

После установки необходимо отредактировать файл build.bat и прописать значения для следующих переменных:

```
set CC="путь\к\файлу\gcc.exe"
set LD="путь\к\файлу\ld.exe"
set NASM="путь\к\файлу\nasm.exe"
set OBJCOPY="путь\к\файлу\objcopy.exe"
```

После редактирования необходимо запустить сборочный скрипт `build.bat`

В результате сборки проекта должен быть создан файл `boot.img`

### Запуск ОС в виртуальной машине VMware Workstation

----
При создании новой виртуальной машины необходимо выполнить следующую последовательность шагов:

Шаг 1:
<details>

![alt text](https://raw.githubusercontent.com/swdevel/unio/master/man/images/setup_wm_01.png)

</details>

Шаг 2:
<details>

![alt text](https://raw.githubusercontent.com/swdevel/unio/master/man/images/setup_wm_02.png)

</details>

Шаг 3:
<details>

![alt text](https://raw.githubusercontent.com/swdevel/unio/master/man/images/setup_wm_03.png)

</details>

Шаг 4:
<details>

![alt text](https://raw.githubusercontent.com/swdevel/unio/master/man/images/setup_wm_04.png)

</details>

Шаг 5:
<details>

![alt text](https://raw.githubusercontent.com/swdevel/unio/master/man/images/setup_wm_05.png)

</details>

Шаг 6:
<details>

![alt text](https://raw.githubusercontent.com/swdevel/unio/master/man/images/setup_wm_06.png)

</details>

Шаг 7:
<details>

![alt text](https://raw.githubusercontent.com/swdevel/unio/master/man/images/setup_wm_07.png)

</details>