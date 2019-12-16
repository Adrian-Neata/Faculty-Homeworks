%include "io.inc"

%define MAX_INPUT_SIZE 4096

section .bss
	expr: resb MAX_INPUT_SIZE

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
	push ebp
	mov ebp, esp
	GET_STRING expr, MAX_INPUT_SIZE
        mov edi, expr
       
        evaluate:
            xor eax, eax
            xor ebx, ebx
            cmp [edi], byte 0
            je end
            cmp [edi], byte '-'
            je subtraction
            cmp [edi], byte '+'
            je addition
            cmp [edi], byte '/'
            je division
            cmp [edi], byte '*'
            je multiplication
            cmp [edi], byte 48
            jge readNumber
            back:
            inc edi
            jmp evaluate
            
        subtraction:
            cmp [edi + 1], byte 48
            jg negativeNumber ;case it's a negative number
            pop ebx
            pop eax
            sub eax, ebx
            push eax
            jmp back
        addition:
            pop ebx
            pop eax
            add eax, ebx
            push eax
            jmp back
        division:
            pop ebx
            pop eax
            cdq ;for correct division
            idiv ebx
            push eax
            jmp back
        multiplication:
            pop ebx
            pop eax
            imul ebx
            push eax
            jmp back
        negativeNumber:
            mov ecx, 1
            inc edi
        readNumber:
            mov al, [edi]
            sub eax, 48
            cmp [edi + 1], byte 48
            jge continueReading ;case it has more digits
            cmp ecx, 1
            je minusSign
            push eax ;case it has a single digit
            jmp back
        continueReading:
            inc edi
            mov ebx, 10
            mul ebx
            mov bl, [edi]
            sub ebx, 48
            add eax, ebx
            cmp [edi + 1], byte 48
            jge continueReading
            cmp ecx, 1
            je minusSign
            push eax
            jmp back
        minusSign:
            mov ebx, -1
            imul ebx
            push eax
            xor ecx, ecx
            jmp back
        end:
            pop eax
            PRINT_DEC 4, eax
	pop ebp
	ret
