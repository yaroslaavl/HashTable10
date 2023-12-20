#include <iostream>
#include <string>
using namespace std;

template <typename T>
class Array {
private:
    T* array;
    int size;
    int capacity;

public:
    Array():size(0), capacity(10) {
        array = new T[ capacity];
    }
    T *index(int index){

        return &array[index];
    };
    void add(const T& num) {
        if (size == capacity ) {
            capacity *= 2 ;
            T* newArray = new T[capacity];
            for (int i = 0; i < size; i++) {
                newArray[i] = array[i];
            }
            delete [] array;
            array = newArray;
        }
        array[size]  = num;
        size++;
    }
    void clear() {
        delete[] array;
        size = 0;
        capacity = 10;
        array = new T[capacity];
    }
    ~Array() {
        delete[] array;
    }
};

template<typename T>
class Node {
public:
    Node<T> * poprzedni;
    Node<T> * nastep;
    T data;
    std::string key;
    Node(std::string key,int data, Node* poprzedni = nullptr, Node* nastep = nullptr)
            : key(key),data(data), poprzedni(poprzedni), nastep(nastep)
    {}
     friend std::ostream& operator<<(std::ostream& os,const Node<T>& node){
        os<<"(" << node.key << ", "<<node.data<<")";
         return os;
    }
};

template<typename T>
class LinkedList {
private:
    Node<T> * head;
    Node<T> * tail;
    int size;
public:
    LinkedList(Node<T>* head = nullptr, Node<T>* tail = nullptr, int size = 0)
            : head(head), tail(tail), size(size) {}

    void Koniec(std::string key, int data);

    auto *getHead(){
        return head;
    }
    friend std::ostream & operator<<(std::ostream& os, LinkedList& list);

    Node<T> *search(string key);

    Node<T> *replace(string &key, T value);
    void clear();
    string del(string key);
};
template<typename T>
void LinkedList<T>::clear(){
    if (head != nullptr)
    {
        Node<T> *temp;
        while (head != tail)
        {
            temp = head->nastep;
            delete head;
            head = temp;
        }
        delete head;
        head = nullptr;
        tail = nullptr;
    }
}
template<typename T>
void LinkedList<T>::Koniec(std::string key, int data) {
    Node<T>* jeszczeJeden = new Node<T>(key,data);
    jeszczeJeden->poprzedni = tail;
    if (tail) {
        tail->nastep = jeszczeJeden;
    } else {
        head = jeszczeJeden;
    }
    tail = jeszczeJeden;
    size++;
}
template<typename T>
Node<T>* LinkedList<T>::search(std::string key){
    Node<T>* curr = head;
    if(curr == nullptr)return nullptr;
    while(curr != nullptr) {
        if(curr->key == key) return curr;
        curr = curr->poprzedni;
    }
    return nullptr;
}
template<typename T>
 Node<T>* LinkedList<T>::replace(std::string&key, T value){
     Node<T>*curr = head;
     while(curr != nullptr){
         if(curr->key == key){
             curr->data = value;
             return reinterpret_cast<Node<int> *>(1);
         }
         curr = curr->nastep;
     }
 }
 template<typename T>
 std::string LinkedList<T>::del(std::string key){
  Node<T>*curr = search(key);
   if(curr == nullptr) {
       return "";
   }
    else {
        std::string usuniety = curr->key;
        if(curr == head) {
        head = curr->poprzedni;
        }
        else{
            Node<T>*parent = head;
            while(parent->poprzedni != curr) parent = parent->poprzedni;
            parent->poprzedni = curr->poprzedni;
        }
        delete curr;
        return usuniety;
    }
}

template<typename T>
std::ostream & operator<<( std::ostream & wyjscie, const LinkedList<T> & list ) {
    Node<T> * jeszczeJeden = list.head;
    while (jeszczeJeden != nullptr) {
        wyjscie << *jeszczeJeden << " " ;
        jeszczeJeden = jeszczeJeden->nastep;
    }
    return wyjscie;
}

template<typename T>
class Tablica{
private:
    int rozmiar;
    int capacity;
    Array<LinkedList<T>> * array;

    void rehash(){
      Array<LinkedList<T>>* rozszerzenie = new Array<LinkedList<T>>();
        int staryRozmiar = rozmiar;
        rozmiar *=2;
        for(int i = 0;i<rozmiar;i++) {
            rozszerzenie->add(*new LinkedList<T>());
        }
        for(int i = 0; i<staryRozmiar;i++) {
            Node<T>* step = array->index(i)->getHead();
            while(step){
                std::string tempKey = step->key;
                int value = step->data;
                int k = hashFunction(tempKey);
                rozszerzenie->index(k)->Koniec(tempKey,value);
              step = step->nastep;
            }
        }
        array->clear();
        delete array;
        array = rozszerzenie;
    }
public:
    Tablica<T>(){
        rozmiar = 10;
        capacity = 0;
       array = new Array<LinkedList<T>>();
       for(int i = 0;i<rozmiar;i++){
           array->add(*new LinkedList<T>());
       }
    }

    int hashFunction(std::string key){
        int sum = 0;
        for(int i = 0; i<key.length();i++){
            sum += key[i]*(31^(key.length()-i-1));
        }
        if(sum<0)sum=-sum;
        return sum % rozmiar;
    }

    void add(std::string key,T value){
        int k = hashFunction(key);
        LinkedList<T>* temp = array->index(k);
        Node<T>*sprawdzenie = temp->search(key);
        if(sprawdzenie){
            temp->replace(key, value);
        }else{
            temp->Koniec(key,value);
        }
        if(capacity>(rozmiar*2)){
            rehash();
        }
    }

    int findKey(std::string key){
        int k = hashFunction(key);
        LinkedList<T>*list = array->index(k);
        Node<T>*node = list->search(key);
        return node->data;
    }
    std::string delet(std::string key){
        int k = hashFunction(key);
        LinkedList<T>*list = array->index(k);
        return list->del(key);
    }
    void clear(){
        for(int i = 0;i<rozmiar;i++){
            LinkedList<T>* list = array->index(i);
            list->clear();
        }
        array->clear();
        capacity = 0;
    }
    std::string wypisz() {
        std::string result;
        for(int i = 0;i<rozmiar;i++){
            result +="Indeks:"+std::to_string(i);
         LinkedList<T>* list = array->index(i);
         Node<T>* curr = list->getHead();
          while(curr){
              result += " [" + curr->key + ", " + std::to_string(curr->data) + "] ";
              curr = curr->nastep;
          }
          result += "\n";
        }
        return result;
    }
};

int main(){
    clock_t start,end;
    start = clock();
    Tablica<int>* tablica = new Tablica<int>();

     tablica->add("praca1",2);
     tablica->add("praca2",3);
     tablica->add("praca3",4);
     tablica->add("praca4",8);
     tablica->add("praca5",21);
     tablica->add("praca6",12);
     tablica->add("praca7",16);
     tablica->add("praca8",12);
     tablica->add("praca8",25);


     cout<<tablica->wypisz()<<endl;

     int find = tablica->findKey("praca5");
     cout<<"Wartosc klucza: "<< find <<endl;

     cout<<endl;
     std::string find1 = tablica->delet("praca3");
     cout<<"Klucz: "<< find1 <<" usuniety"<<endl;

     cout<<endl;
     cout<<tablica->wypisz()<<endl;

     tablica->clear();

     cout<<tablica->wypisz()<<endl;

     end = clock();
    std::cout<<"Program dziala "<<(float)(end-start)/CLOCKS_PER_SEC*1000000<<" mikrosekund"<<std::endl;
    return 0;
}
