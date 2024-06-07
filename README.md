# CEPARCO_MP1


## x86 SIMD AVX2 assembly language using XMM register
With the usage of the XMM register, the solution was straightforward. SIMD operations and instructions such as movdqu and mulpd, made the solution almost “C” like. The solution can be broken down into two sections: multiply the array elements and add them to the final result. Utilizing the movdqu instruction, each element of both arrays is accessed by multiplying a counter variable by 8 throughout where the input is stored in the registry. With this, elements within the array are moved to their respective XMM registers (xmm1 and xmm2). The SSE2 floating point arithmetic instruction mulpd is then used to multiply xmm1 and xmm2 together. The result is added to the total result which is stored in the xmm0 register. The program loops however many times it is set to which is based on the number of elements in both arrays.


## x86 SIMD AVX2 assembly language using YMM register
The YMM register allows more flexibility in where data is stored and can be more efficient when it comes to accessing. The first solution is where there were problems, the output was incorrect. Originally, there was an issue with the horizontal addition where it was only adding the partial sums as well as loop handling. 
