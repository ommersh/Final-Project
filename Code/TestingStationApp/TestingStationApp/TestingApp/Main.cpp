#include "../../Database/DatabaseManager.h"
//#include "../../TestManager/ResultManager/TestInfo.h"
int main()
{
    // Initialize DatabaseManager with the database file path
    DatabaseManager dbManager("test.db");

    // Create a TestInfo object for the first test
    TestInfo test1;
    test1.name = "Test 1";
    test1.status = TestStatus::InProgress;
    test1.realTCA = 10.5;
    test1.realDistance = 20.3;
    test1.distance = 30.7;
    test1.TCA = 40.9;
    test1.runTime = 50.2;
    test1.description = "First test";

    // Create the first test and print its ID and all values
    if (dbManager.createTest(test1)) {
        std::cout << "Test 1 created successfully." << std::endl;
        std::cout << "Test 1 ID: " << test1.recipe.testId << std::endl;
        std::cout << "Printing Test 1 info:" << std::endl;
        std::cout << "Name: " << test1.name << std::endl;
        std::cout << "Status: " << static_cast<int>(test1.status) << std::endl;
        std::cout << "Real TCA: " << test1.realTCA << std::endl;
        std::cout << "Real Distance: " << test1.realDistance << std::endl;
        std::cout << "Distance: " << test1.distance << std::endl;
        std::cout << "TCA: " << test1.TCA << std::endl;
        std::cout << "Run Time: " << test1.runTime << std::endl;
        std::cout << "Description: " << test1.description << std::endl;
    }
    else {
        std::cerr << "Failed to create Test 1." << std::endl;
    }

    // Create two more tests
    TestInfo test2;
    TestInfo test3;

    // Create the second test
    test2.name = "Test 2";
    // Set other fields as needed

    // Create the third test
    test3.name = "Test 3";
    // Set other fields as needed

    // Create the second test and print its ID
    if (dbManager.createTest(test2)) {
        std::cout << "Test 2 created successfully." << std::endl;
        std::cout << "Test 2 ID: " << test2.recipe.testId << std::endl;
    }
    else {
        std::cerr << "Failed to create Test 2." << std::endl;
    }

    // Create the third test and print its ID
    if (dbManager.createTest(test3)) {
        std::cout << "Test 3 created successfully." << std::endl;
        std::cout << "Test 3 ID: " << test3.recipe.testId << std::endl;
    }
    else {
        std::cerr << "Failed to create Test 3." << std::endl;
    }

    return 0;
}