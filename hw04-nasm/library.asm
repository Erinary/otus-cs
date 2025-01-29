_print_char:
    push eax
    mov  eax, 4
    mov  ebx, 1
    mov  ecx, esp
    mov  edx, 1
    int  80h
    pop  eax
    ret

_print_digits:
    xor  edx, edx        ; edx = 0
    mov  ecx, 10
    div  ecx             ; eax = eax/10, edx = eax%10
    push edx             ; добавляем текущую цифру в стек
    cmp  eax, 0          ; проверяем, можно ли делить дальше
    je   .output
    call _print_digits   ; рекурсивно вызываем эту же процедуру
  .output:
    pop  eax
    add  eax, '0'
    call _print_char
    ret

print_number:
    push edx
    push ecx
    push ebx
    push eax

    cmp  eax, 0
    jge  .positive

  .negative:
    push eax
    mov  eax, '-'
    call _print_char
    pop  eax
    neg  eax

  .positive:
    call _print_digits

    mov  eax, 0x0A       ; символ конца строки
    call _print_char

    pop  eax
    pop  ebx
    pop  ecx
    pop  edx
    ret

exit:
    mov  eax, 1
    xor  ebx, ebx
    int  0x80
