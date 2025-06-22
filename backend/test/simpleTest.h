#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <vector>

// Framework de testing simple sin dependencias externas
class SimpleTest
{
private:
    static std::vector<std::function<void()>> tests;
    static std::vector<std::string> testNames;
    static int passedTests;
    static int failedTests;

public:
    static void addTest(const std::string &name, std::function<void()> test)
    {
        testNames.push_back(name);
        tests.push_back(test);
    }

    static void runAllTests()
    {
        std::cout << "🧪 Ejecutando Tests Simples - Proyecto Final\n";
        std::cout << "==========================================\n\n";

        passedTests = 0;
        failedTests = 0;

        for (size_t i = 0; i < tests.size(); ++i)
        {
            std::cout << "[ RUN      ] " << testNames[i] << std::endl;
            try
            {
                tests[i]();
                std::cout << "[       OK ] " << testNames[i] << std::endl;
                passedTests++;
            }
            catch (const std::exception &e)
            {
                std::cout << "[  FAILED  ] " << testNames[i] << " - " << e.what() << std::endl;
                failedTests++;
            }
            catch (...)
            {
                std::cout << "[  FAILED  ] " << testNames[i] << " - Error desconocido" << std::endl;
                failedTests++;
            }
        }

        std::cout << "\n==========================================\n";
        std::cout << "Tests ejecutados: " << (passedTests + failedTests) << std::endl;
        std::cout << "✅ Pasaron: " << passedTests << std::endl;
        std::cout << "❌ Fallaron: " << failedTests << std::endl;

        if (failedTests == 0)
        {
            std::cout << "\n🎉 ¡Todos los tests pasaron!\n";
        }
        else
        {
            std::cout << "\n⚠️  Algunos tests fallaron. Revisa los errores arriba.\n";
        }
    }

    static int getFailedCount() { return failedTests; }
};

// Macros para facilitar el testing
#define ASSERT_EQ(expected, actual)                                       \
    if ((expected) != (actual))                                           \
    {                                                                     \
        throw std::runtime_error("ASSERT_EQ failed: values don't match"); \
    }

#define ASSERT_TRUE(condition)                                               \
    if (!(condition))                                                        \
    {                                                                        \
        throw std::runtime_error("ASSERT_TRUE failed: condition was false"); \
    }

#define ASSERT_FALSE(condition)                                              \
    if (condition)                                                           \
    {                                                                        \
        throw std::runtime_error("ASSERT_FALSE failed: condition was true"); \
    }

#define ASSERT_GT(a, b)                                                                                                   \
    if (!((a) > (b)))                                                                                                     \
    {                                                                                                                     \
        throw std::runtime_error("ASSERT_GT failed: " + std::to_string(a) + " is not greater than " + std::to_string(b)); \
    }

#define ASSERT_NE(a, b)                                                 \
    if ((a) == (b))                                                     \
    {                                                                   \
        throw std::runtime_error("ASSERT_NE failed: values are equal"); \
    }

#define TEST_CASE(name)                              \
    \ void test_##name();                            \
    struct TestRegistrar_##name                      \
    {                                                \
        TestRegistrar_##name()                       \
        {                                            \
            SimpleTest::addTest(#name, test_##name); \
        }                                            \
    };                                               \
    static TestRegistrar_##name registrar_##name;    \
    void test_##name()
