;assembly

section .text
bits 64
default rel ; for addr reloc

global dotproduct_x86
global dotproduct_xmm
global dotproduct_ymm

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

end1:
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
    vaddpd xmm0, xmm1
    
    add rax, 2
    loop dotproduct2

xbd1:
    cmp r9, 0
    jz end2
    
    ;perform 1 more dot product process for last element in odd sized arr
    movdqu xmm1, [rdx + rax * 8]    
    movdqu xmm2, [r8 + rax * 8] 
    mulpd xmm1, xmm2
    vaddpd xmm0, xmm1      
    
end2:
    xor rax,rax
    ret ;value is returned in xmm0


;ymm implementation
dotproduct_ymm:
    vxorpd ymm0, ymm0, ymm0
    mov r12, rcx
    
    and r12, 3 ;indicates number of elems outside boundary(0-3)
    shr rcx, 2 ;divide by 4
    xor rax, rax 

dotproduct3:
    vmovupd ymm1, [rdx + rax * 8]
    vmovupd ymm2, [r8 + rax * 8]
    vmulpd ymm1, ymm1, ymm2
    vaddpd ymm0, ymm0, ymm1

    add rax, 4
    loop dotproduct3

done3:
    vextractf128 xmm1, ymm0, 1
    vaddpd xmm0, xmm0, xmm1
    vhaddpd xmm0, xmm0, xmm0

ybd2:
    cmp r12, 2 ;checks if there are 2 or more boundary elements
    jnge ybd1
    
    movdqu xmm1, [rdx + rax * 8]    
    movdqu xmm2, [r8 + rax * 8] 
    mulpd xmm1, xmm2
    haddpd xmm1, xmm1
    vaddpd xmm0, xmm1   
    
    add rax, 2     
    sub r12, 2
    
ybd1:
    cmp r12, 1 ;checks if there is 1 boundary element left
    jl end3
    
    movdqu xmm1, [rdx + rax * 8]    
    movdqu xmm2, [r8 + rax * 8] 
    mulpd xmm1, xmm2
    vaddpd xmm0, xmm1

end3:
    xor rax, rax
    ret ;value is returned in xmm0