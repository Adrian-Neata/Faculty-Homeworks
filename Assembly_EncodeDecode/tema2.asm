;Neata Adrian 321CB

extern puts
extern printf
extern strlen

%define BAD_ARG_EXIT_CODE -1

section .data
filename: db "./input0.dat", 0
inputlen: dd 2263

fmtstr:            db "Key: %d",0xa, 0
usage:             db "Usage: %s <task-no> (task-no can be 1,2,3,4,5,6)", 10, 0
error_no_file:     db "Error: No input file %s", 10, 0
error_cannot_read: db "Error: Cannot read input file %s", 10, 0

section .text
global main

xor_strings:
        push ebp
        mov ebp, esp
        mov edi, [esp + 8]   ;string address
        mov esi, [esp + 12]  ;key address
        xor_byte:
            mov bl, byte[edi]
            mov cl, byte[esi]
            xor bl, cl
            mov byte [edi], bl
            inc edi
            inc esi
            cmp byte [edi], 0
            jne xor_byte
        leave
	ret

rolling_xor:
        push ebp
        mov ebp, esp
        mov edi, [esp + 8]
        mov eax, edi    ;retains the initial address
        end_string:     ;goes to the end of the string
            inc edi
            cmp byte[edi], 0
            jne end_string
        dec edi
        roll_xor:
            mov bl, byte[edi]
            mov cl, byte[edi - 1]
            xor bl, cl
            mov byte[edi], bl
            dec edi
            cmp edi, eax
            je end_rolling_xor
            jmp roll_xor
        end_rolling_xor:
        leave
        ret

xor_hex_strings:
        push ebp
        mov ebp, esp
        mov edi, [esp + 8]   ;string address
        mov esi, [esp + 12]   ;key address
        push edi    ;remember string address
        push esi        
        call hex_to_dec   ;converts the key
        mov edi, [esp + 4]
        push edi
        call hex_to_dec   ;converts the string
        pop edi
        pop esi
        xor_byte3:
            mov bl, byte[edi]
            mov cl, byte[esi]
            xor bl, cl
            mov byte [edi], bl
            inc edi
            inc esi
            cmp byte [edi], 0
            jne xor_byte3
        leave
	ret

base32decode:
        push ebp
        mov ebp, esp
        mov esi, [esp + 8]      ;string address
        mov edi, esi
        set_bytes:
        mov ecx, 8
        get_8bytes:
            xor eax, eax
            mov al, byte[esi]
            push eax
            call convert_base32
            inc esi
            loop get_8bytes
            
        mov eax, [esp + 28]
        shl al, 3
        mov ebx, [esp + 24]
        shr bl, 2
        or al, bl
        mov byte[edi], al       ;first byte
        inc edi
        
        mov eax, [esp + 24]
        shl al, 6
        mov ebx, [esp + 20]
        shl bl, 1
        mov ecx, [esp + 16]
        shr cl, 4 
        or al, bl
        or al, cl
        mov byte[edi], al    ;second byte
        inc edi
        
        mov eax, [esp + 16]
        shl al, 4 
        mov ebx, [esp + 12]
        shr bl, 1
        or al, bl
        mov byte[edi], al    ;third byte
        inc edi
        
        mov eax, [esp + 12]
        shl al, 7
        mov ebx, [esp + 8]
        shl bl, 2
        mov ecx, [esp + 4]
        shr cl, 3
        or al, bl
        or al, cl
        mov byte[edi], al    ;fourth byte
        inc edi
        
        mov eax, [esp + 4]
        shl al, 5
        mov ebx, [esp]
        or al, bl
        mov byte[edi], al    ;fifth byte
        inc edi
        
        cmp byte[esi], 0
        jne set_bytes
        
        leave
        ret

bruteforce_singlebyte_xor:
        push ebp
        mov ebp, esp
        mov esi, [esp + 8]          ;string address
        xor eax, eax
        
        back_to_xor:
        push eax
        push esi
        call xor_string
 
        mov esi, [esp + 8]
        dec esi                     ;check result
        repeat_testing:
            inc esi
            cmp byte[esi], 0
            je key_not_found
            cmp byte[esi], 'f'
            jne repeat_testing
            inc esi
            cmp byte[esi], 'o'
            jne repeat_testing
            inc esi
            cmp byte[esi], 'r'
            jne repeat_testing
            inc esi
            cmp byte[esi], 'c'
            jne repeat_testing
            inc esi
            cmp byte[esi], 'e'
            jne repeat_testing
            jmp success
        key_not_found:
            mov esi, [ebp + 8]
            push eax
            push esi
            call xor_string             ;turns back the string to its original values
            inc al
            cmp al, 0
            je fail
            mov esi, [ebp + 8]
            jmp back_to_xor
        success:
            push eax
        fail:
        leave
	ret

decode_vigenere:
        push ebp
        mov ebp, esp
        mov esi, [esp + 8]               ;string address
        mov edi, [esp + 12]              ;key address
        vigenere:
            find_al_char:
                mov al, byte[esi]        ;string char
                cmp al, 0
                je end_vigenere
                cmp al, 'a'
                jge found_char
                inc esi
                jmp find_al_char
            found_char:
            mov ah, byte[edi]           ;key char
            sub ah, 'a'                 ;offset
            sub al, ah
            cmp al, 'a'
            jl rotate_back
            back_rotate:
            inc edi
            cmp byte[edi], 0
            je refresh_key
            back_vigenere:
            mov byte[esi], al           ;writes value back
            inc esi
            cmp byte[esi], 0
            jne vigenere
            jmp end_vigenere
        refresh_key:
            mov edi, [esp + 12]
            jmp back_vigenere
        rotate_back:
            mov ah, 'z'
            sub al, 'a'
            add al, ah
            inc al
            jmp back_rotate
        end_vigenere:
        leave
        ret

