# CEPARCO MP1
### S11 Group 6
### Javier, Juan Diego V. | Kho, John Zechariah S. | Tumalad, Shawne Michael Q.

## Program
This program computes the dot product by iterating through each element of the two input arrays, multiplying corresponding elements together, and summing up all the products.
<br><br>The function is made in 4 different versions:
* a C function
* an x86 function
* an xmm function
* a ymm function

Each function was run 30 times and the time it took for each run is averaged.

![Alt text](README/C_output.png)

The image showing the C function averaging process is identical to the other 3 functions shown in the program

## Results and Analysis
### Array size 2^20
#### Debug
![Alt text](README/debug%202^20.png)
#### Release
![Alt text](README/release%202^20.png)
#### Comparison table
| Average Runtime in ms | C | x86 | XMM | YMM |
| - | - | - | - | - |
| Debug | 4.420053 | 3.485913 | 1.549047 | 1.377023 |
| Release | 2.688383 | 2.496110 | 2.178513 | 1.905723 |

EXPLAIN HERE

### Array size 2^26
#### Debug
![Alt text](README/debug%202^26.png)
#### Release
![Alt text](README/release%202^26.png)
#### Comparison table
| Average Runtime in ms | C | x86 | XMM | YMM |
| - | - | - | - | - |
| Debug | 201.624820 | 136.3880q53 | 89.870213 | 77.702683 |
| Release | 88.977820 | 119.052030 | 84.965710 | 70.788297 |

EXPLAIN HERE

### Array size 2^28
#### Debug
![Alt text](README/debug%202^28.png)
#### Release
![Alt text](README/release%202^28.png)
#### Comparison table
| Average Runtime in ms | C | x86 | XMM | YMM |
| - | - | - | - | - |
| Debug | 702.918997 | 493.658220 | 314.526787 | 282.809700 |
| Release | 419.571883 | 489.864893 | 333.865140 | 280.233303 |

EXPLAIN HERE

## x86-64 Program
This x86 assembly code snippet calculates the dot product of two arrays of double-precision floating-point numbers using SIMD instructions and XMM registers. The function initializes by zeroing out the xmm0 register to store the cumulative sum of products and clears the rax register for use as a loop counter. During each iteration of the loop, it loads corresponding elements from the arrays (addressed by rdx and r8) into xmm1 and xmm2. These elements are then multiplied together (mulsd), and the result is added to the cumulative sum in xmm0 (addsd). The loop continues until all elements are processed, incrementing rax after each iteration to advance the array index. The function concludes by zeroing rax for clean state management and returns, with the final dot product result stored in xmm0.

## x86 SIMD AVX2 assembly language using XMM register
With the usage of the XMM register, the solution was straightforward. SIMD operations and instructions such as movdqu and mulpd, made the solution almost “C” like. When first created, the solution was created in SASM first to test the proof of concept before implementing it into C to 86x. Trying to port over the SASM version into Visual Studio 2022, there were some adjustments to which registers were used. The solution can be broken down into two sections: multiply the array elements and add them to the final result. Utilizing the movdqu instruction, each element of both arrays is accessed by multiplying a counter variable by 8 throughout where the input is stored in the registry. With this, elements within the array are moved to their respective XMM registers (xmm1 and xmm2). The SSE2 floating point arithmetic instruction mulpd is then used to multiply xmm1 and xmm2 together. The result is added to the total result which is stored in the xmm0 register. The program loops however often it is set to which is based on the number of elements in both arrays. After a few tests with the final iteration of the YMM section of the program, it was noticed that there were small discrepancies in the decimal section of the output after kernel version 2^26 and onwards. With SIMD XMM and YMM registries, using large numbers and different arithmetic instructions may lead to the result being too large to represent or in the case of floating point, too small to represent (many decimal places). Floating point representation in IEEE-754, underflow occurs when the number is too small to represent. This causes the number to not be represented in full precision, also known as a gradual underflow.

## x86 SIMD AVX2 assembly language using YMM register
The YMM register allows more flexibility in where data is stored and can be more efficient when it comes to accessing. The first solution is where there were problems, the output was incorrect. Originally, there was an issue with the horizontal addition where it only added the partial sums and loop handling. The solution with using the YMM registers is similar to XMM, with the main difference being the usage of horizontal addition. As mentioned before, there is a small issue with the representation in kernels after 2^26 due to gradual underflow. With that in mind, the rest of program is similar to XMM counter part but the main difference being the usage of horizontal operations for better efficiency.


## Methodology
The main reasons for the performance differences between the x86 assembly implementations with XMM and YMM registers and the C code are related to how these techniques manage data and use CPU resources. The sequential memory access of the C implementation might not be as effective in making use of cache lines and data prefetching capabilities of contemporary CPUs. Multiple data points (two double-precision floats per XMM register) can be processed concurrently using SIMD (Single Instruction, Multiple Data) instructions when XMM registers are used. By using a single instruction to process several pieces of data at once, this improves throughput. 
The YMM-based implementation is the fastest because it maximizes the data processing capabilities of the CPU by using the widest registers available (YMM), minimizing loop iterations, and efficiently using memory and cache resources. Each instruction does more work in less time, leading to significant performance gains compared to the XMM implementation and especially over the traditional C implementation.
In conclusion, the YMM implementation leverages modern processor features more effectively, which explains its superior performance compared to the XMM implementation and much more so over the C code version. Because it minimizes loop iterations, makes optimal use of memory and cache resources, and maximizes the CPU's data processing capabilities by utilizing the widest registers (YMM), the YMM-based implementation is the fastest. There are noticeable performance gains over the XMM implementation and especially over the conventional C implementation since each instruction completes more work in less time. In conclusion, the YMM implementation leverages modern processor features more effectively, which explains its superior performance compared to the XMM implementation and much more so over the C code version.

## Challenges
There were several obstacles to overcome when converting assembly code made in SASM to VSCode 2022. The use of different registers from a groupmate's host machine added another layer in the adaptation of the SASM function to VS. Debugging assembly becomes more complex because of the low-level operations and challenges with data visualization associated with wider YMM registers. We primarily debugged the assembly code in SASM because it usually worked directly in VS. The challenges we experienced in that process was the proper use of the registers as sometimes we forgot to use address pointers and instead used the address values and vice versa. Implementing boundary handling was also a challenge as tried to figure out what was a very efficient way of performing it. The solution we came up with was using the AND operator to determine extra elements on the array.

Another problem was the differences in the final answers of the dot product function between the xmm and ymm registers. The different decimal places of the dot product results between XMM and YMM can be ascribed to the rounding behaviors and precision of IEEE-754 floating-point arithmetic. When compared to XMM, the wider YMM registers process more data concurrently, which can change the way rounding errors accumulate and produce slightly different outcomes.
