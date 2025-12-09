#include <gtest.h>
#include <fstream>
#include <sstream>
#include "TList.h"

TEST(TListTest, DefaultConstructor)
{
  TList<int> list;
  EXPECT_EQ(list.GetSize(), 0);
  EXPECT_EQ(list.GetHead(), nullptr);
  EXPECT_EQ(list.GetTail(), nullptr);
  EXPECT_TRUE(list.IsEmpty());
}

TEST(TListTest, InitializerListConstructor)
{
  TList<int> list{1, 2, 3, 4, 5};
  EXPECT_EQ(list.GetSize(), 5);
  EXPECT_FALSE(list.IsEmpty());

  EXPECT_EQ(list[0], 1);
  EXPECT_EQ(list[1], 2);
  EXPECT_EQ(list[4], 5);
}

TEST(TListTest, CopyConstructor)
{
  TList<int> original{1, 2, 3};
  TList<int> copy(original);

  EXPECT_EQ(original.GetSize(), copy.GetSize());
  EXPECT_EQ(original[0], copy[0]);
  EXPECT_EQ(original[1], copy[1]);
  EXPECT_EQ(original[2], copy[2]);

// Проверяем, что это глубокая копия
  original.PushBack(4);
  EXPECT_EQ(original.GetSize(), 4);
  EXPECT_EQ(copy.GetSize(), 3);
}

TEST(TListTest, MoveConstructor)
{
  TList<int> original{1, 2, 3};
  TList<int> moved(std::move(original));

  EXPECT_EQ(moved.GetSize(), 3);
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
  EXPECT_EQ(moved[2], 3);

// Проверяем, что оригинал пуст
  EXPECT_EQ(original.GetSize(), 0);
  EXPECT_TRUE(original.IsEmpty());
  EXPECT_EQ(original.GetHead(), nullptr);
  EXPECT_EQ(original.GetTail(), nullptr);
}

TEST(TListTest, DestructorAndClear)
{
  TList<int> *list = new TList<int>{1, 2, 3, 4, 5};
  EXPECT_EQ(list->GetSize(), 5);

  list->Clear();
  EXPECT_EQ(list->GetSize(), 0);
  EXPECT_TRUE(list->IsEmpty());

  delete list;
}

TEST(TListTest, PushFront)
{
  TList<int> list;
  list.PushFront(3);
  list.PushFront(2);
  list.PushFront(1);

  EXPECT_EQ(list.GetSize(), 3);
  EXPECT_EQ(list[0], 1);
  EXPECT_EQ(list[1], 2);
  EXPECT_EQ(list[2], 3);
  EXPECT_EQ(list.GetHead()->data, 1);
  EXPECT_EQ(list.GetTail()->data, 3);
}

TEST(TListTest, PushBack)
{
  TList<int> list;
  list.PushBack(1);
  list.PushBack(2);
  list.PushBack(3);

  EXPECT_EQ(list.GetSize(), 3);
  EXPECT_EQ(list[0], 1);
  EXPECT_EQ(list[1], 2);
  EXPECT_EQ(list[2], 3);
  EXPECT_EQ(list.GetHead()->data, 1);
  EXPECT_EQ(list.GetTail()->data, 3);
}

TEST(TListTest, PopFront)
{
  TList<int> list{1, 2, 3};

  EXPECT_EQ(list.PopFront(), 1);
  EXPECT_EQ(list.GetSize(), 2);
  EXPECT_EQ(list[0], 2);
  EXPECT_EQ(list[1], 3);

  EXPECT_EQ(list.PopFront(), 2);
  EXPECT_EQ(list.PopFront(), 3);
  EXPECT_EQ(list.GetSize(), 0);
  EXPECT_TRUE(list.IsEmpty());

// Проверяем исключение при пустом списке (если реализовано)
// ASSERT_THROW(list.PopFront(), std::out_of_range);
}

TEST(TListTest, PopBack)
{
  TList<int> list{1, 2, 3};

  EXPECT_EQ(list.PopBack(), 3);
  EXPECT_EQ(list.GetSize(), 2);
  EXPECT_EQ(list[0], 1);
  EXPECT_EQ(list[1], 2);

  EXPECT_EQ(list.PopBack(), 2);
  EXPECT_EQ(list.PopBack(), 1);
  EXPECT_EQ(list.GetSize(), 0);
  EXPECT_TRUE(list.IsEmpty());

// Проверяем исключение при пустом списке (если реализовано)
// ASSERT_THROW(list.PopBack(), std::out_of_range);
}

