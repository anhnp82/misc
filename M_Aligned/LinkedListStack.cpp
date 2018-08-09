
struct Node
{
    Node(int v)
    {
        value = v;
        next = NULL;
        prev = NULL;
    }
    
    int value;
    Node* next;
    Ndoe* prev;
}

class Stack
{
    public:
    
    Stack()
    {
        m_Top = NULL;
    }
    
    void push(int v);
    int pop();
    
    protected:
    
    Node* m_Top;
};

void Stack::push(int v)
{
    Node* n = new Node(v);
    n->prev = m_Top;
    m_Top = n;
}


int Stack::pop()
{
    if (m_Top)
    {
        int value = m_Top->value;
        Node* n = m_Top;
        m_Top = m_Top->prev;
        delete n;
        return value;
    }
    else
        throw exception("empty stack");
}