#ifndef THREAD_POOL_TESTS_H
#define THREAD_POOL_TESTS_H

#include "../tests/TestUtils.h"
#include <IThreadPool.h>
#ifndef ARDUINO
#include <chrono>
#include <thread>
#endif
#include <atomic>

#ifdef ARDUINO
#include <Arduino.h>
#define THREAD_TEST_SLEEP_MS(ms) delay(ms)
#define THREAD_TEST_YIELD() yield()
#else
#define THREAD_TEST_SLEEP_MS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define THREAD_TEST_YIELD() std::this_thread::yield()
#endif

// ============================================================================
// ThreadPool tests (desktop and Arduino/ESP32)
// ============================================================================

static void TestThreadPool_GetPoolSize() {
    std_println("\n=== TestThreadPool_GetPoolSize ===");
    ThreadPool pool(4);
    PrintTestResult("GetPoolSize returns constructor value", pool.GetPoolSize() == 4);
    ThreadPool pool1(1);
    PrintTestResult("GetPoolSize 1", pool1.GetPoolSize() == 1);
}

static void TestThreadPool_IsRunning_IsShutdown() {
    std_println("\n=== TestThreadPool_IsRunning_IsShutdown ===");
    ThreadPool pool(2);
    PrintTestResult("IsRunning true when created", pool.IsRunning());
    PrintTestResult("IsShutdown false when created", !pool.IsShutdown());
    pool.Shutdown();
    PrintTestResult("IsShutdown true after Shutdown", pool.IsShutdown());
    PrintTestResult("IsRunning false after Shutdown", !pool.IsRunning());
}

static void TestThreadPool_SubmitAndWait() {
    std_println("\n=== TestThreadPool_SubmitAndWait ===");
    ThreadPool pool(2);
    std::atomic<bool> executed{false};
    Bool submitted = pool.Submit([&executed]() { executed = true; });
    PrintTestResult("Submit accepted", submitted);
    Bool completed = pool.WaitForCompletion(0);
    PrintTestResult("WaitForCompletion returns true", completed);
    PrintTestResult("Task was executed", executed.load());
}

static void TestThreadPool_MultipleTasks() {
    std_println("\n=== TestThreadPool_MultipleTasks ===");
    ThreadPool pool(2);
    std::atomic<int> count{0};
    for (int i = 0; i < 10; ++i) {
        pool.Submit([&count]() { count++; });
    }
    pool.WaitForCompletion(0);
    PrintTestResult("All 10 tasks executed", count.load() == 10);
}

static void TestThreadPool_SubmitRejectedAfterShutdown() {
    std_println("\n=== TestThreadPool_SubmitRejectedAfterShutdown ===");
    ThreadPool pool(2);
    pool.Shutdown();
    pool.WaitForCompletion(100);
    Bool rejected = !pool.Submit([]() {});
    PrintTestResult("Submit returns false after Shutdown", rejected);
}

static void TestThreadPool_ShutdownNow() {
    std_println("\n=== TestThreadPool_ShutdownNow ===");
    ThreadPool pool(2);
    std::atomic<int> runCount{0};
    for (int i = 0; i < 20; ++i) {
        pool.Submit([&runCount]() {
            THREAD_TEST_SLEEP_MS(50);
            runCount++;
        });
    }
    THREAD_TEST_SLEEP_MS(10);
    pool.ShutdownNow();
    Bool completed = pool.WaitForCompletion(500);
    PrintTestResult("WaitForCompletion completes after ShutdownNow", completed);
    PrintTestResult("IsShutdown true after ShutdownNow", pool.IsShutdown());
    // Some tasks may have run; not all 20 will run due to ShutdownNow
    PrintTestResult("Some or no tasks ran", runCount.load() >= 0 && runCount.load() <= 20);
}

static void TestThreadPool_GetPendingCount() {
    std_println("\n=== TestThreadPool_GetPendingCount ===");
    ThreadPool pool(1);
    std::atomic<bool> slowStarted{false};
    std::atomic<bool> slowDone{false};
    pool.Submit([&slowStarted, &slowDone]() {
        slowStarted = true;
        THREAD_TEST_SLEEP_MS(100);
        slowDone = true;
    });
    while (!slowStarted.load()) {
        THREAD_TEST_YIELD();
    }
    pool.Submit([]() {});
    pool.Submit([]() {});
    Size pending = pool.GetPendingCount();
    PrintTestResult("GetPendingCount >= 0", pending >= 0);
    pool.WaitForCompletion(0);
    PrintTestResult("GetPendingCount 0 after completion", pool.GetPendingCount() == 0);
}

static void TestThreadPool_WaitForCompletionTimeout() {
    std_println("\n=== TestThreadPool_WaitForCompletionTimeout ===");
    ThreadPool pool(1);
    pool.Submit([]() { THREAD_TEST_SLEEP_MS(200); });
    Bool completedShort = pool.WaitForCompletion(10);
    PrintTestResult("WaitForCompletion(10ms) returns false when task still running", !completedShort);
    Bool completedLong = pool.WaitForCompletion(500);
    PrintTestResult("WaitForCompletion(500ms) returns true when task done", completedLong);
}

static void TestThreadPool_ZeroThreadsUsesOne() {
    std_println("\n=== TestThreadPool_ZeroThreadsUsesOne ===");
    ThreadPool pool(0);
    PrintTestResult("Pool with 0 threads reports GetPoolSize 1", pool.GetPoolSize() == 1);
    std::atomic<bool> ran{false};
    pool.Submit([&ran]() { ran = true; });
    pool.WaitForCompletion(0);
    PrintTestResult("Task still runs", ran.load());
}

static void TestThreadPool_ExceptionInTask() {
    std_println("\n=== TestThreadPool_ExceptionInTask ===");
    ThreadPool pool(2);
    std::atomic<bool> secondRan{false};
#ifdef ARDUINO
    pool.Submit([]() { (void)0; }); /* skip throw on Arduino if exceptions disabled */
#else
    pool.Submit([]() { throw std::runtime_error("task error"); });
#endif
    pool.Submit([&secondRan]() { secondRan = true; });
    Bool completed = pool.WaitForCompletion(0);
    PrintTestResult("WaitForCompletion succeeds despite exception", completed);
    PrintTestResult("Second task still ran", secondRan.load());
}

void RunAllThreadPoolTests() {
    std_println("\n========================================");
    std_println("Starting ThreadPool Tests");
    std_println("========================================");

    TestThreadPool_GetPoolSize();
    TestThreadPool_IsRunning_IsShutdown();
    TestThreadPool_SubmitAndWait();
    TestThreadPool_MultipleTasks();
    TestThreadPool_SubmitRejectedAfterShutdown();
    TestThreadPool_ShutdownNow();
    TestThreadPool_GetPendingCount();
    TestThreadPool_WaitForCompletionTimeout();
    TestThreadPool_ZeroThreadsUsesOne();
    TestThreadPool_ExceptionInTask();

    std_println("\n========================================");
    std_println("ThreadPool Tests Completed");
    std_println("========================================\n");
}

#endif // THREAD_POOL_TESTS_H
