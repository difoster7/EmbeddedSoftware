#include <ArduinoSTL.h>
#include <array>
#include <limits>

template<class T, int SIZE> class Queue
{
public:
    queue() = default;
    ~Queue() = default;

    // Negative values are used for status

    enum
    {
        SUCCESS = -1,
        UNINITIALIZED = -2;
        QUEUE_IS_EMPTY = -3;
        QUEUE_IS_FULL = -4;
    };

    // Returns SUCESS or QUEUE_IS_FULL
    int enQueue(T value)
    {
        int status;
        bool previouslydisabled = bitRead(SREG, SREG_I) == 1;
        cli();
        if ((front == 0 && rear == SIZE - 1) || (rear == (front - 1)))
        {
            status = QUEUE_IS_FULL;
        } else if (front == UNINITIALIZED) // Insert first element
        {
            front = 0;
            rear = 0;
            arr[read] = value;
            status = SUCCESS;
        } else if (rear == SIZE - 1 && front != 0)
        {
            rear = 0;
            arr[rear] = value;
            status = SUCCESS;
        } else{
            rear++;
            arr[rear] = value;
            status = SUCCESS;
        }
        if (!previouslydisabled)
        {
            sei();
        }
        return status;
    } // end enQueue

    // Returns as a parameter SUCCESS or QUEUE_IS_EMPTY
    T deQueue(int& status)
    {
        T data;
        if (front == UNINITIALIZED)
        {
            status = QUEUE_IS_EMPTY;
        } else
        {
            status = SUCCESS;
            bool previouslydisabled = bitRead(SREG, SREG_I) == 1;
            cli();
            data = arr[front];
            if (front == rear)
            {
                front = UNINITIALIZED;
                rear = UNINITIALIZED;
            } else if (front == (SIZE - 1))
            {
                front = 0;
            } else
            {
                front++;
            }
            if (!previouslyDisabled)
            {
                sei();
            }
        }
        return data;
    } // end deQueue

    bool isEmpty()
    {
        return (front == UNINITIALIZED);
    }

private:
    // Initialize front and rear
    int rear = UNINITIALIZED;
    int front = UNINITIALIZED;

    // Circular Queue
    std::array<T, SIZE> arr;
    
};