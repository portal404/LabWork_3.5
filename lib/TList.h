#include <iostream>
#include <fstream>
#include <cstddef>
using namespace std;

template <typename T>
struct TListNode
{
  T data;
  TListNode<T> *next;
  TListNode<T> *prev;

  TListNode(const T &value);
};

template<typename T>
class TList
{
protected:
  size_t size;
  TListNode<T>* head;
  TListNode<T>* tail;
public:
  TList();
  TList(std::initializer_list<T> init);
  TList(const TList& oth);
  TList(TList&& oth);
  ~TList();

  size_t GetSize() const;
  TListNode<T>* GetHead() const;
  TListNode<T>* GetTail() const;

  bool IsEmpty() const;
  bool IsFull() const;
  void PushFront(const T& value);
  void PushBack(const T& value);
  T PopFront();
  T PopBack();
  void InsertAfter(TListNode<T>* node, const T& value);
  void Remove(TListNode<T>* node);
  void Clear();

  TList& operator=(const TList& oth);
  TList& operator=(TList&& oth);
  bool operator==(const TList& oth) const;
  bool operator!=(const TList& oth) const;
  T& operator[](size_t index);
  const T& operator[](size_t index) const;
  TList operator+(const TList& oth) const;

  size_t Count(const T& value) const;
  TList<TListNode<T>*> FindAll(const T& value) const;
  void SaveToFile(const std::string& filename) const;
  void LoadFromFile(const std::string& filename);

  TListNode<T>* begin();
  TListNode<T>* end();
  const TListNode<T>* begin() const;
  const TListNode<T>* end() const;


  template<class I>
  friend std::ostream& operator<<(std::ostream& os, const TList<T>& list);

  template<class O>
  friend std::istream& operator>>(std::istream& is, TList<T>& list);
};

template<class T>
TListNode<T>::TListNode(const T &value) : data(value), next(nullptr), prev(nullptr)
{
}

template<class T>
TList<T>::TList() : size(0), head(nullptr), tail(nullptr)
{
}

template<class T>
TList<T>::TList(std::initializer_list<T> init) : size(0), head(nullptr), tail(nullptr)
{
  for (const auto &value : init)
  {
    PushBack(value);
  }
}

template<class T>
TList<T>::TList(const TList& oth) : size(0), head(nullptr), tail(nullptr)
{
  TListNode<T>* current = oth.head;
  while (current)
  {
    PushBack(current->data);
    current = current->next;
  }
}

template<class T>
TList<T>::TList(TList&& oth) : size(oth.size), head(oth.head), tail(oth.tail)
{
  oth.size = 0;
  oth.head = nullptr;
  oth.tail = nullptr;
}

template<class T>
TList<T>::~TList()
{
  Clear();
}

template<class T>
size_t TList<T>::GetSize() const
{
  return size;
}

template<class T>
TListNode<T>* TList<T>::GetHead() const
{
  return head;
}

template<class T>
TListNode<T>* TList<T>::GetTail() const
{
  return tail;
}

template<class T>
bool TList<T>::IsEmpty() const
{
  return size == 0;
}

template<class T>
bool TList<T>::IsFull() const
{
  return false;
}

template<class T>
void TList<T>::PushFront(const T& value)
{
  TListNode<T>* newNode = new TListNode<T>(value);
  if (!head)
  {
    head = tail = newNode;
  }
  else
  {
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
  }
  size++;
}

template<class T>
void TList<T>::PushBack(const T& value)
{
  TListNode<T>* newNode = new TListNode<T>(value);
  if (!tail)
  {
    head = tail = newNode;
  }
  else
  {
    newNode->prev = tail;
    tail->next = newNode;
    tail = newNode;
  }
  size++;
}

template<class T>
T TList<T>::PopFront()
{
  if (IsEmpty())
  {
    throw std::runtime_error("List is empty");
  }

  TListNode<T>* temp = head;
  T value = head->data;
  head = head->next;

  if (head)
  {
    head->prev = nullptr;
  }
  else
  {
    tail = nullptr;
  }

  delete temp;
  size--;
  return value;
}

template<class T>
T TList<T>::PopBack()
{
  if (IsEmpty())
  {
    throw std::runtime_error("List is empty");
  }

  TListNode<T>* temp = tail;
  T value = tail->data;
  tail = tail->prev;

  if (tail)
  {
    tail->next = nullptr;
  }
  else
  {
    head = nullptr;
  }

  delete temp;
  size--;
  return value;
}

template<class T>
void TList<T>::InsertAfter(TListNode<T>* node, const T& value)
{
  if (!node)
  {
    return;
  }

  TListNode<T>* newNode = new TListNode<T>(value);
  newNode->prev = node;
  newNode->next = node->next;

  if (node->next)
  {
    node->next->prev = newNode;
  }
  else
  {
    tail = newNode;
  }

  node->next = newNode;
  size++;
}

