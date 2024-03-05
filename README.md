# ИДЗ 4

## 1. Модель параллельных вычислений

Имеем класс Programmer, который содержит в себе два метода: для отправки “решения” на проверку и собственно метод для проверки чужих “решений”.

Фактически “решения” нет, методы проверки и отправки выдают рандомные результаты верности решения.

```cpp
class Programmer {
public:
    Programmer() {

    }
    static bool check() {
        if (uniformIntDistribution1(defaultRandomEngine) != 1)
            return false;
        return true;
    }
    static void send(Programmer programmer, int t1, int t2) {
        while (!Programmer::check()) {
            //Вывод строки. Mutex нужен, чтобы выводы из разных потоков не перемешивались.
            pthread_mutex_lock(&mutex);
            std::cout << "Programmer" << t1 << " to Programmer" << t2 << " : wrong!\n";
            pthread_mutex_unlock(&mutex);
        }
        pthread_mutex_lock(&mutex);
        std::cout << "Programmer" << t1 << " to Programmer" << t2 << " : correct!\n";
        pthread_mutex_unlock(&mutex);

    }
};
```

Далее для каждого “программиста” запускается отдельный поток, в котором он генерирует “решение” и отправляет его на проверку другому программисту до тех пор, пока ему не придет ответ, что решение корректно.

## 2. Вывод программы

Условием задачи не предполагался какой-либо вывод. Поэтому я реализовал вывод в консоль результаты работы программистов: какой программист, кому отправил на проверку, и что получил в ответ, например:

```cpp
Programmer1 to Programmer2 : correct!
```

Для корректного вывода используется блок при помощи Mutex. Блокировка включается до вывода, и разблокируется после вывода всей строки.

## 3. Виды ввода

Условием задачи не предполагается ввод, но для более высокой оценки необходимо реализовать ввод из консоли, файла и рандомную генерацию. Поэтому пользователю можно выбрать, сколько “работ” будет делать каждый программист. Выобор ввода предлагается пользователю в самом начале, и осуществляется отдельным методом getNum.

```cpp
int getNum(int inputType) {
    int num1;
    switch (inputType) {
        case 1: {
            std::cout << "Enter number of works\n";
            std::cin >> num1;
            break;
        }
        case 2: {
            std::string fileName;
            std::cout << "Enter file name\n";
            std::cin >> fileName;
            std::ifstream stream(fileName);
            if (stream.is_open()) {
                stream >> num1;
            } else {
                std::cout << "Something is wrong with file, use console, to enter number of works\n";
                std::cin >> num1;
            }
            break;
        }
        case 3: {
            num1 = uniformIntDistribution2(defaultRandomEngine);
            std::cout << "Random num is " << num1 << "\n";
            break;
        }
        default : {
            std::cout << "Wrong command, enter 1, 2 or 3\n";
            std::cin >> inputType;
            num1 = getNum(inputType);
        }
    }
    return num1;
}
```

### Рандомная генерация:

В случае с проверкой корректности решения, шанс ответа “корректно” составляет 1/3

В случае количества решений, которые будет писать каждый программист, рандом будет иметь значение от 1 до 10.

## 4. Примеры входных и выходных праметров

1. Ввод 2 (ввод с файла)
2. В файле только 3 (кол-во работ, сделанных каждым программистом)

Вывод:

```cpp
Programmer1 to Programmer2 : wrong!
Programmer1 to Programmer2 : correct!
Programmer3 to Programmer1 : correct!
Programmer2 to Programmer3 : wrong!
Programmer2 to Programmer3 : wrong!
Programmer2 to Programmer3 : correct!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : correct!
Programmer1 to Programmer2 : wrong!
Programmer1 to Programmer2 : correct!
Programmer2 to Programmer3 : wrong!
Programmer2 to Programmer3 : correct!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : wrong!
Programmer3 to Programmer1 : correct!
Programmer1 to Programmer2 : correct!
Programmer2 to Programmer3 : wrong!
Programmer2 to Programmer3 : wrong!
Programmer2 to Programmer3 : wrong!
Programmer2 to Programmer3 : correct!
```

Таким образом можно увидеть взаимодействие программистов друг с другом. Каждый программист добивается корректной сдачи решения.

## 5. Дополнение

Если понадобится, то файлы с кодом, а также тестовый файл, содержащий число 3 прикладываю в гит.
