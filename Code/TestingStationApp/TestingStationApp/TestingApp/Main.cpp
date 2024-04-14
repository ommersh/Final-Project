#include "../../Database/DatabaseManager.h"

void printTestInfo(const TestInfo& test) {
    //std::cout << "Name: " << test.name << std::endl;
    std::cout << "Format: " << int(test.format) << std::endl;
    std::cout << "Time Interval: " << test.recipe.timeIntervalSizeSec << std::endl;
    std::cout << "Iterations: " << test.recipe.numberOfIterations<< std::endl;
    std::cout << "Algorithm: " << int(test.recipe.testedAlgorithm) << std::endl;
    std::cout << "Catch Polynom Degree: " << test.recipe.catchPolynomialDegree << std::endl;
    //std::cout << "First Element Data: " << test.firstElemData << std::endl;
    //std::cout << "Second Element Data: " << test.secondElemData << std::endl;
    std::cout << "Number of Time Points: " << test.recipe.numberOfPoints << std::endl;
    std::cout << "Real TCA: " << test.realTCA << std::endl;
    std::cout << "Real Distance: " << test.realDistance << std::endl;
    std::cout << "Distance: " << test.distance << std::endl;
    std::cout << "TCA: " << test.TCA << std::endl;
    std::cout << "Run Time: " << test.runTime << std::endl;
    std::cout << "Status: " << int(test.status) << std::endl;
}


int main()
{
    //// Database file path
    //std::string dbPath = "test.db";

    //// Create a DatabaseManager instance
    //DatabaseManager dbManager(dbPath);

    //// Create tables if they don't exist
    //if (!dbManager.createTables()) {
    //    std::cerr << "Failed to create tables." << std::endl;
    //    return 1;
    //}

    //// Create a test
    //TestInfo test;
    //test.name = "Test 1";
    //test.format = SatelliteDataFormat::XML;
    //test.recipe.timeInterval = 10.5;
    //test.recipe.iterations = 100;
    //test.recipe.alg = Algorithm::Ancas;
    //test.recipe.catchPolynomDeg = 5;
    //test.firstElemData = "Data 1";
    //test.secondElemData = "Data 2";
    //test.recipe.numOfTimePoints = 50;
    //test.realTCA = 20.3;
    //test.realDistance = 100.7;
    //test.distance = 105.2;
    //test.TCA = 18.8;
    //test.runTime = 15.6;
    //test.status = TestStatus::Completed;

    //if (dbManager.createTest(test)) {
    //    std::cout << "Test created successfully." << std::endl;
    //}
    //else {
    //    std::cerr << "Failed to create test." << std::endl;
    //    return 1;
    //}

    //std::cout << "\n\nTest before editing:" << std::endl;
    //printTestInfo(test);

    //// Edit the test
    //test.realTCA = 25.8;
    //test.realDistance = 110.2;

    //if (dbManager.editTest(test)) {
    //    std::cout << "Test edited successfully." << std::endl;
    //}
    //else {
    //    std::cerr << "Failed to edit test." << std::endl;
    //    return 1;
    //}

    //test = dbManager.getTestInfo(test.recipe.testId);

    //std::cout << "\n\nTest after editing:" << std::endl;
    //printTestInfo(test);

    //// Delete the test
    //unsigned int testIdToDelete = test.recipe.testId;
    //if (dbManager.deleteTest(testIdToDelete)) {
    //    std::cout << "Test deleted successfully." << std::endl;
    //}
    //else {
    //    std::cerr << "Failed to delete test." << std::endl;
    //    return 1;
    //}


    return 0;
}
