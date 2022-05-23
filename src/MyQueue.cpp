#pragma once

#include <stdexcept>

template <typename T> class QueueNode;
template <typename T> class Queue;

template <typename T>
class QueueNode 
{
    friend class Queue<T>;

    public:         

        QueueNode(T val, QueueNode<T> *next = nullptr);
        
        T val;
        QueueNode<T>* next = nullptr;
        
};

template <typename T>
QueueNode<T>::QueueNode(T val, QueueNode<T>* next)
{
    this->val = val;
    this->next = next;
}

//  Common queue
template <typename T>
class Queue
{

    friend class QueueNode<T>;

    public:

        Queue();
        ~Queue();

        bool empty();
        void push(T val);
        void pop();
        T front();
        T back();

    private:

        QueueNode<T> *first = nullptr;
        QueueNode<T> *last = nullptr;
};

template <typename T>
Queue<T>::Queue()
{
    first = nullptr;
    last = nullptr;
}

template <typename T>
Queue<T>::~Queue()
{
    QueueNode<T>* current;
    while (first) {
        current = first;
        first = first->next;
        delete current;
    }
    last = nullptr;
}

//  Checks if the queue is empty. Returns true if so and false if not.
//
template <typename T>
bool Queue<T>::empty()
{
    if (!first)
        return true;
    return false;
}

//  Adds new element at the end of queue
//          val       -   value of new elements
template <typename T>
void Queue<T>::push(T val)
{
    QueueNode<T>* newNode = new QueueNode<T>(val);
    if (!first) {
        first = last = newNode;
    } else {
        last->next = newNode;
        last = newNode;
    }
}

//  Removes first queue element.
//
template <typename T>
void Queue<T>::pop()
{
    if (!first) {
        throw std::out_of_range("Tried to pop empty queue");
    } else if (first == last) {
        QueueNode<T>* temp = first;
        first = last = nullptr;
        delete temp;
    } else {
        QueueNode<T>* temp = first;
        first = first->next;
        delete temp;
    }
}

//  Returns value of first element in queue
//
template <typename T>
T Queue<T>::front()
{
    return first->val;
}

//  Returns value of last element in queue
//
template <typename T>
T Queue<T>::back()
{
    return last->val;
}

