### LC-3 assembly language

#### Программа выводит двоичное написание числа в обратном порядке:

- Пример: 137 = 10001001 ---> 10010001
- В программе выведется 15 знаков, т. к. используются регистры 16-bit (16-й бит используется под знак)
- Число задается в переменной 'NUM'
- [Симулятор LC-3](https://spacetech.github.io/LC3Simulator/)

```
.ORIG x3000
LD R2, NUM

; Main cycle counter
AND R4, R4, #0
ADD R4, R4, #15

; Bit mooving to left
AND R3, R3, #0
ADD R3, R3, #1

BRnzp MAIN_LOOP

; Move bit to left
SHIFT_LEFT ADD R3, R3, R3

MAIN_LOOP AND R0, R3, R2
BRz PRINT_ZERO

; Print one
LD R0, S_ONE
OUT

BRnzp MAIN_LOOP_CNT_DEC

; Print zero
PRINT_ZERO LD R0, S_ZERO
OUT

; End of main loop
MAIN_LOOP_CNT_DEC ADD R4, R4, #-1
BRp SHIFT_LEFT

HALT
;
;
; Vars
;
NUM .FILL 137
S_ZERO     .FILL 48
S_ONE      .FILL 49

.END
```