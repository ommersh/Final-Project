#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& dbPath) : db(nullptr) {
    // Open the database
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        db = nullptr;
    }
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

bool DatabaseManager::createTables() {
    if (tablesExist()) {
        std::cout << "Tables have already been created." << std::endl;
        return true;
    }

    bool success = true;

    // Create table
    std::string createTestInfoTableSql = "CREATE TABLE IF NOT EXISTS TestInfo ("
        "testId INTEGER PRIMARY KEY,"           //0
        "name TEXT,"                            //1
        "format INTEGER,"                       //2
        "timeInterval REAL,"                    //3
        "iterations INTEGER,"                   //4
        "alg INTEGER,"                          //5
        "catchPolynomDeg INTEGER,"              //6
        "numberOfPointsPerSegment INTEGER,"     //7
        "numOfTimePoints INTEGER,"              //8
        "realTCA REAL,"                         //9
        "realDistance REAL,"                    //10
        "timeOfTcaFromStartingPointSec REAL, "  //11
        "distanceOfTcaKM REAL, "                //12
        "numberOfPointsTheAlgUsed INTEGER,"     //13
        "runTimeMicro REAL, "                   //14
        "avgRunTimeMicro REAL, "                //15
        "minRunTimeMicro REAL, "                //16
        "status INTEGER,"                       //17
        "SegmentSize REAL, "                    //18
        "TminFactor REAL, "                     //19
        "TOLd INTEGER, "                        //20
        "TOLt INTEGER, "                        //21
        "description TEXT"                      //22
        ");";
    success &= executeSql(createTestInfoTableSql);

    return success;
}

bool DatabaseManager::createDatabase(const std::string& dbPath) {
    if (fileExists(dbPath)) {
        std::cout << "Database file already exists." << std::endl;
        return true; // Database already exists, no need to create
    }

    // Create the database file
    sqlite3* tempDb;
    int rc = sqlite3_open(dbPath.c_str(), &tempDb);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create database: " << sqlite3_errmsg(tempDb) << std::endl;
        sqlite3_close(tempDb);
        return false;
    }
    sqlite3_close(tempDb);

    std::cout << "Database created successfully." << std::endl;
    return true;
}

bool DatabaseManager::executeSql(const std::string& sql) {
    if (!db) {
        std::cerr << "Database not opened." << std::endl;
        return false;
    }

    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool DatabaseManager::tablesExist() {
    if (!db) {
        std::cerr << "Database not opened." << std::endl;
        return false;
    }

    // Check if tables exist
    std::string sql = "SELECT name FROM sqlite_master WHERE type='table';";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    bool tablesExist = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* tableName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (strcmp(tableName, "TestInfo") == 0) {
            tablesExist = true;
            break;
        }
    }

    sqlite3_finalize(stmt);
    return tablesExist;
}

bool DatabaseManager::createTest(TestInfo& test) {
    //                                         1      2         3           4         5         6                       7                     8          
    std::string sql = "INSERT INTO TestInfo (name, format, timeInterval, iterations, alg, catchPolynomDeg, numberOfPointsPerSegment, numOfTimePoints, " 
    //      9           10                  11                       12                 13                     14                15            16         
        "realTCA, realDistance, timeOfTcaFromStartingPointSec, distanceOfTcaKM, numberOfPointsTheAlgUsed, runTimeMicro, avgRunTimeMicro, minRunTimeMicro, "
    //      17         18          19      20    21
        "status, SegmentSize, TminFactor, TOLd, TOLt) "
    //           1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    // Prepare statement 
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, test.recipe.testName, 80, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, static_cast<int>(test.format));
    sqlite3_bind_double(stmt, 3, test.recipe.timeIntervalSizeSec);
    sqlite3_bind_int(stmt, 4, test.recipe.numberOfIterations);
    sqlite3_bind_int(stmt, 5, static_cast<int>(test.recipe.testedAlgorithm));
    sqlite3_bind_int(stmt, 6, test.recipe.catchPolynomialDegree);
    sqlite3_bind_int(stmt, 7, test.recipe.numberOfPointsPerSegment);
    sqlite3_bind_int(stmt, 8, test.recipe.numberOfPoints);
    sqlite3_bind_double(stmt, 9, test.realTCA);
    sqlite3_bind_double(stmt, 10, test.realDistance);
    sqlite3_bind_double(stmt, 11, test.timeOfTcaFromStartingPointSec);
    sqlite3_bind_double(stmt, 12, test.distanceOfTcaKM);
    sqlite3_bind_int(stmt, 13, test.numberOfPointsTheAlgUsed);
    sqlite3_bind_double(stmt, 14, test.runTimeMicro);
    sqlite3_bind_double(stmt, 15, test.avgRunTimeMicro);
    sqlite3_bind_double(stmt, 16, test.minRunTimeMicro);
    sqlite3_bind_int(stmt, 17, static_cast<int>(test.status));
    sqlite3_bind_double(stmt, 18, test.recipe.segmentSizeSec);
    sqlite3_bind_int(stmt, 19, test.recipe.TminFactor);
    sqlite3_bind_double(stmt, 20, test.recipe.TOLd);
    sqlite3_bind_double(stmt, 21, test.recipe.TOLt);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to insert test record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Retrieve the last inserted row ID (testId) and assign it to the TestInfo structure
    test.recipe.testID = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt);
    //std::cout << "Test record inserted successfully with testId: " << test.recipe.testID << std::endl;
    return true;
}

