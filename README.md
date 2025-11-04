# 📜 get_next_line

> A function that reads and returns a single line from a file descriptor — one call at a time.

---

## 🧠 Overview
In the **get_next_line** project at 42, the goal is to implement a C function that can **read and return the next line** from a file or standard input with each call.  

Although it seems simple, it’s a fundamental exercise in **file descriptor management**, **dynamic memory allocation**, and **buffered I/O**.  
`get_next_line` forms the foundation for understanding how low-level file reading and efficient data handling work in C.

---

## ⚙️ Implementation Details
Each call to `get_next_line(fd)` must:
- Return **exactly one line** (including the newline `\n` if present).  
- Manage **partial reads** using a buffer of fixed size (`BUFFER_SIZE`).  
- **Preserve the unread portion** of the buffer between calls using a static variable.  
- Handle multiple file descriptors simultaneously (bonus part).  

The implementation relies on:
- `read()` for low-level file input  
- Dynamic memory management with `malloc` and `free`  
- Careful string manipulation and memory-safe operations  

---

## 🧩 Bonus Part
The **bonus version** extends functionality by:
- Managing **multiple file descriptors** at once  
- Using **only one static variable** (the buffer) to track remaining data  

This pushes the project toward a more **realistic system-level challenge**, closer to how Unix handles multiple streams of input.