TEST(TListTest, InsertAfter)
{
  TList<int> list{1, 3, 5};

// Вставляем после головы
  list.InsertAfter(list.GetHead(), 2);
  EXPECT_EQ(list.GetSize(), 4);
  EXPECT_EQ(list[0], 1);
  EXPECT_EQ(list[1], 2);
  EXPECT_EQ(list[2], 3);
  EXPECT_EQ(list[3], 5);

// Вставляем после хвоста
  list.InsertAfter(list.GetTail(), 6);
  EXPECT_EQ(list.GetSize(), 5);
  EXPECT_EQ(list[4], 6);
  EXPECT_EQ(list.GetTail()->data, 6);

// Вставляем в середину
  list.InsertAfter(list.GetHead()->next->next, 4);
  EXPECT_EQ(list[3], 4);
}

TEST(TListTest, Remove)
{
  TList<int> list{1, 2, 3, 4, 5};

// Удаляем голову
  list.Remove(list.GetHead());
  EXPECT_EQ(list.GetSize(), 4);
  EXPECT_EQ(list[0], 2);

// Удаляем хвост
  list.Remove(list.GetTail());
  EXPECT_EQ(list.GetSize(), 3);
  EXPECT_EQ(list.GetTail()->data, 4);

// Удаляем из середины
  list.Remove(list.GetHead()->next); // удаляем 3
  EXPECT_EQ(list.GetSize(), 2);
  EXPECT_EQ(list[0], 2);
  EXPECT_EQ(list[1], 4);
}

TEST(TListTest, CopyAssignment)
{
  TList<int> list1{1, 2, 3};
  TList<int> list2{4, 5};

  list2 = list1;

  EXPECT_EQ(list1.GetSize(), list2.GetSize());
  EXPECT_EQ(list1[0], list2[0]);
  EXPECT_EQ(list1[1], list2[1]);
  EXPECT_EQ(list1[2], list2[2]);

// Проверяем глубокое копирование
  list1.PushBack(10);
  list2.PushBack(20);
  EXPECT_EQ(list1.GetSize(), 4);
  EXPECT_EQ(list2.GetSize(), 4);
  EXPECT_EQ(list1[3], 10);
  EXPECT_EQ(list2[3], 20);
}

TEST(TListTest, MoveAssignment)
{
  TList<int> list1{1, 2, 3};
  TList<int> list2;

  list2 = std::move(list1);

  EXPECT_EQ(list2.GetSize(), 3);
  EXPECT_EQ(list2[0], 1);
  EXPECT_EQ(list2[1], 2);
  EXPECT_EQ(list2[2], 3);

// Проверяем, что оригинал пуст
  EXPECT_EQ(list1.GetSize(), 0);
  EXPECT_TRUE(list1.IsEmpty());
}

TEST(TListTest, EqualityOperator)
{
  TList<int> list1{1, 2, 3};
  TList<int> list2{1, 2, 3};
  TList<int> list3{1, 2};
  TList<int> list4{1, 2, 4};

  EXPECT_TRUE(list1 == list2);
  EXPECT_FALSE(list1 == list3);
  EXPECT_FALSE(list1 == list4);
}

TEST(TListTest, InequalityOperator)
{
  TList<int> list1{1, 2, 3};
  TList<int> list2{1, 2, 3};
  TList<int> list3{1, 2, 4};

  EXPECT_FALSE(list1 != list2);
  EXPECT_TRUE(list1 != list3);
}

TEST(TListTest, IndexOperator)
{
  TList<int> list{10, 20, 30, 40};

  EXPECT_EQ(list[0], 10);
  EXPECT_EQ(list[1], 20);
  EXPECT_EQ(list[2], 30);
  EXPECT_EQ(list[3], 40);

// Проверяем модификацию
  list[1] = 25;
  EXPECT_EQ(list[1], 25);

// Проверяем константный доступ
  const TList<int> &constList = list;
  EXPECT_EQ(constList[0], 10);
  EXPECT_EQ(constList[1], 25);

// Проверяем выход за границы (если реализовано)
// ASSERT_THROW(list[10], std::out_of_range);
}

TEST(TListTest, ConcatenationOperator)
{
  TList<int> list1{1, 2, 3};
  TList<int> list2{4, 5, 6};

  TList<int> result = list1 + list2;

  EXPECT_EQ(result.GetSize(), 6);
  EXPECT_EQ(result[0], 1);
  EXPECT_EQ(result[1], 2);
  EXPECT_EQ(result[2], 3);
  EXPECT_EQ(result[3], 4);
  EXPECT_EQ(result[4], 5);
  EXPECT_EQ(result[5], 6);

// Проверяем, что исходные списки не изменились
  EXPECT_EQ(list1.GetSize(), 3);
  EXPECT_EQ(list2.GetSize(), 3);
}

TEST(TListTest, Count)
{
  TList<int> list{1, 2, 3, 2, 4, 2, 5};

  EXPECT_EQ(list.Count(2), 3);
  EXPECT_EQ(list.Count(1), 1);
  EXPECT_EQ(list.Count(6), 0);

  TList<std::string> strList{"apple", "banana", "apple", "cherry"};
  EXPECT_EQ(strList.Count("apple"), 2);
}

