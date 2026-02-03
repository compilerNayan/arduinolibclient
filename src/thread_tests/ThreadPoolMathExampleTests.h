#ifndef THREAD_POOL_MATH_EXAMPLE_TESTS_H
#define THREAD_POOL_MATH_EXAMPLE_TESTS_H

#include "../tests/TestUtils.h"
#include <IThreadPool.h>
#ifdef ARDUINO
#include <ArduinoThreadPool.h>
#else
#include <ThreadPool.h>
#endif

// ============================================================================
// Autowired thread pool (implementation resolved via Implementation<IThreadPool>)
// ============================================================================

/* @Autowired */
IThreadPoolPtr threadPool;

// ============================================================================
// Example: class with a method to run on the thread pool
// ============================================================================

class Math {
public:
    int add(int a, int b) {
        return a + b;
    }
};

// ============================================================================
// Example tests: execute Math::add on the thread pool
// ============================================================================

/**
 * Example: Submit Math::add to the thread pool using a lambda.
 * The pool expects std::function<void()>, so we capture the Math instance
 * and a place to store the result, then call add inside the lambda.
 */
static void TestThreadPool_MathAdd_SingleCall() {
    std_println("\n=== TestThreadPool_MathAdd_SingleCall ===");
    Math math;
    int result = 0;

    threadPool->Submit([&math, &result]() {
        result = math.add(3, 7);
    });
    threadPool->WaitForCompletion(0);

    PrintTestResult("Math::add(3, 7) executed on pool", result == 10);
}

/**
 * Example: Run multiple Math::add calls on the pool and collect results.
 */
static void TestThreadPool_MathAdd_MultipleCalls() {
    std_println("\n=== TestThreadPool_MathAdd_MultipleCalls ===");
    Math math;
    int result1 = 0, result2 = 0, result3 = 0;

    threadPool->Submit([&math, &result1]() { result1 = math.add(1, 2); });
    threadPool->Submit([&math, &result2]() { result2 = math.add(10, 20); });
    threadPool->Submit([&math, &result3]() { result3 = math.add(-5, 5); });
    threadPool->WaitForCompletion(0);

    PrintTestResult("add(1,2)=3", result1 == 3);
    PrintTestResult("add(10,20)=30", result2 == 30);
    PrintTestResult("add(-5,5)=0", result3 == 0);
}

/**
 * Example: Lambda stored in variable, submitted via IThreadPoolPtr.
 */
static void TestThreadPool_MathAdd_WithBind() {
    std_println("\n=== TestThreadPool_MathAdd_WithBind ===");
    Math math;
    int result = 0;

    auto addTask = [&math, &result]() {
        result = math.add(100, 200);
    };
    threadPool->Submit(addTask);
    threadPool->WaitForCompletion(0);

    PrintTestResult("add(100,200)=300 via lambda", result == 300);
}

void RunAllThreadPoolMathExampleTests() {
    std_println("\n========================================");
    std_println("ThreadPool Math Example Tests");
    std_println("========================================");

    TestThreadPool_MathAdd_SingleCall();
    TestThreadPool_MathAdd_MultipleCalls();
    TestThreadPool_MathAdd_WithBind();

    std_println("\n========================================");
    std_println("ThreadPool Math Example Tests Completed");
    std_println("========================================\n");
}

#endif // THREAD_POOL_MATH_EXAMPLE_TESTS_H
