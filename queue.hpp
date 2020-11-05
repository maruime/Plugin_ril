template <class T>
class Queue
{
  struct Item
  {
    T *data;
    Item *next;
  };
  Item *m_front, *m_back;
  size_t m_count;
public:
  Queue()
  {
    m_front = NULL;
    m_back = NULL;
    m_count = 0;
  }

  ~Queue()
  {
    clear();
  }

  void clear()
  {
    if (!empty()) pop();
  }

  void push(T *a)
  {
    Item *item = new Item;
    item->data = a;
    item->next = NULL;
    if (NULL != m_back) m_back->next = item;
    if (NULL == m_front) m_front = item;
    m_back = item;
    ++m_count;
  }

  void pop()
  {
    Item *next = m_front->next;
    delete m_front->data;
    delete m_front;
    if (m_front == m_back) m_back = NULL;
    m_front = next;
    --m_count;
  }

  T& front()
  {
    return *m_front->data;
  }

  bool empty()
  {
    return (NULL == m_front);
  }

  size_t size()
  {
    return m_count;
  }
};