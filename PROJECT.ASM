.MODEL SMALL
.STACK 100h

.DATA
msg1 DB 'Enter text: $'
msg2 DB 13,10,'Character count = $'
msg3 DB 13,10,'Encrypted: $'
msg4 DB 13,10,'Decrypted: $'

input DB 100 DUP('$')
count DW 0
num DB '000','$'

enc_lo DB 'qwertyuiopasdfghjklzxcvbnm'
dec_lo DB 'kxvmcnophqrszyijadlegwbuft'
enc_up DB 'QWERTYUIOPASDFGHJKLZXCVBNM'
dec_up DB 'KXVMCNOPHQRSZYIJADLEGWBUFT'

.CODE
start:
    MOV AX, @DATA
    MOV DS, AX

    ; Ask input
    LEA DX, msg1
    MOV AH, 9
    INT 21h

    LEA SI, input
    XOR CX, CX

read_loop:
    MOV AH, 1
    INT 21h
    CMP AL, 13
    JE end_input
    MOV [SI], AL
    INC SI
    INC CX
    JMP read_loop

end_input:
    MOV BYTE PTR [SI], '$'
    MOV count, CX

    ; Show count
    LEA DX, msg2
    MOV AH, 9
    INT 21h
    CALL print_count

    ; Encrypt
    LEA DX, msg3
    MOV AH, 9
    INT 21h
    MOV AH, 1
    LEA SI, input
    CALL crypt
    LEA DX, input
    MOV AH, 9
    INT 21h

    ; Decrypt
    LEA DX, msg4
    MOV AH, 9
    INT 21h
    MOV AH, 0
    LEA SI, input
    CALL crypt
    LEA DX, input
    MOV AH, 9
    INT 21h

    MOV AH, 4Ch
    INT 21h

; =========================
; Print decimal count
; =========================
print_count PROC NEAR
    MOV AX, count
    MOV BX, 10
    LEA DI, num+2

pc_loop:
    XOR DX, DX
    DIV BX
    ADD DL, '0'
    MOV [DI], DL
    DEC DI
    CMP AX, 0
    JNE pc_loop

    LEA DX, num
    MOV AH, 9
    INT 21h
    RET
print_count ENDP

; =========================
; Encrypt / Decrypt
; AH = 1 encrypt
; AH = 0 decrypt
; =========================
crypt PROC NEAR
crypt_loop:
    MOV AL, [SI]
    CMP AL, '$'
    JE crypt_done
    CALL map_char
    MOV [SI], AL
    INC SI
    JMP crypt_loop

crypt_done:
    RET
crypt ENDP

; =========================
; Map character
; =========================
map_char PROC NEAR
    CMP AL, 'a'
    JB upper_case
    CMP AL, 'z'
    JA map_done

    SUB AL, 'a'
    MOV BX, OFFSET enc_lo
    CMP AH, 0
    JNE do_xlat1
    MOV BX, OFFSET dec_lo
do_xlat1:
    XLAT
    RET

upper_case:
    CMP AL, 'A'
    JB map_done
    CMP AL, 'Z'
    JA map_done

    SUB AL, 'A'
    MOV BX, OFFSET enc_up
    CMP AH, 0
    JNE do_xlat2
    MOV BX, OFFSET dec_up
do_xlat2:
    XLAT

map_done:
    RET
map_char ENDP

END start
