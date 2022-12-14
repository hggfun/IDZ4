#include <iostream>
#include <fstream>
#include <random>

pthread_mutex_t mutex;
std::random_device randomDevice;
std::default_random_engine defaultRandomEngine(randomDevice());
std::uniform_int_distribution<int> uniformIntDistribution1(1, 3);
std::uniform_int_distribution<int> uniformIntDistribution2(1, 10);


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

//Метод для получения числа работ, которые должен написать один программист
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
void *work(void *);

bool checked[] = {false, false, false};

Programmer programmer1 = Programmer();
Programmer programmer2 = Programmer();
Programmer programmer3 = Programmer();

int main() {
    std::string fileName;
    int inputType;
    int num1, t1 = 1, t2 = 2, t3 = 3;
    std::cout << "Enter input type: 1 - console, 2 - file, 3 - random\n";
    std::cin >> inputType;
    num1 = getNum(inputType);
    pthread_t threads[3];
    for (int i = 0; i < num1; i++) {
        pthread_create(&threads[0], nullptr, work, &t1);
        pthread_create(&threads[1], nullptr, work, &t2);
        pthread_create(&threads[2], nullptr, work, &t3);
        for (int y = 0; y < 3; ++y) {
            pthread_join(threads[0], nullptr);
        }
    }
    return 0;
}

void *work(void* t) {
    int order = *((int *) t);
    switch (order) {
        case 1: {
            Programmer::send(programmer2, 1, 2);
            break;
        }
        case 2: {
            Programmer::send(programmer3, 2, 3);
            break;
        }
        case 3: {
            Programmer::send(programmer1, 3, 1);
            break;
        }
    }
}