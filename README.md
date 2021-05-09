# High-Performance-Computing
This git contents the tasks of the course "HPC"-Computing at the FSU-Jena.
The lecture is hold by Alexander Breuer.


[Lecture-Homepage](https://scalable.uni-jena.de/opt/hpc/index.html)


## Contents

[TOC]

## Mircobenchmarks
Mircobenchmarking of the Neoverse N2 ARM-Processor in the aws Grvaiton V2 Server.


## Bits
1. Printing bits of variable datatypes.

2. Bitmanipulation of various inputs.

3. The variable `unsigned int i_vec_instr` encodes a Floating-point fused Multiply-Add to accumulator vector-operation and contains an instruction-vector which contains the source registers, the destination register, the (byte)-length of the participating data.
This instruction multiplies 2 floating-point values and adds the product to the corresponding vector element. The bits 0-4 (Rd) and 5-9 (Rn) are encoding the registers where the sourcedata can be found. The bits 16-20 (Rm) are encoding the destination register.

## Pipelining
1. The metrics in the [Arm Neoverse N1 Software Optimization Guide](https://developer.arm.com/documentation/swog309707/a) were:
![alt text](https://github.com/Jolles-workm8/HPC/blob/main/Pipelining/fmla_fmul.png)
2. We can calculate the theoretical GFLOP/s via : Clockcycles * ASIMD (128Bit per Operation) * Throughput * FMA / Latency(just one operation at a time)

Operation | Calculation | Result
--- | --- | ---
fmul       | 2.5*10^9 * 4 * 2 * 2 / 4 | 10 GFlop/s      
fmla       | 2.5*10^9 * 4 * 2 / 3     | 6.66666 GFlop/s

3. & 4. The result of the benchmarking using one source register were:
  ![alt text](https://github.com/Jolles-workm8/HPC/blob/main/Pipelining/fmul_fmla_src.png)

5. The result of the benchmarking using one destination register were:
  ![alt text](https://github.com/Jolles-workm8/HPC/blob/main/Pipelining/fmul_fmla_dst.png)

You can find the .csv files and the code in /Pipelines

##Small GEMM's
1. You can find the code in the folder Small_GEMMs
2. The number of required operations to solve the Matrix-Matrix product of A * B, given A is a M * K Matrix and B is a K * N Matrix is :
   2 * K * M * N

Performance of the gemm kernels with different optimization levels in GFLOP's:

type | without optimization | with O2 | with O3
--- | --- | --- | ---
gemm_ref: 4 | 0.308438 | 0.314211 | 0.314826
gemm_ref: 8 | 0.331801 | 0.335824 | 0.335214
gemm_ref: 12 | 0.337057 | 0.3367 | 0.337976
gemm_ref: 16 | 0.341165 | 0.340673 | 0.341988
gemm_ref: 24 | 0.333424 | 0.331905 | 0.333077
gemm_ref: 32 | 0.330431 | 0.332958 | 0.333319
gemm_ref: 48 | 0.327675 | 0.333685 | 0.333967
gemm_ref: 64 | 0.324477 | 0.333919 | 0.33419
gemm_compiler_mnk: | 0.371008 | 0.371415 | 0.37179
gemm_compiler_nkm: | 0.36406 | 0.363802	| 0.363637
