<img src="https://capsule-render.vercel.app/api?type=waving&color=gradient&customColorList=6,11,20&height=180&section=header&text=HPC%20Lab&fontSize=44&fontColor=fff&animation=twinkling&desc=High%20Performance%20Computing%20Laboratory&descSize=16&descAlignY=75" width="100%"/>

<div align="center">

# HPC Lab -- High Performance Computing

[![C](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=black)](https://en.cppreference.com/w/c)
[![OpenMP](https://img.shields.io/badge/OpenMP-0071C5?style=for-the-badge&logo=intel&logoColor=white)](https://www.openmp.org)
[![MPI](https://img.shields.io/badge/MPI-FF6600?style=for-the-badge&logo=gnu&logoColor=white)](https://www.open-mpi.org)

</div>

---

## Overview

Lab exercises and assignments from the **High Performance Computing** course. Covers parallel programming paradigms including shared memory (OpenMP), distributed memory (MPI), and GPU computing basics.

---

## Lab Topics

| Lab | Topic | Technique |
|---|---|---|
| Lab 1 | Introduction to Parallel Computing | OpenMP basics |
| Lab 2 | Matrix Multiplication | OpenMP parallelization |
| Lab 3 | Parallel Sorting | Merge sort, Odd-Even sort |
| Lab 4 | MPI Point-to-Point Communication | MPI_Send / MPI_Recv |
| Lab 5 | MPI Collective Operations | MPI_Bcast, MPI_Reduce |
| Lab 6 | Parallel Graph Traversal | BFS / DFS with OpenMP |
| Lab 7 | Hybrid MPI + OpenMP | Combined approach |

---

## Running Labs

```bash
# OpenMP example
gcc -fopenmp lab1.c -o lab1 && ./lab1

# MPI example
mpicc lab4.c -o lab4 && mpirun -np 4 ./lab4
```

---

**Author:** [Gagan Diwakar](https://portfolio-gagan-nu.vercel.app/) | [GitHub](https://github.com/Divgagan)

<img src="https://capsule-render.vercel.app/api?type=waving&color=gradient&customColorList=6,11,20&height=100&section=footer" width="100%"/>