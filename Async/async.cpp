#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <numeric>
#include <cmath>

// 使用promise传递复杂结果
void calculateSum(std::promise<int>&& prom, std::vector<int> data) {
    std::cout << "Calculate sum started..." << std::endl;
    int sum = std::accumulate(data.begin(), data.end(), 0);
    prom.set_value(sum);
    std::cout << "Sum calculated: " << sum << std::endl;
}

// 使用async包装的异步任务
std::future<int> asyncMultiply(int a, int b) {
    return std::async([=] {
        std::cout << "Multiplication started..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return a * b;
        });
}

int main() {
    // Part 1: promise/future传递数据
    std::promise<int> sumPromise;
    auto sumFuture = sumPromise.get_future();
    std::thread worker(calculateSum, std::move(sumPromise),
        std::vector<int>{1, 2, 3, 4, 5});

    // Part 2: 同时执行多个async任务
    auto fut1 = asyncMultiply(6, 7);
    auto fut2 = asyncMultiply(8, 9);

    // 等待所有结果
    std::cout << "Sum result: " << sumFuture.get() << std::endl;
    std::cout << "Multiply results: " << fut1.get() << ", " << fut2.get() << std::endl;

    worker.join();
    return 0;
}
