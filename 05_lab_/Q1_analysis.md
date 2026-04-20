**🔹 Program Overview**



Computes nth Fibonacci number using recursion + OpenMP task parallelism.



One thread starts recursion → creates tasks for Fib(n−1) and Fib(n−2) → waits → combines results.



**🔹 Key OpenMP Concepts**



Parallel (#pragma omp parallel) → Creates multiple threads.



Single (#pragma omp single) → Only one thread starts recursion.



Task (#pragma omp task) → Executes recursive calls in parallel.



Taskwait (#pragma omp taskwait) → Ensures correct result.



**🔹 Working**



Each recursive call becomes a separate task.



Tasks run simultaneously on different CPU threads.



Improves CPU utilization.



**🔹 Analysis**



Task Parallelism: Recursive calls run concurrently.



Load Balancing: OpenMP distributes tasks automatically.



Synchronization: taskwait ensures correctness.



Performance: Small n → overhead high; Large n → better parallel benefit.



Scalability: Performance improves with more cores.



**🔹 Observations**



Correct output confirms synchronization.

* 

Small input → little speed gain.



Large input → better parallel execution.



Higher CPU usage due to multi-threading.