bool DatabaseManager::editTest(const TestInfo& test) {
    std::string sql = "UPDATE TestInfo SET "
        "name = ?, "
        "format = ?, "
        "timeInterval = ?, "
        "iterations = ?, "
        "alg = ?, "
        "catchPolynomDeg = ?, "
        "numberOfPointsPerSegment = ?, "
        "numOfTimePoints = ?, "
        "realTCA = ?, "
        "realDistance = ?, "
        "timeOfTcaFromStartingPointSec = ?, "
        "distanceOfTcaKM = ?, "
        "numberOfPointsTheAlgUsed = ?, "
        "runTimeMicro = ?, "
        "avgRunTimeMicro = ?, "
        "minRunTimeMicro = ?, "
        "status = ? "
        "SegmentSize = ? "
        "TminFactor = ? "
        "TOLd = ? "
        "TOLt = ? "
        "WHERE testId = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, test.recipe.testName, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, static_cast<int>(test.format));
    sqlite3_bind_double(stmt, 3, test.recipe.timeIntervalSizeSec);
    sqlite3_bind_int(stmt, 4, test.recipe.numberOfIterations);
    sqlite3_bind_int(stmt, 5, static_cast<int>(test.recipe.testedAlgorithm));
    sqlite3_bind_int(stmt, 6, test.recipe.catchPolynomialDegree);
    sqlite3_bind_int(stmt, 7, test.recipe.numberOfPointsPerSegment);
    sqlite3_bind_int(stmt, 8, test.recipe.numberOfPoints);
    sqlite3_bind_double(stmt, 9, test.realTCA);
    sqlite3_bind_double(stmt, 10, test.realDistance);
    sqlite3_bind_double(stmt, 11, test.timeOfTcaFromStartingPointSec);
    sqlite3_bind_double(stmt, 12, test.distanceOfTcaKM);
    sqlite3_bind_int(stmt, 13, test.numberOfPointsTheAlgUsed);
    sqlite3_bind_double(stmt, 14, test.runTimeMicro);
    sqlite3_bind_double(stmt, 15, test.avgRunTimeMicro);
    sqlite3_bind_double(stmt, 16, test.minRunTimeMicro);
    sqlite3_bind_int(stmt, 17, static_cast<int>(test.status));
    sqlite3_bind_double(stmt, 18, test.recipe.segmentSizeSec);
    sqlite3_bind_int(stmt, 19, test.recipe.TminFactor);
    sqlite3_bind_double(stmt, 20, test.recipe.TOLd);
    sqlite3_bind_double(stmt, 21, test.recipe.TOLt);
    sqlite3_bind_int(stmt, 22, test.recipe.testID);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update test record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    //std::cout << "Test record updated successfully." << std::endl;
    return true;
}

bool DatabaseManager::deleteTest(unsigned int testId) {
    std::string sql = "DELETE FROM TestInfo WHERE testId = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, testId);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to delete test record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    //std::cout << "Test record deleted successfully." << std::endl;
    return true;
}

