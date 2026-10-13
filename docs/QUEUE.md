# Generic Queue Implementation Specification

## Overview

The `Queue<T>` component is a standalone, template-based, generic First-In-First-Out (FIFO) data container implemented in C++20. It serves as an internal foundational structure for event logging, FIFO pipelines, and breadth-first search traversals within the BioChroma engine.

---

## Memory & Structural Architecture

The container is implemented using a singly linked list with dynamic node allocation:
* **Head Pointer (`Node* head`)**: Marks the front of the queue. Elements are dequeued (`pop`) from this position in $\mathcal{O}(1)$.
* **Tail Pointer (`Node* tail`)**: Marks the rear of the queue. Elements are enqueued (`push`) to this position in $\mathcal{O}(1)$.
* **Element Counter (`size_t size_`)**: Tracks the exact number of active elements in $\mathcal{O}(1)$.

### Internal Node Schema
```cpp
template <typename T>
struct Node {
    T data;
    Node* next;
    Node(const T& val) : data(val), next(nullptr) {}
};
```

---

## Interface Specification

1. **Lifecycle & Resource Management (Rule of Five)**
- `Queue()`: Default constructor. Initializes an empty queue (`head = nullptr`, `tail = nullptr`, `size_ = 0`).
- `~Queue()`: Destructor. Iteratively frees all remaining nodes on the heap.
- `Queue(const Queue& other)`: Deep copy constructor. Replicates node structure and payload sequence.
- `Queue& operator=(const Queue& other)`: Copy assignment operator. Frees existing nodes and replicates other safely (using copy-and-swap idiom).
- `Queue(Queue&& other) noexcept`: Move constructor. Steals pointers and state from other without dynamic reallocation.
- `Queue& operator=(Queue&& other) noexcept`: Move assignment operator.

2. **Capacity & Element Access**
- `bool empty() const`: Returns `true` if `size_ == 0`, otherwise `false`.
- `size_t size() const`: Returns the number of currently retained elements.
- `T& front()`: Accesses the front element. Throws `std::out_of_range` if the queue is empty.
- `const T& front() const`: Const accessor for the front element.
- `T& back()`: Accesses the most recently enqueued element. Throws `std::out_of_range` if the queue is empty.
- `const T& back() const`: Const accessor for the back element.

3. **Modifiers**
- `void push(const T& value)`: Appends an element to the back of the queue.
- `void pop()`: Removes the front element. Throws `std::out_of_range` if the queue is empty.
- `void clear()`: Deallocates all elements and resets the queue to an empty state.

4. **Comparison & Stream Operators**
- `bool operator==(const Queue& other) const`: Equality check. Compares elements sequentially.
- `bool operator!=(const Queue& other) const`: Inequality check.