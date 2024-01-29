#include "LinkedList.h"
#include "../Game/GameObject.h"

template<typename T>
Node<T>::Node(T data)
{
    this->data = data;
    next = nullptr;
    previous = nullptr;
}

template<typename T>
LinkedList<T>::LinkedList(T headData)
{
    head = new Node<T>(headData);
}

template<typename T>
void LinkedList<T>::AddNode(T data)
{
    Node<T>* node = new Node<T>(data);

    //so basically these lines do this:
    //[node]<->[head]->...
    //[head]<->[some node]<->...
    //we make node and head point at eachother, then change the starting point of the list

    node->next = head;
    head->previous = node;
    head = node;
}

template<typename T>
void LinkedList<T>::RemoveNode(Node<T>* node)
{
    //so the idea is that we link up the next and previous nodes so that the list forgets about this node
    //so we got: [prev]->[node]<-[next]
    //and we gotta get: [prev]<->[next]

    node->previous->next = node->next;
    node->next->previous = node->previous;

    delete node;
};

template LinkedList<Segratha::GameObject*>::LinkedList(Segratha::GameObject* data);
template void LinkedList<Segratha::GameObject*>::AddNode(Segratha::GameObject* data);
template void LinkedList<Segratha::GameObject*>::RemoveNode(Node<Segratha::GameObject*>* node);