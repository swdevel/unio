;
; primary.asm - ��������� ��������� �������
;

[bits 16]
[org 0]

%define BOOT_RAM            0x07c0 ; ����� �� �������� ���������� ������ ������������ ������� � ����������� �������� ����������
%define DEST_RAM            0x9000 ; ����� �� �������� ����� ���������� ����������� ������
%define LOADER_RAM          0x0700 ; ����� �� �������� ����� �������� ��������� ���������
%define CYLINDERS_TO_READ   10     ; ���������� ��������� �������, ������� ���������� ������� ��� �������� �������
%define MAX_READ_ERRORS     5      ; ���������� ���������� ������ ������. �������� ������ ���������� ��������� ��������� ���, ��� ��� ����� ������� ����� �� ������ ����������� 

entry:  cli                   ; ��������� ����������
        mov ax, si            ; � �������� si ��������� 0
        mov ds, ax
        mov es, ax
        mov ss, ax
        mov sp, BOOT_RAM    
        sti                   ; ��������� ����������
        
        ; �������������� ����������� ������������ ������� �� ������ 0x9000    
        mov ax, BOOT_RAM    
        mov ds, ax
        
        mov ax, DEST_RAM
        mov es, ax
        
        xor di, di
        xor si, si
        
        ; ���������� ����������� 512 ���� (128 dword)        
        mov cx, 128
        rep movsd
        
        jmp DEST_RAM:main    ; ��������� ������ �� ��� �����, ��� ������ ��������� ����� ������������ ������� (0x9000:0000)
        
        ; ��� ���� ����������� ��� �� ������ 0x9000:0000        
main:   mov ax, cs           ; ������� ����� �������� � ���������� ��������
        mov ds, ax
        mov ss, ax
        
        mov si, msg_loading  ; ������� ��������� � ������ �������� �������
        call putstr        
        
        mov si, msg_reading
        call putstr
        
        ; ������ ���������
        mov di, 1            ; �������� ������ � 1-��� �������� (0-�� ������� ����� ������ ���������) 
        mov ax, 0x290        ; ����� �������� � ������� ����� �������� ������
        xor bx, bx
        
.loop:  mov cx, 0x50
        mov es, cx
        
        push di
        
        ; ���������� �������� � ������� (ch - �������, dh-�������)
        shr di, 1
        setc dh
        mov cx, di
        xchg cl, ch
        
        pop di
            
        ; ��������: ��� �� �������� ���� �������?
        cmp di, CYLINDERS_TO_READ
        je .quit    
        
        call read            ; ����� ��������� ������ ��������� (������� ����� �������� �� ������ 0x50:0x0000 - 0x50:0x2400)
        
        ; ����������� ������������ �������� �� ������� ������
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
        
        ; ���������� 0-��� ��������    
.quit:  mov ax, 0x50
        mov es, ax
        mov bx, 0
        mov ch, 0
        mov dh, 0
        
        call read
        
        jmp 0x0000:LOADER_RAM ; ������ �� ����������� ���
        
; ��������� ������ ���������. ����� ��� ������ - es:bx, ch - �������, dh - ������� 

read:   mov [.read_errors], byte 0 ; ����� �������� ������
        pusha

        ; ����� ��������� � ����������� ��������
        mov si, msg_cylinder    
        call putstr
        
        mov ah, ch
        call puthex
        
        ; ����� ��������������� �������
        push ax
        mov al, ' '
        call putchr
        pop ax
        
        ; ����� ��������� � ��������� �������
        mov si, msg_head    
        call putstr
        
        mov ah, dh
        call puthex    
        
        mov si, msg_rn    
        call putstr
        
        popa
        pusha
        
        ; ����� ��������� ������� BIOS
.call:  mov ah, 0x02
        mov al, 18
        mov cl, 1    
            
        int 0x13
        jc .error
        
        popa
        ret
        
        ; ���������� ������ ������
.error: inc byte [.read_errors]   ; ����������� ������� ������
        mov si, msg_read_error    ; ������� ��������� �� ������
        call putstr
        call puthex               ; ������� ��� ������
        mov si, msg_rn    
        call putstr
        
        ; ���� ���-�� ������ ������ ��� ����������� ����������� ��������, �� ��������� ����� ��������� ������� BIOS
        cmp byte [.read_errors], MAX_READ_ERRORS
        jl .call
        
        ; ������ ������� �����, ������� ��������� � ������������� ������������ ������� � ������������� ���������
        mov si, msg_reboot
        call putstr
        cli
        hlt

.read_errors:    db 0 ; ���������� ������ ������

; ��������� ������ 16-������� �������� ����� �� �����. ��������� ����� ������ ���� ��������� � ������� ah    
        
hex_table:        db "0123456789ABCDEF" ; ������� �������� ��� ��������� puhex    
                
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

; ��������� ������ ������� �� �����. ��������� ������ ������ ���� �������� � ������� al

putchr: pusha
        mov ah, 0x0E
        int 0x10
        popa
        ret            
        
; ��������� ������ ������ �� �����. �� ������ ������ ������ ��������� ������� si

putstr: pusha
.loop:  lodsb
        test al, al
        jz .quit
        mov ah, 0x0e
        int 0x10
        jmp .loop
.quit:  popa
        ret
        
; ���������

msg_loading:       db "The first bootloader received control", 0x0A, 0x0D, 0
msg_reading:       db "Reading cylinders...", 0x0A, 0x0D, 0
msg_cylinder:      db "Cylinder:", 0
msg_head:          db "Head:", 0
msg_read_error:    db "Error reading from floppy, error code:", 0
msg_reboot:        db "Too many errors. Please, reboot your system", 0
msg_rn:            db 0x0A, 0x0D, 0

        
        ; ���������� ����������� ������ ������
        times 510 - ($-$$) db 0
        db 0xAA, 0x55
        
        ;  ��������� � ��� primary.bin ��������� ���� secondary.bin
incbin 'build\secondary.bin'
