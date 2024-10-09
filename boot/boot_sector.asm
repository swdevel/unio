;
; Файл содержит код загрузочного сектора для FAT 12
;

;
; BIOS Parameter Block (BPB)
;

JumpBoot:
    jmp short BootStart ; Переход на код загрузчика
    nop

    PBP_Label                   db  '        '      ; Спецификация FAT сообщает, что данное поле игнорируется (8 байт)
    PBP_BytesPerSector          dw  0x200           ; Количество байт на логический сектор
    BPB_SectorsPerCluster       db  1               ; Количество секторов на класстер
    BPB_ReservedSectorCount     dw  1               ; Количество скрытых секторов
    BPB_FATCount                db  2               ; Количество копий FAT
    BPB_RootDirMaxEntryCount    dw  224             ; Максимальное количество элементов в корневом каталоге
    BPB_SectorsTotal16          dw  2880            ; Общее число секторов
    BPB_MediaType               db  0xf0            ; Код типа устройства (0xf0 - дискета 3.5 дюйма)
    BPB_SectorsPerTable         dw  9               ; Количество секторов на одну таблицу
    BPB_SectorsPerTrack         dw  18              ; Количество секторов на одну дорожку
    BPB_HeadCount               dw  2               ; Количество головок
    PBP_HiddenSectorsCount      dd  0               ; Количество скрытых секторов
    BPB_SectorsTotal32          dd  0               ; Общее число секторов. Данное поле используется, если число секторов
                                                    ; превышает 65535 и его невозможно записать в поле "BPB_SectorsTotal16"
                                                    ; В противном случае поле должно быть инициализировано значением "0"

;
; Extended Boot Record (EBPB)
;

    EBPB_DriverNumber           db  0               ; Номер диска для прерывания int13
    EBPB_ReservedForNT          db  0               ; Зарезервировано для Windows NT
    EBPB_BootSignature          db  0x29            ; Сигнатура расширения
    EBPB_VolumeId               dd  0x00000000      ; Серийный номер тома
    EBPB_VolumeLabel            db  'Boot disk  '   ; Метка тома (11 байт)
    EBPB_FileSystemType         db  'FAT12   '      ; Тип файловой системы (8 байт)

;
; Код загрузчика
;

BootStart: