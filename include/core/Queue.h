#ifndef QUEUE_H
#define QUEUE_H

#include <cstdlib>
#include <stdexcept> 

template <typename T>
struct Node {
    // Component of a queue
    T data; 
    Node* next; 
    Node(const T& val) : data(val), next(nullptr) {}
}; 

template <typename T>
class Queue {
    // FIFO data container for Node elements. 

    private: 
        Node<T>* head; 
        Node<T>* tail; 
        std::size_t size; 
    
    public: 
        // Constructors
        Queue() {
            // Define Queue with null pointers 
            this->head = nullptr; 
            this->tail = nullptr; 
            this->size = 0;
        }; 
        ~Queue() {
            // Clear the queue 
            this->clear(); 
        }; 
        Queue(const Queue& other) : head(nullptr), tail(nullptr), size(0) {
            Node<T>* current = other.head;
            while (current != nullptr) {
                this->push(current->data);
                current = current->next;
            }
        }; 
        Queue& operator=(const Queue& other) {
            if (this == &other) {
                return *this; 
            }
            // Delete its own memory 
            this->clear(); 
            // Get other elements 
            // Deep-copy for all nodes 
            Node<T>* current = other.head; 
            while(current != nullptr) {
                this->push(current->data); 
                current = current->next; 
            }
            return *this;
        }; 
        Queue(Queue&& other) noexcept {
            // Steal other pointers 
            this->head = other.head; 
            this->tail = other.tail; 
            this->size = other.size; 
            // Reinit other pointers to prevent destruction 
            other.head = nullptr; 
            other.tail = nullptr; 
            other.size = 0; 
        }; 
        Queue& operator=(Queue&& other) noexcept {
            // Copy other content in this object 
            if (this == &other) {
                return *this; 
            }
            // Delete current values
            this->clear(); 

            // Get source content
            this->head = other.head;
            this->tail = other.tail;
            this->size = other.size;

            // Clear source object 
            other.head = nullptr;
            other.tail = nullptr;
            other.size = 0;

            return *this; 
        }; 

        // Access  
        bool empty() const {
            return (this->size == 0); 
        }; 
        std::size_t get_size() const {return this->size; }; 
        T& front() { 
            if (this->empty()) {
                throw std::out_of_range("Try to access front() of an empty Queue.");
            }
            return this->head->data; 
        }; 
        const T& front() const { 
            if (this->empty()) {
                throw std::out_of_range("Try to access front() of an empty Queue.");
            }
            return this->head->data; 
        }; 
        T& back() { 
            if (this->empty()) {
                throw std::out_of_range("Try to access back() of an empty Queue.");
            }
            return this->tail->data; 
        }; 
        const T& back() const {
            if (this->empty()) {
                throw std::out_of_range("Try to access back() of an empty Queue.");
            }
            return this->tail->data;
        }; 

        // Modifyers 
        void push(const T& value) {
            Node<T>* n = new Node<T>(value);
            if (this->empty()) {
                this->head = n;
                this->tail = n;
            } else {
                this->tail->next = n;
                this->tail = n;
            }
            this->size++;
        }
        void pop() {
            if (this->empty()) throw std::out_of_range("Unable to pop() an empty Queue."); 
            
            Node<T>* new_head = this->head->next;
            delete this->head; 
            this->head = new_head; 
            this->size -= 1; 

            if (this->size == 0) {
                this->tail = nullptr;
            }
        }
        void clear() {
            // Clear the Queue 
            while(this->empty() == false) {
                this->pop(); 
            }
            // Erase local values 
            this->head = nullptr; 
            this->tail = nullptr; 
        }; 

        // Comparison 
        bool operator==(const Queue& other) const {
            if (this->size != other.size) return false;
            
            Node<T>* cur1 = this->head;
            Node<T>* cur2 = other.head;
            while (cur1 != nullptr && cur2 != nullptr) {
                if (cur1->data != cur2->data) return false;
                cur1 = cur1->next;
                cur2 = cur2->next;
            }
            return true; 
        }

        bool operator!=(const Queue& other) const {
            return !(*this == other); 
        }
}; 

#endif 