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
        "testId INTEGER PRIMARY KEY,"
        "name TEXT,"
        "format INTEGER,"
        "timeInterval REAL,"
        "iterations INTEGER,"
        "alg INTEGER,"
        "catchPolynomDeg INTEGER,"
        "numOfTimePoints INTEGER,"
        "realTCA REAL,"
        "realDistance REAL,"
        "distance REAL,"
        "TCA REAL,"
        "runTime REAL,"
        "status INTEGER,"
        "description TEXT"
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
    std::string sql = "INSERT INTO TestInfo (name, format, timeInterval, iterations, alg, catchPolynomDeg, numOfTimePoints, realTCA, realDistance, distance, TCA, runTime, status) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    // Prepare statement 
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, test.name, 40, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, static_cast<int>(test.format));
    sqlite3_bind_double(stmt, 3, test.recipe.timeInterval);
    sqlite3_bind_int(stmt, 4, test.recipe.iterations);
    sqlite3_bind_int(stmt, 5, static_cast<int>(test.recipe.alg));
    sqlite3_bind_int(stmt, 6, test.recipe.catchPolynomDeg);
    sqlite3_bind_int(stmt, 7, test.recipe.numOfTimePoints);
    sqlite3_bind_double(stmt, 8, test.realTCA);
    sqlite3_bind_double(stmt, 9, test.realDistance);
    sqlite3_bind_double(stmt, 10, test.distance);
    sqlite3_bind_double(stmt, 11, test.TCA);
    sqlite3_bind_double(stmt, 12, test.runTime);
    sqlite3_bind_int(stmt, 13, static_cast<int>(test.status));

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to insert test record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Retrieve the last inserted row ID (testId) and assign it to the TestInfo structure
    test.recipe.testId = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt);
    std::cout << "Test record inserted successfully with testId: " << test.recipe.testId << std::endl;
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
        "numOfTimePoints = ?, "
        "realTCA = ?, "
        "realDistance = ?, "
        "distance = ?, "
        "TCA = ?, "
        "runTime = ?, "
        "status = ? "
        "WHERE testId = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, test.name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, static_cast<int>(test.format));
    sqlite3_bind_double(stmt, 3, test.recipe.timeInterval);
    sqlite3_bind_int(stmt, 4, test.recipe.iterations);
    sqlite3_bind_int(stmt, 5, static_cast<int>(test.recipe.alg));
    sqlite3_bind_int(stmt, 6, test.recipe.catchPolynomDeg);
    sqlite3_bind_int(stmt, 7, test.recipe.numOfTimePoints);
    sqlite3_bind_double(stmt, 8, test.realTCA);
    sqlite3_bind_double(stmt, 9, test.realDistance);
    sqlite3_bind_double(stmt, 10, test.distance);
    sqlite3_bind_double(stmt, 11, test.TCA);
    sqlite3_bind_double(stmt, 12, test.runTime);
    sqlite3_bind_int(stmt, 13, static_cast<int>(test.status));
    sqlite3_bind_int(stmt, 14, test.recipe.testId);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update test record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    std::cout << "Test record updated successfully." << std::endl;
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
    std::cout << "Test record deleted successfully." << std::endl;
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
        testInfo.recipe.testId = sqlite3_column_int(stmt, 0);
        const unsigned char* columnData = sqlite3_column_text(stmt, 1);
        strncpy_s(testInfo.name, reinterpret_cast<const char*>(columnData), sizeof(testInfo.name) - 1);
        testInfo.name[sizeof(testInfo.name) - 1] = '\0';
        testInfo.format = static_cast<SatelliteDataFormat>(sqlite3_column_int(stmt, 2));
        testInfo.recipe.timeInterval = sqlite3_column_double(stmt, 3);
        testInfo.recipe.iterations = sqlite3_column_int(stmt, 4);
        testInfo.recipe.alg = static_cast<AlgorithmsEnums::Algorithm>(sqlite3_column_int(stmt, 5));
        testInfo.recipe.catchPolynomDeg = sqlite3_column_int(stmt, 6); 
        testInfo.recipe.numOfTimePoints = sqlite3_column_int(stmt, 7); 
        testInfo.realTCA = sqlite3_column_double(stmt, 8);
        testInfo.realDistance = sqlite3_column_double(stmt, 9); 
        testInfo.distance = sqlite3_column_double(stmt, 10);
        testInfo.TCA = sqlite3_column_double(stmt, 11);
        testInfo.runTime = sqlite3_column_double(stmt, 12);
        testInfo.status = static_cast<TestStatus>(sqlite3_column_int(stmt, 13)); 
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
