;
; secondary.asm - вторичный загрузчик системы
;

[bits 16]
[org 0x700]

%define KERNEL_RAM    0x200000 ; адрес по которому будет загружено ядро
 
        cli
        mov ax, 0
        mov ds, ax
        mov es, ax
        mov ss, ax
        mov sp, 0x700
        sti
        
        mov si, msg_recieve
        call putstr
        
        mov si, msg_protected
        call putstr
        
        ; установка базового вектора контроллера прерываний в 0x20
        mov al,00010001b
        out 0x20,al
        mov al,0x20
        out 0x21,al
        mov al,00000100b
        out 0x21,al
        mov al,00000001b
        out 0x21,al 
        
        cli
        
        ; загрузка регистра GDTR
        lgdt [global_desc_registr]
        
        ; включение A20
        in al, 0x92
        or al, 2
        out 0x92, al
        
        ; установка бита PE регистра CR0
        mov eax, cr0
        or al, 1
        mov cr0, eax
        
        ; загрузка нужного сегмента в регистр CS при помощи длинного прыжка
        jmp 0x8: protected
                        
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

[BITS 32]
protected: ; загрузка регистров DS и SS селектором сегмента данных
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov ss, ax
            
        mov esi, kernel_binary
        mov edi, 0x200000

        ;; размер ядра в dword
        ;; (65536 байт)
        mov ecx, 0x4000

        ;; поехали копировать =)
        rep movsd
            
        ; ядро скопировано, передаем управление ему
        jmp 0x200000
        

; данные для загрузки в регистр глобальной дескрипторной таблицы GDTR

global_desc_registr:

dw 8192                 ; полное количество дескрипторов - 8192
dd global_desc_table    ; адрес по которому расположена таблица дескрипторов
        
global_desc_table:      ; глобальная таблица дескрипторов

; нулевой дескриптор

db 0x00
db 0x00
db 0x00
db 0x00
db 0x00
db 0x00
db 0x00
db 0x00

; дескриптор сегмента кода

db 0xff ; размер сегмента
db 0xff
db 0x00 ; адрес начала сегмента
db 0x00
db 0x00
db 10011010b ; бит 7     - присутствие сегмента
             ; биты 6..5 - уровень привилегий (Descriptor Privilege Level) 0-3
             ; 4 бит     - является ли дескриптор системным или же дескриптором данных или кода
             ; биты 0..3 - тип сегмента (0010 - дескриптора сегмента данных, 1010 - дескриптора сегмента кода)
db 11001111b ; бит 7     - бит гранулярности. Если 0 - лимит измеряется в байтах, если 1 - лимит измеряется в 4096-байтнах страницах
             ; бит 6     - бит разрядности сегмента. Если 0 - 16-битный сегмент, если 1 - 32-битный сегмент
             ; бит 5     - должен быть 0
             ; остальные биты не используются процессором
db 0x00

; дескриптор сегмента данных

db 0xff            ; сегмент данных с DPL=0
db 0xff            ; базой=0 и лимитом=4Гб
db 0x00
db 0x00
db 0x00
db 10010010b
db 11001111b
db 0x00

; сообщения
msg_recieve:    db "The second bootloader received control", 0x0A, 0x0D, 0
msg_protected:  db "Entering protected mode...", 0x0A, 0x0D, 0

kernel_binary:  incbin 'build\kernel.bin'
