#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

std::mutex cout_mutex;
std::condition_variable cv;
int current = 1;

void calculate_square_bylg(int x) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    int result = x * x;

    std::cout << x << "^2 = " << x * x << std::endl;
}

void calculate_square_bycv(int x) {
    std::unique_lock<std::mutex> lock(cout_mutex);
    cv.wait(lock, [x] {return current == x; });

    int result = x * x;
    std::cout << x << "^2 = " << result << std::endl;

    current++;  //�ֵ���һ������
    cv.notify_all(); //֪ͨ���еȴ����߳�
}

void calculate_square1(int x) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    int result = x * x;

    std::cout << x << "^2 = " << x * x << std::endl;
}

int main() {
    std::vector<std::thread> workers;
    for (int i = 1; i <= 5; ++i) {
        // �����̲߳�����ִ�У�����Ӷ��ʦ��������ʼ���ˡ���
        workers.emplace_back(calculate_square_bycv, i);
    }

    // �ȴ������߳�ִ����ϣ����ȴ����г�ʦ����ˡ���
    for (auto& th : workers) {
        th.join();
    }
    return 0;
}