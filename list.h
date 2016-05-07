#ifndef LIST_H
#define LIST_H

#include <exception>
#include <iostream>
#include <fstream>

#include "node.h"

using namespace std;

class ListException : public std::exception {
private:
    std::string msg;
public:
    explicit ListException(const char* message) : msg(message) {}
    explicit ListException(const std::string& message) :msg(message) {}
    virtual ~ListException() throw () {}
    virtual const char* what() const throw () {
        return msg.c_str();
    }
};

template <class T>
class List {
private:
    Node<T>* header;
    unsigned int counter;

    bool isValidPos(Node<T>*);
public:
    List();
    ~List();
    List(const List<T> &);

    bool isEmpty();

    void insertData(Node <T>*, const T&);
    void deleteData(Node <T>*);

    Node<T>* getFirstPos();
    Node<T>* getLastPos();
    Node<T>* getPrevPos(Node<T>*);
    Node<T>* getNextPos(Node<T>*);
    Node<T>* findData(const T&);

    T& retrieve (Node<T>*);

    void printData();
    void deleteAll();

    void readFromDisk(const std::string&);
    void writeToDisk(const std::string&);
};

template <class T>
bool List<T>::isValidPos(Node<T>* p) {
    if(isEmpty()) {
        return false;
    }

    Node<T>* aux = header->getNext();
    do {
        if(aux == p) {
            return true;
        }
        aux = aux->getNext();
    } while(aux != header);

    return false;
}

template <class T>
List<T>::List() {
    header =  new Node<T>();

    if(header == nullptr) {
        throw ListException("Memoria no disponible al iniciar la lista");
    }

    header->setNext(header);
    header->setPrev(header);

    counter = 0;
}

template <class T>
List<T>::~List() {
    deleteAll();
    delete header;
}

template <class T>
List<T>::List(const List<T>& l) : List() {
    Node<T>* aux = l.header.getNext();

    while(aux != l.header) {
        insertData(getLastPos(), aux->getData());
        aux = aux->getNext();
    };
}

template <class T>
bool List<T>::isEmpty() {
    return header == header->getNext();
}

template <class T>
void List<T>::insertData(Node <T>* p, const T& e) {
    if(!isValidPos(p) and p != nullptr) {
        throw ListException("Posición no valida al tratar de insertar");
    }

    Node<T>* aux;

    try {
        aux = new Node<T>(e);
    } catch (NodeException ex) {
        throw ListException("Error al crear el nodo al INSERTAR DATOS");
    }

    if(aux == nullptr) {
        throw ListException("Error, no hay espacio de memoria al tratar de insertar");
    }

    if(p == nullptr) {
        p = header;
    }

    aux->setPrev(p);
    aux->setNext(p->getNext());

    p->getNext()->setPrev(aux);
    p->setNext(aux);

    counter++;
}

template <class T>
void List<T>::deleteData(Node <T>* p) {
    if(!isValidPos(p)) {
        throw ListException("Error al eliminar, posicion no valida");
    }

    p->getPrev()->setNext(p->getNext());
    p->getNext()->setPrev(p->getPrev());

    delete p;
    counter--;
}

template <class T>
Node<T>* List<T>::getFirstPos() {
    if (isEmpty()) {
        return nullptr;
    }

    return header->getNext();
}

template <class T>
Node<T>* List<T>::getLastPos() {
    if (isEmpty()) {
        return nullptr;
    }

    return header->getPrev();
}

template <class T>
Node<T>* List<T>::getPrevPos(Node<T>* p) {
    if(!isValidPos(p) or p == getFirstPos()) {
        return nullptr;
    }

    return p->getPrev();
}

template <class T>
Node<T>* List<T>::getNextPos(Node<T>* p) {
    if(!isValidPos(p) or p == getLastPos()) {
        return nullptr;
    }

    return p->getNext();
}

template <class T>
Node<T>* List<T>::findData(const T& p) {
    if(isEmpty()) {
        return nullptr;
    }

    Node<T>* aux = header->getNext();

    while(aux != header) {
        if(aux->getData() == p) {
            return aux;
        }
        aux = aux->getNext();
    }

    return nullptr;
}

template <class T>
T& List<T>::retrieve(Node<T>* p) {
    if(!isValidPos(p)) {
        throw ListException("Posicion no valida en RETRIEVE");
    }

    return p->getData();
}

template <class T>
void List<T>::printData() {
    if(isEmpty()) {
        return;
    }

    Node<T>* aux = header->getNext();

    while(aux != header) {
        std::cout << aux->getData() << endl;
        aux = aux->getNext();
    }
}

template <class T>
void List<T>::deleteAll() {
    if(isEmpty()) {
        return;
    }

    Node<T>* aux;

    while(header->getNext() != header) {
        aux = header->getNext();
        header->setNext(header->getNext()->getNext());
        delete aux;
    }

    counter = 0;
}

template <class T>
void List<T>::readFromDisk(const string& fileName) {
    ifstream myFile;
    myFile.open(fileName.c_str(), ios_base::in);

    T d;

    if(!myFile.is_open()) {
        throw ListException("Error al LEER DE DISCO");
    }

    deleteAll();

    try {
        while(myFile >> d) {
            insertData(getLastPos(), d);
        }
    } catch (ListException ex) {
        ListException("Error al LEER DE DISCO");
    }
}

template <class T>
void List<T>::writeToDisk(const string& fileName) {
    ofstream myFile;
    myFile.open(fileName.c_str(), ios_base::out);

    Node<T>* aux = header->getNext();

    if(isEmpty()) {
        throw ListException("Insuficiencia de datos al ESCRIBIR EN ARCHIVO");
    }

    if(myFile.is_open()) {
        do {
            myFile << aux->getData();

            aux = aux->getNext();
        } while(aux != header);

        myFile.close();

    } else {
        throw ListException("Error de ESCRITURA EN DISCO, no se pudo abrir el archivo");
    }
}

#endif // LIST_H
