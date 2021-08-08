
#include <iostream>
#include <string>
#include <list>
#include <iterator>
using namespace std;



enum Actions {
    ADD_BEG,
    ADD_END,
    DEL_BEG,
    DEL_END,
    PRINT_FORWARD,
    PRINT_BACKWARD,
    SIZE,
    PUSH,
    POP,
    PRINT_QUEUE,
    COUNT,
    GARBAGE_SOFT,
    GARBAGE_HARD,
    END,
    BORDER
};

Actions stringToEnum(const string& option) {
    if (option == "ADD_BEG") return ADD_BEG;
    else if (option == "ADD_END") return ADD_END;
    else if (option == "DEL_BEG") return DEL_BEG;
    else if (option == "DEL_END") return DEL_END;
    else if (option == "PRINT_FORWARD") return PRINT_FORWARD;
    else if (option == "PRINT_BACKWARD") return PRINT_BACKWARD;
    else if (option == "SIZE") return SIZE;
    else if (option == "PUSH") return PUSH;
    else if (option == "POP") return POP;
    else if (option == "PRINT_QUEUE") return PRINT_QUEUE;
    else if (option == "COUNT") return COUNT;
    else if (option == "GARBAGE_SOFT") return GARBAGE_SOFT;
    else if (option == "GARBAGE_HARD") return GARBAGE_HARD;
    else if (option == "BORDER") return BORDER;
    else  return END;
}

struct Node {
public:
    int data;
    Node* npx;
};

struct Queue {
    int size = 0;
    Node* front = NULL;
    Node* preRear= NULL;
    Node* rear= NULL;
    Node* preFront = NULL;
    
};


Node* XOR(Node* prev, Node* next) {
    return (Node*)((uintptr_t)prev ^ (uintptr_t)next);
}



void printNode(Node* node) {
    if (node != NULL)
        cout << node->data << endl;
    else
        cout << "NULL" << endl;
}

int findSize(Node* head, Node* end)
{
    Node* curr = head;
    Node* next;
    Node* prev = NULL;

    int res = 0;
    while (curr != end)
    {
        next = XOR(prev, curr->npx);
        res++;
        prev = curr;
        curr = next;
    }
    return res;
}


bool isInQueue(Queue* queue, Node* node, Node* tail) {

    Node* curr = queue->front;
    Node* prev = queue->preFront;
    Node* next;
    bool check = 0;

    while (curr != queue->rear)
    {
        if (node == curr) 
            check = 1;
        
        next = XOR(prev, curr->npx);
 
        if (next == NULL) {
            next = tail;
            curr = NULL;
        }

        prev = curr;


        curr = next;
      
    }

    if (node == queue->rear) {
        check = 1;
    }

        return check;
}


Node* addOnBeg(Node** head, int data, Node** tail, Queue** queue) {
    Node* newBeg = new Node;
    Queue* tempQueue = *queue;
    newBeg->data = data;
    newBeg->npx = *head;
    if (*head == NULL) {
        *tail = newBeg;
    }

    if (*head != NULL) {
        (*head)->npx = XOR(newBeg, (*head)->npx);
    }
   
    *head = newBeg;


    if (isInQueue(tempQueue, newBeg, *tail))
        tempQueue->size += 1;

    return newBeg;
}


void addOnEnd(Node** head, int data, Node** tail, Queue** queue) {
    Node* curr = *head;
    Node* last = *tail;
    Queue* tempQueue = *queue;

    if (curr == NULL) {
        addOnBeg(head, data, tail,queue);
    }
    else {

        Node* end = new Node;
        end->data = data;
        last->npx = XOR(XOR(NULL, last->npx),end);
        end->npx = XOR(last, NULL);
        *tail = end;
        if ((*queue)->rear == last)
            (*queue)->preRear = end;
        if ((*queue)->front == last)
            (*queue)->preFront = end;


        if (isInQueue(tempQueue, end, *tail))
            tempQueue->size += 1;

    }
    
}



void printList(Node* head, Node* end) {
    Node* curr = head;
    Node* prev = NULL;
    Node* next;
    if (curr == NULL) {
        cout << "NULL";
    }

    while (curr != end)
    {
        cout << curr->data << " ";

        next = XOR(prev, curr->npx);

        prev = curr;
        curr = next;
    }
}