template<class T>
void TList<T>::Remove(TListNode<T>* node)
{
  if (!node)
  {
    return;
  }

  if (node->prev)
  {
    node->prev->next = node->next;
  }
  else
  {
    head = node->next;
  }

  if (node->next)
  {
    node->next->prev = node->prev;
  }
  else
  {
    tail = node->prev;
  }

  delete node;
  size--;
}

template<class T>
void TList<T>::Clear()
{
  while (!IsEmpty())
  {
    PopFront();
  }
}

template<class T>
TList<T>& TList<T>::operator=(const TList& oth)
{
  if (this != &oth)
  {
    Clear();
    TListNode<T>* current = oth.head;
    while (current)
    {
      PushBack(current->data);
      current = current->next;
    }
  }
  return *this;
}

template<class T>
TList<T>& TList<T>::operator=(TList&& oth)
{
  if (this != &oth)
  {
    Clear();
    size = oth.size;
    head = oth.head;
    tail = oth.tail;

    oth.size = 0;
    oth.head = nullptr;
    oth.tail = nullptr;
  }
  return *this;
}

template<class T>
bool TList<T>::operator==(const TList& oth) const
{
  if (size != oth.size)
  {
    return false;
  }

  TListNode<T>* cur1 = head;
  TListNode<T>* cur2 = oth.head;
  while (cur1 && cur2)
  {
    if (cur1->data != cur2->data)
    {
      return false;
    }
    cur1 = cur1->next;
    cur2 = cur2->next;
  }
  return true;
}

template<class T>
bool TList<T>::operator!=(const TList& oth) const
{
  return !(*this == oth);
}

template<class T>
T& TList<T>::operator[](size_t index)
{
  if (index >= size)
  {
    throw std::out_of_range("Index out of range");
  }

  TListNode<T>* current = head;
  for (size_t i = 0; i < index; i++)
  {
    current = current->next;
  }
  return current->data;
}

template<class T>
const T& TList<T>::operator[](size_t index) const
{
  if (index >= size)
  {
    throw std::out_of_range("Index out of range");
  }

  TListNode<T>* current = head;
  for (size_t i = 0; i < index; i++)
  {
    current = current->next;
  }
  return current->data;
}

template<class T>
TList<T> TList<T>::operator+(const TList& oth) const
{
  TList<T> result = *this;
  TListNode<T>* current = oth.head;
  while (current)
  {
    result.PushBack(current->data);
    current = current->next;
  }
  return result;
}

template<class T>
size_t TList<T>::Count(const T& value) const
{
  size_t count = 0;
  TListNode<T>* current = head;
  while (current)
  {
    if (current->data == value)
    {
      count++;
    }
    current = current->next;
  }
  return count;
}

template<class T>
TList<TListNode<T>*> TList<T>::FindAll(const T& value) const
{
  TList<TListNode<T>*> result;
  TListNode<T>* current = head;
  while (current)
  {
    if (current->data == value)
    {
      result.PushBack(current);
    }
    current = current->next;
  }
  return result;
}

template<class T>
void TList<T>::SaveToFile(const std::string& filename) const
{
  std::ofstream file(filename, std::ios::binary);
  if (!file)
  {
    throw std::runtime_error("Cannot open file");
  }

  TListNode<T>* current = head;
  while (current)
  {
    file.write(reinterpret_cast<const char*>(&current->data), sizeof(T));
    current = current->next;
  }
}

template<class T>
void TList<T>::LoadFromFile(const std::string& filename)
{
  Clear();
  std::ifstream file(filename, std::ios::binary);
  if (!file)
  {
    throw std::runtime_error("Cannot open file");
  }

  T value;
  while (file.read(reinterpret_cast<char*>(&value), sizeof(T)))
  {
    PushBack(value);
  }
}

template<class T>
TListNode<T>* TList<T>::begin()
{
  return head;
}

template<class T>
TListNode<T>* TList<T>::end()
{
  return nullptr;
}

template<class T>
const TListNode<T>* TList<T>::begin() const
{
  return head;
}

template<class T>
const TListNode<T>* TList<T>::end() const
{
  return nullptr;
}

template<typename I>
std::ostream& operator<<(std::ostream& os, const TList<I>& list)
{
  const TListNode<I>* current = list.head;
  while (current)
  {
    os << current->data;
    if (current->next)
    {
      os << " ";
    }
    current = current->next;
  }
  return os;
}

template<typename O>
std::istream& operator>>(std::istream& is, TList<O>& list)
{
  list.Clear();
  O value;
  while (is >> value)
  {
    list.PushBack(value);
  }
  return is;
}