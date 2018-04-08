;
; secondary.asm - ��������� ��������� �������
;

[bits 16]
[org 0x700]

%define KERNEL_RAM    0x200000 ; ����� �� �������� ����� ��������� ����
 
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
        
        ; ��������� �������� ������� ����������� ���������� � 0x20
        mov al,00010001b
        out 0x20,al
        mov al,0x20
        out 0x21,al
        mov al,00000100b
        out 0x21,al
        mov al,00000001b
        out 0x21,al 
        
        cli
        
        ; �������� �������� GDTR
        lgdt [global_desc_registr]
        
        ; ��������� A20
        in al, 0x92
        or al, 2
        out 0x92, al
        
        ; ��������� ���� PE �������� CR0
        mov eax, cr0
        or al, 1
        mov cr0, eax
        
        ; �������� ������� �������� � ������� CS ��� ������ �������� ������
        jmp 0x8: protected
                        
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

[BITS 32]
protected: ; �������� ��������� DS � SS ���������� �������� ������
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov ss, ax
            
        mov esi, kernel_binary
        mov edi, 0x200000

        ;; ������ ���� � dword
        ;; (65536 ����)
        mov ecx, 0x4000

        ;; ������� ���������� =)
        rep movsd
            
        ; ���� �����������, �������� ���������� ���
        jmp 0x200000
        

; ������ ��� �������� � ������� ���������� ������������� ������� GDTR

global_desc_registr:

dw 8192                 ; ������ ���������� ������������ - 8192
dd global_desc_table    ; ����� �� �������� ����������� ������� ������������
        
global_desc_table:      ; ���������� ������� ������������

; ������� ����������

db 0x00
db 0x00
db 0x00
db 0x00
db 0x00
db 0x00
db 0x00
db 0x00

; ���������� �������� ����

db 0xff ; ������ ��������
db 0xff
db 0x00 ; ����� ������ ��������
db 0x00
db 0x00
db 10011010b ; ��� 7     - ����������� ��������
             ; ���� 6..5 - ������� ���������� (Descriptor Privilege Level) 0-3
             ; 4 ���     - �������� �� ���������� ��������� ��� �� ������������ ������ ��� ����
             ; ���� 0..3 - ��� �������� (0010 - ����������� �������� ������, 1010 - ����������� �������� ����)
db 11001111b ; ��� 7     - ��� �������������. ���� 0 - ����� ���������� � ������, ���� 1 - ����� ���������� � 4096-������� ���������
             ; ��� 6     - ��� ����������� ��������. ���� 0 - 16-������ �������, ���� 1 - 32-������ �������
             ; ��� 5     - ������ ���� 0
             ; ��������� ���� �� ������������ �����������
db 0x00

; ���������� �������� ������

db 0xff            ; ������� ������ � DPL=0
db 0xff            ; �����=0 � �������=4��
db 0x00
db 0x00
db 0x00
db 10010010b
db 11001111b
db 0x00

; ���������
msg_recieve:    db "The second bootloader received control", 0x0A, 0x0D, 0
msg_protected:  db "Entering protected mode...", 0x0A, 0x0D, 0

kernel_binary:  incbin 'build\kernel.bin'
