#include <SimpleTimer.h>
#include <DataStorage.h>
#include <WaterTank.h>
#include <unity.h>

DataStorage::WaterTankData data;
DataStorage::WaterTankSettings settings;

void setUp()
{
    settings.housePumpContactorPin = 2;
    settings.wellPumpContactorPin = 3;
    settings.tankFillSolenoidPin = 4;
    pinMode(settings.housePumpContactorPin, OUTPUT);
    pinMode(settings.wellPumpContactorPin, OUTPUT);
    pinMode(settings.tankFillSolenoidPin, OUTPUT);
}

void tearDown()
{

}

void test_weight_target_max()
{
    data.weight = 20000;
    data.wellDepth = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    settings.maxIncWeight = 3000;
    TEST_ASSERT_EQUAL_UINT16(3000,WaterTank::calculateWeightTarget(settings, data));  
}

void test_weight_target_well_limited()
{
    data.weight = 20000;
    data.wellDepth = 1200;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    settings.maxIncWeight = 3000;
    TEST_ASSERT_EQUAL_UINT16(612,WaterTank::calculateWeightTarget(settings, data));  
}

void test_weight_target_tank_limited()
{
    data.weight = 34500;
    data.wellDepth = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    settings.maxIncWeight = 3000;
    TEST_ASSERT_EQUAL_UINT16(500,WaterTank::calculateWeightTarget(settings, data));  
}

void test_stop_tank_full()
{
    digitalWrite(settings.tankFillSolenoidPin, HIGH);
    data.weight = 35000;
    data.wellDepth = 0;
    data.fillingTimeTarget = 1000;
    data.fillingWeightTarget = 36000;
    uint16_t now = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_TRUE(WaterTank::stopTankIfNeeded(settings, data, now));  
}

void test_stop_tank_well()
{
    digitalWrite(settings.tankFillSolenoidPin, HIGH);
    data.weight = 30000;
    data.wellDepth = 1251;
    data.fillingTimeTarget = 1000;
    data.fillingWeightTarget = 36000;
    uint16_t now = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_TRUE(WaterTank::stopTankIfNeeded(settings, data, now));  
}

void test_stop_tank_timeout()
{
    digitalWrite(settings.tankFillSolenoidPin, HIGH);
    data.weight = 30000;
    data.wellDepth = 0;
    data.fillingTimeTarget = 1000;
    data.fillingWeightTarget = 36000;
    uint16_t now = 1001;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_TRUE(WaterTank::stopTankIfNeeded(settings, data, now));  
}

void test_stop_tank_complete()
{
    digitalWrite(settings.tankFillSolenoidPin, HIGH);
    data.weight = 32001;
    data.wellDepth = 1251;
    data.fillingTimeTarget = 1000;
    data.fillingWeightTarget = 32000;
    uint16_t now = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_TRUE(WaterTank::stopTankIfNeeded(settings, data, now));  
}

void test_start_tank()
{
    digitalWrite(settings.tankFillSolenoidPin, LOW);
    data.weight = 30000;
    data.wellDepth = 0;
    data.rechargeTimeTarget = -100;
    data.wellDataGoodUntil = 100;
    uint16_t now = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_TRUE(WaterTank::startTankIfNeeded(settings, data, now));  
}

void test_start_tank_filling()
{
    digitalWrite(settings.tankFillSolenoidPin, HIGH);
    data.weight = 30000;
    data.wellDepth = 0;
    data.rechargeTimeTarget = -100;
    data.wellDataGoodUntil = 100;
    uint16_t now = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_FALSE(WaterTank::startTankIfNeeded(settings, data, now));  
}

void test_start_tank_full()
{
    digitalWrite(settings.tankFillSolenoidPin, LOW);
    data.weight = 35000;
    data.wellDepth = 0;
    data.rechargeTimeTarget = -100;
    data.wellDataGoodUntil = 100;
    uint16_t now = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_FALSE(WaterTank::startTankIfNeeded(settings, data, now));  
}

void test_start_tank_well()
{
    digitalWrite(settings.tankFillSolenoidPin, LOW);
    data.weight = 30000;
    data.wellDepth = 1251;
    data.rechargeTimeTarget = -100;
    data.wellDataGoodUntil = 100;
    uint16_t now = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_FALSE(WaterTank::startTankIfNeeded(settings, data, now));  
}

void test_start_tank_recharge()
{
    digitalWrite(settings.tankFillSolenoidPin, LOW);
    data.weight = 30000;
    data.wellDepth = 0;
    data.rechargeTimeTarget = 100;
    data.wellDataGoodUntil = 100;
    uint16_t now = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_FALSE(WaterTank::startTankIfNeeded(settings, data, now));  
}

void test_start_tank_well_timeout()
{
    digitalWrite(settings.tankFillSolenoidPin, LOW);
    data.weight = 35000;
    data.wellDepth = 0;
    data.rechargeTimeTarget = -100;
    data.wellDataGoodUntil = -100;
    uint16_t now = 0;
    settings.maxWeight = 35000;
    settings.wellWorkingDepth = 1250;
    TEST_ASSERT_FALSE(WaterTank::startTankIfNeeded(settings, data, now));  
}

void setup()
{
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_weight_target_max);
    RUN_TEST(test_weight_target_well_limited);
    RUN_TEST(test_weight_target_tank_limited);
    RUN_TEST(test_stop_tank_full);
    RUN_TEST(test_stop_tank_well);
    RUN_TEST(test_stop_tank_timeout);
    RUN_TEST(test_stop_tank_complete);
    RUN_TEST(test_start_tank);
    RUN_TEST(test_start_tank_filling);
    RUN_TEST(test_start_tank_full);
    RUN_TEST(test_start_tank_well);
    RUN_TEST(test_start_tank_recharge);
    RUN_TEST(test_start_tank_well_timeout);


    UNITY_END();
}

void loop()
{

}