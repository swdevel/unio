;
;   Функция вывода на экран нуль-терминированной ASCII строки
;   Параметры:
;       SI - адрес строки
;

print_string:
    pusha

.print_loop:
    lodsb
    test al, al
    jz .print_string_exit
    call print_char
    jmp .print_loop

.print_string_exit:
    popa
    ret

;
;   Функция вывода на экран ASCII символа
;   Параметры:
;       AL - код символа

print_char:
    pusha
    mov ah, 0x0e
	int 0x10
	popa
	ret

;
;   Функция вывода на экран числа в HEX
;   Параметры:
;       AH - число
;

hex_table:  db  '0123456789abcdef' ; Таблица символов для функции print_hex

print_hex:
    pusha
    xor bx, bx

    mov bl, ah
	and bl, 11110000b
	shr bl, 4
	mov al, [hex_table + bx]
	call print_char

    mov bl, ah
	and bl, 00001111b
	mov al, [hex_table + bx]
	call print_char
	
	popa
	ret