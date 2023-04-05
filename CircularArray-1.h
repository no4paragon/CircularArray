// STUDENT ORGANIC WAGYU CODE
// Author: Yuta Nakano
// San Diego State University
// CS210/CS496: Data Structures
//
// File: CircularArray.h
// Defines the CircularArray collection class.

#define VERSION_CIRARRAY 0.1

// STUDENT CODE
#ifndef PROG1_CIRCULARARRAY_H
#define PROG1_CIRCULARARRAY_H

#include "Queue.h"
#include "List.h"
#include <cstring>

namespace sdsu {
    template<typename T>
    class CircularArray : public sdsu::Queue<T>, public sdsu::List<T> {

    private:
        // Size represents how many T items the structure holds
        int curSize;

        // Capacity represents how many T items the structure CAN hold
        int curCapacity;

        // todo: head and tail counters for the circular nature
        int front = -1;
        int rear = -1;

        // the array on the heap
        T *storage;

    public:

        // This is the default class constructor. It sets the current size to 0
        CircularArray() : curSize(0) {
            // todo: initialize a default capacity storage array on the heap
            curCapacity = 10;
            storage = new T[curCapacity];
        }

        // The copy constructor!
        CircularArray(const CircularArray<T> &other) : CircularArray() {
            std::cout << "Copying . . .";
            memcpy(storage, (void*) &other, curSize);

            /*curCapacity = other.curCapacity;
            storage = new T[other.curCapacity];
            int temp = 0;

            while (temp < other.size()) {
                addLast(other.storage[(other.front + temp) % curCapacity]);
                temp++;
            }*/
        }

        ~CircularArray() override {
            delete[] storage;
        }

        void clear() override {
            curSize = 0;
            curCapacity = 10;
            front = rear = -1;
            delete[] storage;
            storage = new T[curCapacity];

            // size == 0, and the capacity and dynamic array should
            // shrink back to its default size. There is a potential
            // for a memory leak here.
        }

        T dequeue() override {
            return removeFirst();
            /*T temp = get(0);
            //void *ptr = nullptr;

            if (isEmpty()) {
                return NULL;
            }

            if (front == rear) {
                front = rear = -1;
            } else {
                front = (front + 1) % curCapacity;
            }

            curSize--;
            return temp;*/
        }

        bool enqueue(T t) override {
            return insert(size(), t);
        }

        bool isEmpty() const override {
            return size() == 0;
        }

        T &peek() override {
            // todo: this is here to make it compile, but it is wrong.
            return storage[front];
        }

        bool addFirst(T t) override {
            // a well written insert method makes this a single line.
            return insert(0, t);
        }

        bool addLast(T t) override {
            // a well written insert method makes this a single line.
            return insert(size(), t);
        }

        T &get(int idx) override {
            // todo: we need idx range checking. In C++, this is dangerous.
            if (idx < 0 || idx >= size()) {
                throw std::out_of_range("CircularArray<T>::get(idx) : index out of range.");
            }

            // returning item at the given logical index.
            return storage[(front + idx) % curCapacity];

            //logical index
            //The logical index treats the "front" as index 0 at all times.
            // Mathematically: Logical Index = (Index - Front) cycled around capacity.
            // To find the -actual- index, it's the other direction: Index = (Logical Index + Front) cycled around capacity
            // The user only cares about the logical index. The thing at the front is 0, the next thing 1, etc.
            // But the implementation cares about the actual index. Front might be 3 or 7 or 29 or any other number.
            // If we stick something at the front, then we have to mess with the Front index. The user, however,
            // only has to insert something at logical index 0.
        }

        bool insert(int idx, T t) override {

            //if there is no space
            if (size() + 1 > curCapacity) {
                //doubles temp array.
                T *temp = new T[curCapacity * 2];

                // set t = 0 to grab from head to tail
                int t = 0;

                // copying things over.
                while (t < size()) {
                    temp[t] = get(t);
                    t++;
                }

                //recording doubled capacity.
                curCapacity *= 2;
                rear = size() - 1;
                front = 0;
                delete[] storage;
                storage = temp;
            }

            //Empty
            if (isEmpty()) {
                front = rear = 0;
                storage[0] = t;
                curSize++;

                //closer to the front
            } else if (idx < size() - idx) {

                /*   if(front == 0){
                         front = curCapacity - 1;
                     }else{
                       front --;
                     }*/

                //Add entire capacity then move -1.
                front = (front + curCapacity - 1) % curCapacity;
                int temp = 0;

                //Copying things counterclockwise.
                while (temp < idx) {
                    storage[(front + temp) % curCapacity] = storage[(front + temp + 1) % curCapacity];
                    temp++;
                }

                //actually putting things.
                storage[(front + temp) % curCapacity] = t;
                curSize++;

            // closer to the rear
            } else {

                int temp = 0;
                // How far back to go instead of how far forward to go.
                int idx2 = size() - idx;
                rear = (rear + 1) % curCapacity;

                //Copying things clockwise.
                while (temp < idx2) {
                    storage[(rear + curCapacity - temp) % curCapacity] = storage[(rear + curCapacity - temp - 1) % curCapacity];
                    temp++;
                }

                storage[(front + idx) % curCapacity] = t;
                curSize++;
            }
            // if you figure out if you are closer to the head or tail, you can
            // minimize the amount of array manipulation you need to perform.
            return false;
        }

        T remove(int i) override {

            if (i < 0 || i >= size()) {
                throw std::out_of_range("CircularArray<T>::remove(i) : index out of range.");
            }

            int temp = 0;
            // storing the value of the logical index to remove.
            T tempStorage = storage[(front + i) % curCapacity];

            //If it's closer to front.
            if (i < size() - i) {
                //temp index
                int t_idx = i + curCapacity + front;

                while ((t_idx - temp) % curCapacity != front) {
                    //copying from left to right.
                    storage[(t_idx - temp) % curCapacity] = storage[(t_idx - temp - 1) % curCapacity];
                    temp++;
                }

                front = (front + 1) % curCapacity;
                curSize--;

            } else {
                int t_end = rear + curCapacity;
                int t_idx = front + i;
                temp = 0;

                while ((t_idx + temp) % curCapacity != rear) {
                    storage[(t_idx + temp) % curCapacity] = storage[(t_idx + temp + 1) % curCapacity];
                    temp++;
                }
                rear = (t_end - 1) % curCapacity;
                curSize--;
            }
            return tempStorage;
        }

        T removeFirst() override {
            // If you wrote remove correctly, this can be a single line.
            // you *might* want to clean up any exception handling though . . . .
            return remove(0);
        }

        T removeLast() override {
            // If you wrote remove correctly, this can be a single line.
            // you *might* want to clean up any exception handling though . . . .
            return remove(size() - 1);
        }

        void set(int idx, T value) override {
            throw std::out_of_range("CircularArray<T>::set(idx,value) : index out of range.");
        }

        int size() const override {
            return curSize;
        }
    };
}

#endif //PROG1_CIRCULARARRAY_H
