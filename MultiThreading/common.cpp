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

    current++;  //轮到下一个任务
    cv.notify_all(); //通知所有等待的线程
}

void calculate_square1(int x) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    int result = x * x;

    std::cout << x << "^2 = " << x * x << std::endl;
}

int main() {
    std::vector<std::thread> workers;
    for (int i = 1; i <= 5; ++i) {
        // 创建线程并立即执行（“雇佣厨师并让他开始做菜”）
        workers.emplace_back(calculate_square_bycv, i);
    }

    // 等待所有线程执行完毕（“等待所有厨师做完菜”）
    for (auto& th : workers) {
        th.join();
    }
    return 0;
}