Node* delBeg(Node** tail, Node** head, Queue** queue) {
    Node* curr = *head;
    Queue* tempQueue = *queue;

    if (*head == NULL) {
    }
    else {


        Node* temp = *head;

        if (isInQueue(tempQueue, temp, *tail))
            tempQueue->size -= 1;

        if (tempQueue->front != tempQueue->rear) {
            if (tempQueue->front == temp) {
                tempQueue->front = *tail;
                tempQueue->preFront = NULL;
                tempQueue->size -= 1;
            }
            if (tempQueue->rear == temp) {
                Node* temp1 = tempQueue->rear;
                tempQueue->rear = tempQueue->preRear;
                tempQueue->preRear = XOR(temp1, tempQueue->preRear->npx);
                tempQueue->size -= 1;
            }
        }
        else {
            tempQueue->rear = tempQueue->front = tempQueue->preRear = tempQueue->preFront = NULL;
            tempQueue->size = 0;
        }
        *head = XOR(NULL, temp->npx);
        if (*head != NULL) {
            (*head)->npx = XOR(NULL, XOR(temp, (*head)->npx));
        }



        if (XOR(NULL, curr->npx) == NULL)
            *tail = NULL;

        delete temp;
    }
    return *head;
}




Node* delEnd( Node** head, Node** tail, Queue** queue)
{
    if (*head == NULL) {
    }
    else {
        Node* last = *tail;
        Node* prev = XOR(NULL,last->npx);
        Queue* tempQueue = *queue;

        if (isInQueue(tempQueue, last, *tail))
            tempQueue->size -= 1;

        if (tempQueue->rear != tempQueue->front) {
            if (tempQueue->rear == last) {
                tempQueue->rear = *head;
                tempQueue->preRear = XOR(NULL, (*head)->npx);
                tempQueue->size -= 1;
            }
            if (tempQueue->front == last) {
                tempQueue->front = XOR(tempQueue->preFront, tempQueue->front->npx);
                tempQueue->preFront = NULL;
                tempQueue->size -= 1;
            }

            if (tempQueue->preFront == last)
                tempQueue->preFront = NULL;
            if (tempQueue->preRear == last)
                tempQueue->preRear = NULL;

        }
        else  {
            tempQueue->rear = tempQueue->front = tempQueue->preRear = tempQueue->preFront = NULL;
            tempQueue->size = 0;
        }


        if (prev != NULL) {
            prev->npx = XOR(XOR(last, prev->npx), NULL);
            *tail = prev;
        }
        else {
            *head = NULL;
            *tail = NULL;
        }

        delete last;
    }

    return *head;
}



void insert(Queue* queue, int data) {
    Queue* tempQueue = queue;
    Node* newNode = new Node;

    Node* next = XOR(tempQueue->preRear, tempQueue->rear->npx);
    newNode->data = data;
    next->npx = XOR(newNode, XOR(tempQueue->rear, next->npx));
    tempQueue->rear->npx = XOR(queue->preRear, newNode);

    newNode->npx = XOR(tempQueue->rear, next);
    tempQueue->preRear = tempQueue->rear;
    tempQueue->rear = newNode;
    tempQueue->preFront = newNode;
}




void enQueue(int x, Queue** queue, Node** head, Node** tail)
{
    Queue* tempQueue = *queue;
    Node* curr = *head;
    Node* last = *tail;
    Node* temp;
    
     if (curr == NULL) {
        tempQueue->front = tempQueue->rear = addOnBeg(head, x, tail,queue);;
        (*queue)->size += 1;
    }else if(curr != NULL && tempQueue->front == NULL){
        tempQueue->front = tempQueue->rear = last;
        last->data = x;
        (*queue)->size += 1;
    }
    else if (tempQueue->front != NULL) {
         if (findSize(curr, NULL)== (*queue)->size &&tempQueue->rear == curr) {
             tempQueue->preRear = *head;
             tempQueue->rear = addOnBeg(head, x, tail,queue);
             (*queue)->size += 1;
         }
         else if (findSize(*head, NULL) == (*queue)->size && tempQueue->rear != curr) {
             insert(*queue, x);
             (*queue)->size += 1;
         }
         else if (tempQueue->rear == curr) {
             last->data = x;
             tempQueue->rear = last;
             tempQueue->preRear = NULL;
             (*queue)->size += 1;
 
         }else{
             temp = tempQueue->rear;
             XOR(tempQueue->preRear, tempQueue->rear->npx)->data = x;
             tempQueue->rear = XOR(tempQueue->preRear, tempQueue->rear->npx);
             tempQueue->preRear = temp;
             (*queue)->size += 1;
         }
    }

}



void deQueue(Queue** queue, Node** head, Node** tail) {
    Queue* tempQueue = *queue;
    Node* temp;
    if (head == NULL) {
        tempQueue->front = tempQueue->rear = tempQueue->preFront = tempQueue->preRear = NULL;
        cout << "NULL"<< endl;
        tempQueue->size = 0;
    }
    else if (tempQueue->front == tempQueue->rear) {
        printNode(tempQueue->front);
        tempQueue->front = tempQueue->rear = tempQueue->preFront = tempQueue->preRear = NULL;
        tempQueue->size = 0;

    }
    else if (tempQueue->front == *head) {
        printNode(tempQueue->front);
        tempQueue->front = *tail;
        tempQueue->preFront = NULL;
        tempQueue->size -= 1;
    }
    else {
        temp = tempQueue->front;
        printNode(tempQueue->front);
        tempQueue->front = XOR(tempQueue->preFront, tempQueue->front->npx);
        tempQueue->preFront = temp;
        tempQueue->size -= 1;
        
    }


}


