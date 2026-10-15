#include <iomanip>
#include <iostream>

int main() {
  // Журнал оценок. Есть несколько групп, в каждой группе свое число
  // студентов, у каждого студента свое число оценок. Для каждой группы
  // считаем средний балл и находим лучшего студента.
  //
  // int***: journal[g][s][m] это m-я оценка s-го студента из группы g.
  // Размеры разные на каждом уровне, поэтому обычный int[G][S][K] не
  // подходит, нужен массив указателей на массивы указателей на массивы int.
  //
  // void*: чтобы не делать new на каждую группу и каждого студента (и потом
  // не забыть столько же delete[]), выделяем один кусок памяти под весь
  // журнал. В нем лежат данные разных типов: int** (группы), int* (студенты)
  // и int (оценки). Поэтому у самого куска нет конкретного типа, это void*,
  // а нужные части мы потом приводим к int**, int* и int*.
  //
  // Ввод:
  //   G - число групп
  //   G чисел - сколько студентов в каждой группе
  //   для каждого студента по порядку - сколько у него оценок
  //   потом сами оценки студентов в том же порядке

  int groups;
  std::cin >> groups;
  if (!std::cin || groups <= 0) {
    std::cout << "error\n";
    return 0;
  }

  int* group_size = new int[groups];
  int students = 0;
  for (int g = 0; g < groups; ++g) {
    std::cin >> group_size[g];
    students += group_size[g];
  }

  int* marks_count = new int[students + 1];
  int marks = 0;
  for (int i = 0; i < students; ++i) {
    std::cin >> marks_count[i];
    marks += marks_count[i];
  }

  // сначала все указатели, потом все int, чтобы ничего не съехало
  // по выравниванию
  size_t bytes =
      groups * sizeof(int**) + students * sizeof(int*) + marks * sizeof(int);
  void* memory = operator new(bytes);
  char* p = (char*)memory;  // арифметику на void* делать нельзя

  int*** journal = (int***)p;
  p += groups * sizeof(int**);
  for (int g = 0; g < groups; ++g) {
    journal[g] = (int**)p;
    p += group_size[g] * sizeof(int*);
  }

  int idx = 0;
  for (int g = 0; g < groups; ++g) {
    for (int s = 0; s < group_size[g]; ++s) {
      journal[g][s] = (int*)p;
      p += marks_count[idx] * sizeof(int);
      ++idx;
    }
  }

  idx = 0;
  for (int g = 0; g < groups; ++g) {
    for (int s = 0; s < group_size[g]; ++s) {
      for (int m = 0; m < marks_count[idx]; ++m) {
        std::cin >> journal[g][s][m];
      }
      ++idx;
    }
  }

  std::cout << std::fixed << std::setprecision(2);
  idx = 0;
  for (int g = 0; g < groups; ++g) {
    int group_sum = 0;
    int group_marks = 0;
    int best = -1;
    double best_avg = 0;

    for (int s = 0; s < group_size[g]; ++s) {
      int k = marks_count[idx];
      ++idx;
      if (k == 0) {
        continue;
      }
      int sum = 0;
      for (int m = 0; m < k; ++m) {
        sum += journal[g][s][m];
      }
      group_sum += sum;
      group_marks += k;
      double avg = (double)sum / k;
      if (best == -1 || avg > best_avg) {
        best = s;
        best_avg = avg;
      }
    }

    std::cout << "group " << g + 1 << ": ";
    if (group_marks == 0) {
      std::cout << "no marks\n";
    } else {
      std::cout << "average " << (double)group_sum / group_marks
                << ", best student " << best + 1 << " (" << best_avg << ")\n";
    }
  }

  operator delete(memory);
  delete[] group_size;
  delete[] marks_count;
  return 0;
}