TEST(TListTest, FileOperations)
{
// Подготовка тестового файла
  const std::string testFile = "test_list.dat";

// Сохраняем список в файл
  TList<int> original{1, 2, 3, 4, 5};
  original.SaveToFile(testFile);

// Загружаем из файла
  TList<int> loaded;
  loaded.LoadFromFile(testFile);

// Проверяем, что списки одинаковы
  EXPECT_EQ(original.GetSize(), loaded.GetSize());
  for (size_t i = 0; i < original.GetSize(); ++i)
  {
    EXPECT_EQ(original[i], loaded[i]);
  }

// Очищаем тестовый файл
  std::remove(testFile.c_str());
}

TEST(TListTest, StreamOperators)
{
// Тест оператора ввода
  TList<int> newList;
  std::istringstream iss("10 20 30 40");
  iss >> newList;

// Проверяем, что список заполнился
  EXPECT_EQ(newList.GetSize(), 4);
  EXPECT_EQ(newList[0], 10);
  EXPECT_EQ(newList[1], 20);
  EXPECT_EQ(newList[2], 30);
  EXPECT_EQ(newList[3], 40);
}

TEST(TListTest, IsFull)
{
  TList<int> list;

// Для связанного списка IsFull обычно всегда возвращает false,
// если только нет специального ограничения на максимальный размер
  EXPECT_FALSE(list.IsFull());

// Заполняем список
  for (int i = 0; i < 1000; ++i)
  {
    list.PushBack(i);
  }

// Проверяем, что список не полон (для динамического списка)
  EXPECT_FALSE(list.IsFull());
}

TEST(TListTest, ComplexOperations)
{
  TList<int> list;

// Комплексный тест: смешиваем различные операции
  list.PushFront(3);
  list.PushBack(5);
  list.PushFront(2);
  list.InsertAfter(list.GetHead(), 4); // после 2 вставляем 4
  list.PushBack(6);
  list.PopFront(); // удаляем 2

// Ожидаемый результат: 4, 3, 5, 6
  EXPECT_EQ(list.GetSize(), 4);
  EXPECT_EQ(list[0], 4);
  EXPECT_EQ(list[1], 3);
  EXPECT_EQ(list[2], 5);
  EXPECT_EQ(list[3], 6);

// Удаляем из середины
  list.Remove(list.GetHead()->next); // удаляем 3
  EXPECT_EQ(list.GetSize(), 3);
  EXPECT_EQ(list[0], 4);
  EXPECT_EQ(list[1], 5);
  EXPECT_EQ(list[2], 6);

// Копируем и сравниваем
  TList<int> copy = list;
  EXPECT_TRUE(copy == list);

// Изменяем копию и проверяем, что оригинал не изменился
  copy.PushBack(10);
  EXPECT_FALSE(copy == list);
  EXPECT_EQ(list.GetSize(), 3);
  EXPECT_EQ(copy.GetSize(), 4);
}

TEST(TListTest, StringType)
{
// Тестируем с другим типом данных
  TList<std::string> strList;

  strList.PushBack("Hello");
  strList.PushBack("World");
  strList.PushBack("!");

  EXPECT_EQ(strList.GetSize(), 3);
  EXPECT_EQ(strList[0], "Hello");
  EXPECT_EQ(strList[1], "World");
  EXPECT_EQ(strList[2], "!");

// Тестируем поиск
  strList.PushBack("Hello");
  EXPECT_EQ(strList.Count("Hello"), 2);
}

TEST(TListTest, SelfAssignment)
{
  TList<int> list{1, 2, 3};

// Тест самоприсваивания
  list = list;

  EXPECT_EQ(list.GetSize(), 3);
  EXPECT_EQ(list[0], 1);
  EXPECT_EQ(list[1], 2);
  EXPECT_EQ(list[2], 3);
}

TEST(TListTest, MoveToSelf)
{
  TList<int> list{1, 2, 3};

// Тест перемещения в самого себя
  list = std::move(list);

// Поведение может отличаться, но хотя бы не должно падать
  EXPECT_GE(list.GetSize(), 0);
}


TEST(TListTest, Iterators)
{
  TList<int> list{1, 2, 3, 4, 5};

// Тестируем обычные итераторы
  int sum = 0;
  int expectedSum = 1 + 2 + 3 + 4 + 5;

  for (auto it = list.begin(); it != list.end(); ++it)
  {
    sum += it->data;
  }
  EXPECT_EQ(sum, expectedSum);

// Тестируем range-based for
  sum = 0;
  for (const auto &node: list)
  {
    sum += node.data;
  }
  EXPECT_EQ(sum, expectedSum);

// Тестируем константные итераторы
  const TList<int> &constList = list;
  sum = 0;
  for (auto it = constList.begin(); it != constList.end(); ++it)
  {
    sum += it->data;
  }
  EXPECT_EQ(sum, expectedSum);
}