convert_base32:
        push ebp
        mov ebp, esp
        mov edx, [esp + 8]
        cmp edx, 'A'
        jge leteral
        cmp edx, '7'
        jle digital
        jmp equalal
        leteral:
            sub edx, 'A'
            jmp endconvert
        digital:
            sub edx, '2'
            add edx, 26
            jmp endconvert
        equalal:
            xor edx, edx
            jmp endconvert
        endconvert:
        mov [esp + 8], edx
        leave
        ret
        
hex_to_dec:
        push ebp
        mov ebp, esp
        mov esi, [esp + 8]              ;string address
        push esi
        mov edi, esi
        convertFromHex:
            mov bl, byte[esi]
            mov bh, byte[esi + 1]
            cmp bl, 'a'
            jl numberbl
            jge leterbl
            backbl:
            cmp bh, 'a'
            jl numberbh
            jge leterbh
            backbh:
            mov al, 16
            mul bl
            add al, bh                  ;conversion complete
            mov byte[edi], al
            inc edi
            add esi, 2
            cmp byte[esi], 0
            jne convertFromHex
            jmp FirstEnd
        numberbl:
            sub bl, '0'
            jmp backbl
        leterbl:
            sub bl, 'a'
            add bl, 10
            jmp backbl
         numberbh:
            sub bh, '0'
            jmp backbh
        leterbh:
            sub bh, 'a'
            add bh, 10
            jmp backbh
        FirstEnd:
        leave
	ret

xor_string:
    push ebp
    mov ebp, esp
    mov esi, [ebp + 8]                  ;string address
    mov eax, [ebp + 12]                 ;key value
    loop_xor_string:
        mov bl, byte[esi]
        xor bl, al
        mov byte[esi], bl
        inc esi
        cmp byte[esi], 0
        jne loop_xor_string
    leave
    ret
    
main:
	push ebp
	mov ebp, esp
	sub esp, 2300

	; test argc
	mov eax, [ebp + 8]
	cmp eax, 2
	jne exit_bad_arg

	; get task no
	mov ebx, [ebp + 12]
	mov eax, [ebx + 4]
	xor ebx, ebx
	mov bl, [eax]
	sub ebx, '0'
	push ebx

	; verify if task no is in range
	cmp ebx, 1
	jb exit_bad_arg
	cmp ebx, 6
	ja exit_bad_arg

	; create the filename
	lea ecx, [filename + 7]
	add bl, '0'
	mov byte [ecx], bl

	; fd = open("./input{i}.dat", O_RDONLY):
	mov eax, 5
	mov ebx, filename
	xor ecx, ecx
	xor edx, edx
	int 0x80
	cmp eax, 0
	jl exit_no_input

	; read(fd, ebp - 2300, inputlen):
	mov ebx, eax
	mov eax, 3
	lea ecx, [ebp-2300]
	mov edx, [inputlen]
	int 0x80
	cmp eax, 0
	jl exit_cannot_read

	; close(fd):
	mov eax, 6
	int 0x80

	; all input{i}.dat contents are now in ecx (address on stack)
	pop eax
	cmp eax, 1
	je task1
	cmp eax, 2
	je task2
	cmp eax, 3
	je task3
	cmp eax, 4
	je task4
	cmp eax, 5
	je task5
	cmp eax, 6
	je task6
	jmp task_done

task1:
        mov edx, ecx                ;string address
        findkey1:
            inc ecx
            cmp byte [ecx], 0
            jne findkey1
        inc ecx                     ;key address
        
        push ecx
        push edx
        call xor_strings
        pop ecx
        push ecx
        call puts                   ;print resulting string
        add esp, 4

        jmp task_done

task2:
        push ecx
        call rolling_xor
        pop ecx
        push ecx
        call puts
        add esp, 4

        jmp task_done

task3:
        mov edx, ecx      ;remembers the string address
        findkey3:
            inc ecx
            cmp byte [ecx], 0
            jne findkey3
        inc ecx       ;key address
	; TODO TASK 1: call the xor_hex_strings function

        push ecx  ;key address
        push edx  ;string address
        call xor_hex_strings
        pop ecx
        
        push ecx                     ;print resulting string
        call puts
        add esp, 4

	jmp task_done

task4:
        push ecx
        call base32decode
        pop ecx
        
	push ecx
	call puts                    ;print resulting string
	pop ecx
	
	jmp task_done

task5:
        push ecx
        call bruteforce_singlebyte_xor
        pop edx
        push eax
        
        push ecx                    ;print resulting string
        call puts
        pop ecx

        pop eax
	push eax                    ;eax = key value
        push fmtstr
        call printf                 ;print key value
        add esp, 8

        jmp task_done

task6:
        mov edx, ecx                ;remembers the string address
        findkey6:
            inc ecx
            cmp byte [ecx], 0
            jne findkey6
        inc ecx                     ;key address
        
        push edx                    ;remembers the string address
        
        push ecx                    ;key address
        push edx                    ;string address
        call decode_vigenere
        pop ecx

	push ecx
	call puts
	add esp, 4

task_done:
	xor eax, eax
	jmp exit

exit_bad_arg:
	mov ebx, [ebp + 12]
	mov ecx , [ebx]
	push ecx
	push usage
	call printf
	add esp, 8
	jmp exit

exit_no_input:
	push filename
	push error_no_file
	call printf
	add esp, 8
	jmp exit

exit_cannot_read:
	push filename
	push error_cannot_read
	call printf
	add esp, 8
	jmp exit

exit:
	mov esp, ebp
	pop ebp
	ret
