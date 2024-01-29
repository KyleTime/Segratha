#ifndef LINKEDLIST_H
#define LINKEDLIST_H

template<typename T>
class Node
{
    public:
        T data;
        Node<T>* next;
        Node<T>* previous;

        Node(T data);
};

/// @brief A basic LinkedList class that allows adding and removing nodes. Also allows public access to the head.
/// @tparam T type to store in LinkedList
template<typename T>
class LinkedList
{
    private:

    public:
        Node<T>* head;
        LinkedList(T headData);

        //adds a node onto the front, pushing "head" down the list
        void AddNode(T data);
        //remove the specified node
        void RemoveNode(Node<T>* node);
};

#endif