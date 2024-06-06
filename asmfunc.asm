;assembly

section .data
    sdot dq 0.0 ; result
    zero dq 0.0 ; always 0
    msg db `%.2lf\n`,0

section .text
bits 64
default rel ; for addr reloc

global dotproduct_x86
global dotproduct_xmm
global dottest
extern printf

; Format: rcx = A[] rdx = B[] r8 = ARR_SIZE


;x86 implementation
dotproduct_x86:
    movq xmm0, qword [zero]    
    xor rax, rax 
    
dotproduct1:
    cmp rax, r8
    jge done1

    movsd xmm1, [rcx + rax * 8] 
    movsd xmm2, [rdx + rax * 8]
    mulsd xmm1, xmm2 
    addsd xmm0, xmm1 

    inc rax
    jmp dotproduct1

done1:
    movq rax, xmm0       ; Move result to rax for return
    ret


;xmm implementation
dotproduct_xmm:
    movq xmm0, qword [zero]
    xor rax, rax 

dotproduct2:
    cmp rax, r8
    jge done2
    
    movupd xmm1, [rcx + rax * 8]    
    movupd xmm2, [rdx + rax * 8] 
    mulpd xmm1, xmm2
    haddpd xmm1, xmm1
    
    vaddsd xmm0, xmm1
    
    inc rax
    inc rax
    jmp dotproduct2
    
done2:
    xor rax,rax
    ret ;value is returned in xmm0

dottest:
    movq xmm0, qword [sdot]
    
    shr rcx, 1    
    xor rax,rax
loop_start:
    cmp rax, rcx
    jge done
    
    movdqu xmm1, [rdx]    
    movdqu xmm2, [r8] 
    mulpd xmm1, xmm2

    movhlps xmm2, xmm1
    haddpd xmm1, xmm1
    addsd xmm0, xmm1
    
    add rdx, 16
    add r8, 16
    inc rax
    jmp loop_start
    
done:
    movsd [sdot], xmm0          

    sub rsp, 8*5     
    mov rcx, msg                
    mov rdx, [sdot]           
    call printf                  
    add rsp, 8*5                
    xor rax, rax                
    ret