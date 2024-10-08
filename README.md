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

1. [MinGW](https://winlibs.com/#download-release)
```
Следует использовать версию mingw для win32 или модифицировать файл build.bat таким образом, чтобы сборка ядра выполнялась для режима x32.
Во втором случае следует использовать ключи: "-m32" и "-m i386pe"
```
2. [NASM под Windows](https://www.nasm.us/)

После установки необходимо отредактировать файл build.bat и прописать значения для следующих переменных:

```
set CC="%PATH%\gcc.exe"
set LD="%PATH%\ld.exe"
set NASM="%PATH%\nasm.exe"
set OBJCOPY="%PATH%\objcopy.exe"
```

После редактирования необходимо запустить сборочный скрипт `build.bat`

В результате сборки проекта должен быть создан файл `boot.img`

### Запуск ОС в виртуальной машине VMware Workstation

[Пошаговая инструкция](https://github.com/swdevel/unio/wiki/Запуск-ОС-в-виртуальной-машине-VMware-Workstation)