#include <SimpleTimer.h>
#include <unity.h>

#define SAMPLES 10
volatile int16_t currentSeconds = 0;

void setUp()
{
    SimpleTimer::Setup(&currentSeconds);
}

void tearDown()
{

}

void test_time_addition()
{
    TEST_ASSERT_EQUAL(3600, SimpleTimer::Start(0, 3600));
}

void test_time_addition_rollover()
{
    TEST_ASSERT_EQUAL(-29936, SimpleTimer::Start(32000, 3600));
}

void test_time_not_elapsed()
{
    TEST_ASSERT_EQUAL(false, SimpleTimer::Ended(0, 3600));

}

void test_time_not_elapsed_rollover()
{
    TEST_ASSERT_EQUAL(false, SimpleTimer::Ended(32000, -29936));
}

void test_time_elapsed()
{
    TEST_ASSERT_EQUAL(true, SimpleTimer::Ended(3600, 0));

}

void test_time_elapsed_rollover()
{
    TEST_ASSERT_EQUAL(true, SimpleTimer::Ended(-29936, 32000));
}

void test_timer_accuracy()
{

    int16_t lastValue = currentSeconds;
    while (lastValue == currentSeconds) 
    {
        //wait for change in time
    }

    lastValue = currentSeconds;

    long lastTime = micros();
    double result = 0;

    for (int i = 0; i < SAMPLES; i++)
    {
        while (lastValue == currentSeconds) 
        {
        //wait for change in time
        }
        long now = micros();
        result += now - lastTime;
        lastTime = now;
        lastValue = currentSeconds;
    }

    TEST_ASSERT_EQUAL(1000000, result / SAMPLES);
}

void setup()
{
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_time_addition);
    RUN_TEST(test_time_addition_rollover);
    RUN_TEST(test_time_not_elapsed);
    RUN_TEST(test_time_not_elapsed_rollover);
    RUN_TEST(test_time_elapsed);
    RUN_TEST(test_time_elapsed_rollover);
    RUN_TEST(test_timer_accuracy);

    UNITY_END();
}

void loop()
{

}