TestInfo DatabaseManager::getTestInfo(int testId) {
    TestInfo testInfo = TestInfo();

    // Prepare SQL statement
    std::string sql = "SELECT * FROM TestInfo WHERE testId = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return testInfo; // Return an empty TestInfo object on error
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, testId);

    // Step through the result set
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        testInfo.recipe.testID = sqlite3_column_int(stmt, 0);
        const unsigned char* columnData = sqlite3_column_text(stmt, 1);
        strncpy_s(testInfo.recipe.testName, reinterpret_cast<const char*>(columnData), sizeof(testInfo.recipe.testName) - 1);
        testInfo.recipe.testName[sizeof(testInfo.recipe.testName) - 1] = '\0';
        testInfo.format = static_cast<SatelliteDataFormat>(sqlite3_column_int(stmt, 2));
        testInfo.recipe.timeIntervalSizeSec = sqlite3_column_double(stmt, 3);
        testInfo.recipe.numberOfIterations = sqlite3_column_int(stmt, 4);
        testInfo.recipe.testedAlgorithm = static_cast<AlgorithmsEnums::Algorithm>(sqlite3_column_int(stmt, 5));
        testInfo.recipe.catchPolynomialDegree = sqlite3_column_int(stmt, 6); 
        testInfo.recipe.numberOfPointsPerSegment = sqlite3_column_int(stmt, 7);
        testInfo.recipe.numberOfPoints = sqlite3_column_int(stmt, 8); 
        testInfo.realTCA = sqlite3_column_double(stmt, 9);
        testInfo.realDistance = sqlite3_column_double(stmt, 10); 
        testInfo.timeOfTcaFromStartingPointSec = sqlite3_column_double(stmt, 11);
        testInfo.distanceOfTcaKM = sqlite3_column_double(stmt, 12);
        testInfo.numberOfPointsTheAlgUsed = sqlite3_column_int(stmt, 13);
        testInfo.runTimeMicro = sqlite3_column_double(stmt, 14);
        testInfo.avgRunTimeMicro = sqlite3_column_double(stmt, 15);
        testInfo.minRunTimeMicro = sqlite3_column_double(stmt, 16);
        testInfo.status = static_cast<TestStatus>(sqlite3_column_int(stmt, 17)); 
        testInfo.recipe.segmentSizeSec = sqlite3_column_double(stmt, 18);
        testInfo.recipe.TminFactor = sqlite3_column_int(stmt, 19);
        testInfo.recipe.TOLd = sqlite3_column_double(stmt, 20);
        testInfo.recipe.TOLt = sqlite3_column_double(stmt, 21);

    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return testInfo;
}

std::set<int> DatabaseManager::getAllTestIds() {
    std::set<int> testIds ;

    // Prepare SQL statement
    std::string sql = "SELECT testId FROM TestInfo;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return testIds; // Return an empty set on error
    }

    // Step through the result set and populate the set
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        unsigned int id = sqlite3_column_int(stmt, 0);
        testIds.insert(id);
        std::cerr << id << sqlite3_errmsg(db) << std::endl;

    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return testIds;
}

//Handle Test State Change and Test Results
bool DatabaseManager::updateTestResults(const TestInfo test)
{
    std::string sql = "UPDATE TestInfo SET "
        "timeOfTcaFromStartingPointSec = ?, "   //1
        "distanceOfTcaKM = ?, "                 //2
        "numberOfPointsTheAlgUsed = ?, "        //3
        "runTimeMicro = ?, "                    //4
        "avgRunTimeMicro = ?, "                 //5
        "minRunTimeMicro = ? "                  //6
        "WHERE testId = ?;";                    //7

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters
    sqlite3_bind_double (stmt, 1, test.timeOfTcaFromStartingPointSec);
    sqlite3_bind_double (stmt, 2, test.distanceOfTcaKM);
    sqlite3_bind_int    (stmt, 3, test.numberOfPointsTheAlgUsed);
    sqlite3_bind_double (stmt, 4, test.runTimeMicro);
    sqlite3_bind_double (stmt, 5, test.avgRunTimeMicro);
    sqlite3_bind_double (stmt, 6, test.minRunTimeMicro);
    sqlite3_bind_int    (stmt, 7, test.recipe.testID);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update test record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    //std::cout << "Test record updated successfully." << std::endl;
    return true;
}
bool DatabaseManager::updateRealTca(const TestInfo test)
{
    std::string sql = "UPDATE TestInfo SET "
        "realTCA = ?, "          //1
        "realDistance = ? "      //2
        "WHERE testId = ?;";     //3
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters
    sqlite3_bind_double(stmt, 1, test.realTCA);
    sqlite3_bind_double(stmt, 2, test.realDistance);
    sqlite3_bind_int(stmt, 3, test.recipe.testID);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update test record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    //std::cout << "Test record updated successfully." << std::endl;
    return true;
}

bool DatabaseManager::updateTestStatus(const TestStatus status, const unsigned int testId)
{
    std::string sql = "UPDATE TestInfo SET "
        "status = ? "           //1
        "WHERE testId = ?;";    //2

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, static_cast<int>(status));
    sqlite3_bind_int(stmt, 2, testId);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update test record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    //std::cout << "Test record updated successfully." << std::endl;
    return true;
}