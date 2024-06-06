;assembly

section .data
    sdot dq 0.0 ; result
    msg db `%.2lf\n`,0

section .text
bits 64
default rel ; for addr reloc

global dotproduct_x86
global dotproduct_xmm
extern printf

; Format: rcx = ARR_SIZE rdx = A[] r8 = B[]


;x86 implementation
dotproduct_x86:
    xorpd xmm0, xmm0    
    xor rax, rax 
    
dotproduct1:
    movsd xmm1, [rdx + rax * 8] 
    movsd xmm2, [r8 + rax * 8]
    mulsd xmm1, xmm2 
    addsd xmm0, xmm1 

    inc rax
    loop dotproduct1

done1:
    xor rax, rax
    ret ;value is returned in xmm0


;xmm implementation
dotproduct_xmm:
    mov r9, rcx
    
    and r9, 1 ;check if odd number size
    shr rcx, 1 ;divide by 2
    xor rax, rax
    xorpd xmm0, xmm0
     
dotproduct2:  
    movdqu xmm1, [rdx + rax * 8]    
    movdqu xmm2, [r8 + rax * 8] 
    mulpd xmm1, xmm2
    haddpd xmm1, xmm1
    vaddsd xmm0, xmm1
    
    inc rax
    inc rax
    loop dotproduct2

end:
    cmp r9, 0
    jz done2
    
    ;perform 1 more dot product process for last element in odd sized arr
    movdqu xmm1, [rdx + rax * 8]    
    movdqu xmm2, [r8 + rax * 8] 
    mulpd xmm1, xmm2
    vaddsd xmm0, xmm1      
    
done2:
    xor rax,rax
    ret ;value is returned in xmm0
