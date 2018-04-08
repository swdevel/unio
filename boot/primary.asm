;
; primary.asm - первичный загрузчик системы
;

[bits 16]
[org 0]

%define BOOT_RAM            0x07c0 ; адрес по которому происходит чтение загрузочного сектора и последующая передача управления
%define DEST_RAM            0x9000 ; адрес по которому будет скопирован загрузочный сектор
%define LOADER_RAM          0x0700 ; адрес по которому будет загружен вторичный загрузчик
%define CYLINDERS_TO_READ   10     ; количество цилиндров дискеты, которые необходимо считать для загрузки системы
%define MAX_READ_ERRORS     5      ; допустимое количество ошибок чтения. Операцию чтения необходимо повторять несколько раз, так как мотор привода может не успеть разогнаться 

entry:  cli                   ; запрещаем прерывания
        mov ax, si            ; в регистре si находится 0
        mov ds, ax
        mov es, ax
        mov ss, ax
        mov sp, BOOT_RAM    
        sti                   ; разрешаем прерывания
        
        ; подготавливаем копирование загрузочного сектора по адресу 0x9000    
        mov ax, BOOT_RAM    
        mov ds, ax
        
        mov ax, DEST_RAM
        mov es, ax
        
        xor di, di
        xor si, si
        
        ; производим копирование 512 байт (128 dword)        
        mov cx, 128
        rep movsd
        
        jmp DEST_RAM:main    ; совершаем прыжок на тот адрес, где теперь находится копия загрузочного сектора (0x9000:0000)
        
        ; код ниже выполняется уже по адресу 0x9000:0000        
main:   mov ax, cs           ; заносим новые значения в сегментные регистры
        mov ds, ax
        mov ss, ax
        
        mov si, msg_loading  ; выводим сообщение о начале загрузки системы
        call putstr        
        
        mov si, msg_reading
        call putstr
        
        ; чтение цилиндров
        mov di, 1            ; начинать чтение с 1-ого цилиндра (0-ой цилиндр будет считан последним) 
        mov ax, 0x290        ; адрес сегмента в который будут записаны данные
        xor bx, bx
        
.loop:  mov cx, 0x50
        mov es, cx
        
        push di
        
        ; вычисление цилиндра и головки (ch - цилиндр, dh-головка)
        shr di, 1
        setc dh
        mov cx, di
        xchg cl, ch
        
        pop di
            
        ; проверка: все ли цилиндры были считаны?
        cmp di, CYLINDERS_TO_READ
        je .quit    
        
        call read            ; вызов процедуры чтения цилиндров (цилиндр будет прочитан по адресу 0x50:0x0000 - 0x50:0x2400)
        
        ; копирование прочитанного цилиндра по нужному адресу
        pusha
        push ds
        
        mov cx, 0x50
        mov ds, cx
        mov es, ax
        xor di, di
        xor si, si
        mov cx, 0x2400
        rep movsb
        
        pop ds
        popa    
        
        inc di
        add ax, 0x240
        jmp .loop
        
        ; считывание 0-ого цилиндра    
.quit:  mov ax, 0x50
        mov es, ax
        mov bx, 0
        mov ch, 0
        mov dh, 0
        
        call read
        
        jmp 0x0000:LOADER_RAM ; прыжок на загруженный код
        
; процедура чтения цилиндров. Буфер для чтения - es:bx, ch - цилиндр, dh - головка 

read:   mov [.read_errors], byte 0 ; сброс счётчика ошибок
        pusha

        ; вывод сообщения о прочитанном цилиндре
        mov si, msg_cylinder    
        call putstr
        
        mov ah, ch
        call puthex
        
        ; вывод разделительного пробела
        push ax
        mov al, ' '
        call putchr
        pop ax
        
        ; вывод сообщения о выбранной головке
        mov si, msg_head    
        call putstr
        
        mov ah, dh
        call puthex    
        
        mov si, msg_rn    
        call putstr
        
        popa
        pusha
        
        ; вызов дискового сервиса BIOS
.call:  mov ah, 0x02
        mov al, 18
        mov cl, 1    
            
        int 0x13
        jc .error
        
        popa
        ret
        
        ; обработчик ошибок чтения
.error: inc byte [.read_errors]   ; увеличиваем счётчик ошибок
        mov si, msg_read_error    ; выводим сообщение об ошибке
        call putstr
        call puthex               ; выводим код ошибки
        mov si, msg_rn    
        call putstr
        
        ; если кол-во ошибок меньше чем максимально допустимого значения, то повторяем вызов дискового сервиса BIOS
        cmp byte [.read_errors], MAX_READ_ERRORS
        jl .call
        
        ; ошибок слишком много, выводим сообщение о необходимости перезагрузки системы и останавливаем процессор
        mov si, msg_reboot
        call putstr
        cli
        hlt

.read_errors:    db 0 ; количество ошибок чтения

; процедура вывода 16-ричного значения числа на экран. Выводимое число должно быть загружено в регистр ah    
        
hex_table:        db "0123456789ABCDEF" ; таблица символов для процедуры puhex    
                
puthex: pusha
        xor bx, bx
        
        mov bl, ah
        and bl, 11110000b
        shr bl, 4
        mov al, [hex_table+bx]
        call putchr
        
        mov bl, ah
        and bl, 00001111b
        mov al, [hex_table+bx]
        call putchr
        
        popa
        ret

; процедура вывода символа на экран. Выводимый символ должен быть загружен в регистр al

putchr: pusha
        mov ah, 0x0E
        int 0x10
        popa
        ret            
        
; процедура вывода строки на экран. На начало строки должен указывать регистр si

putstr: pusha
.loop:  lodsb
        test al, al
        jz .quit
        mov ah, 0x0e
        int 0x10
        jmp .loop
.quit:  popa
        ret
        
; сообщения

msg_loading:       db "The first bootloader received control", 0x0A, 0x0D, 0
msg_reading:       db "Reading cylinders...", 0x0A, 0x0D, 0
msg_cylinder:      db "Cylinder:", 0
msg_head:          db "Head:", 0
msg_read_error:    db "Error reading from floppy, error code:", 0
msg_reboot:        db "Too many errors. Please, reboot your system", 0
msg_rn:            db 0x0A, 0x0D, 0

        
        ; дописываем загрузочный сектор нулями
        times 510 - ($-$$) db 0
        db 0xAA, 0x55
        
        ;  включение в код primary.bin бинарного кода secondary.bin
incbin 'build\secondary.bin'