void printQueue(Queue* queue, Node* tail) {
     if (queue->front == NULL)
        cout << "NULL";
    else if (queue->front == queue->rear)
        cout << queue->front->data;
   
    else {
        Node* curr = queue->front;
        Node* prev = queue->preFront;
        Node* next;
        if (curr == NULL) {
            cout << "NULL";
        }

        while (curr != queue->rear)
        {
            cout << curr->data << " ";
            next = XOR(prev, curr->npx);
            if (next == NULL){
                next = tail;
                curr = NULL;
                }
            prev = curr;
          
            curr = next;
        }
        cout << queue->rear->data;
    }
}



void garbageSoft(Queue** queue, Node** tail, Node** head) {
    Node* curr = *head;
    Node* prev = NULL;
    Node* next;
    if (curr == NULL) {

    }

    while (curr != NULL)
    {

        if (!isInQueue(*queue, curr, *tail))
            curr->data = 0;

        next = XOR(prev, curr->npx);

        prev = curr;
        curr = next;
    }
}


void garbageHard(Queue** queue, Node** tail, Node** head) {
    Node* curr = *head;
    Node* prev = NULL;
    Node* next;
    Node* tempCurr;
    Node* tempLast = NULL;
    if (curr == NULL) {

    }
    else
    {

        while (curr != NULL)
        {
       
            next = XOR(prev, curr->npx);
            if (!isInQueue(*queue, curr, *tail)) {
                if (curr == *head)
                    delBeg(tail, head, queue);
                else if (curr == *tail)
                    delEnd(head, tail, queue);
                else{ 
                    Node* tempPrev = XOR(curr,prev->npx);
                    tempCurr = prev;
                    while (!isInQueue(*queue, next, *tail)) {
                       Node* temp = curr;
                        next = XOR(prev, curr->npx);
                        tempLast = XOR(curr, next->npx);
                        prev = curr;
                        curr = next;
                        delete temp;
                    }
                    (*queue)->preFront = next;
                    curr = tempCurr;
                    tempCurr->npx = XOR(next, tempPrev);
                    next->npx = XOR(curr, tempLast);   
                }

            }
            prev = curr;

            curr = next;

        }
    }
}




void deleteList(Node* head, Node* end) {
    Node* curr = head;
    Node* prev = NULL;
    Node* next;
   
    if (curr == NULL) {
    }

    while (curr != end)
    {
        Node* temp = curr;
        next = XOR(prev, curr->npx);

        prev = curr;
        curr = next;
        delete temp;
    }
}


int main()
{
    string option;
    Actions action;
    int tmp = 0;

    Node* xorLinkedList = NULL;
    Node* tail = xorLinkedList;
    Queue* queue = new Queue;


    while (cin >> option) {
        action = stringToEnum(option);

        switch (action) {
        case ADD_BEG:
            cin >> tmp;
            addOnBeg(&xorLinkedList, tmp, &tail,&queue);

            break;
        case ADD_END:
            cin >> tmp;
            addOnEnd(&xorLinkedList, tmp, &tail, &queue);
            break;
        case DEL_BEG:
            delBeg(&tail, &xorLinkedList, &queue);

            break;
        case DEL_END:
            delEnd( &xorLinkedList, &tail,&queue);
            break;
        case PRINT_FORWARD:
            printList(xorLinkedList, NULL);
            cout << endl;
            break;
        case PRINT_BACKWARD:
            printList(tail,NULL);
            cout << endl;
            break;
        case SIZE:
           cout << findSize(xorLinkedList,NULL) << endl;
            break;
        case PUSH:
            cin >> tmp;
            enQueue(tmp, &queue, &xorLinkedList, &tail);
            break;
        case POP:
            deQueue(&queue, &xorLinkedList, &tail);
            break;
        case PRINT_QUEUE:
            printQueue(queue,tail);
            cout<<endl;
            break;
        case COUNT:
           cout<< queue->size << endl;
            break;
        case GARBAGE_SOFT:
            garbageSoft(&queue, &tail, &xorLinkedList);
            break;
        case GARBAGE_HARD:
            garbageHard(&queue, &tail, &xorLinkedList);
            break;   
        case END:
            break;
        case BORDER:
            break;

        }
    }


    deleteList(xorLinkedList, tail);
    return 0;
}
