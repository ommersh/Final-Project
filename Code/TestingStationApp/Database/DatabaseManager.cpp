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
        "name TEXT,"
        "testId INTEGER PRIMARY KEY,"
        "format INTEGER,"
        "timeInterval REAL,"
        "iterations INTEGER,"
        "alg INTEGER,"
        "catchPolynomDeg INTEGER,"
        "firstElemData TEXT,"
        "secondElemData TEXT,"
        "numOfTimePoints INTEGER,"
        "realTCA REAL,"
        "realDistance REAL,"
        "distance REAL,"
        "TCA REAL,"
        "runTime REAL,"
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
    std::string sql = "INSERT INTO TestInfo (name, format, timeInterval, iterations, alg, catchPolynomDeg, firstElemData, secondElemData, numOfTimePoints, realTCA, realDistance, distance, TCA, runTime, description) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    //prepare statement 
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    //todo: if elemdata turns from string to char[], then first convert it to string and then give it to bind_text
    sqlite3_bind_text(stmt, 1, test.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, static_cast<int>(test.recipe.format));
    sqlite3_bind_double(stmt, 4, test.recipe.timeInterval);
    sqlite3_bind_int(stmt, 5, test.recipe.iterations);
    sqlite3_bind_int(stmt, 6, static_cast<int>(test.recipe.alg));
    sqlite3_bind_int(stmt, 7, test.recipe.catchPolynomDeg);
    sqlite3_bind_text(stmt, 8, test.recipe.firstElemData.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, test.recipe.secondElemData.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 10, test.recipe.numOfTimePoints);
    sqlite3_bind_double(stmt, 11, test.realTCA);
    sqlite3_bind_double(stmt, 12, test.realDistance);
    sqlite3_bind_double(stmt, 13, test.distance);
    sqlite3_bind_double(stmt, 14, test.TCA);
    sqlite3_bind_double(stmt, 15, test.runTime);
    sqlite3_bind_text(stmt, 16, test.description.c_str(), -1, SQLITE_STATIC);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to insert test record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Update testId in the TestInfo structure with the last inserted row ID
    test.recipe.testId = sqlite3_last_insert_rowid(db);

    //release resources
    sqlite3_finalize(stmt);
    std::cout << "Test record inserted successfully." << std::endl;
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
        "firstElemData = ?, "
        "secondElemData = ?, "
        "numOfTimePoints = ?, "
        "realTCA = ?, "
        "realDistance = ?, "
        "distance = ?, "
        "TCA = ?, "
        "runTime = ?, "
        "description = ? "
        "WHERE testId = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, test.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, static_cast<int>(test.recipe.format));
    sqlite3_bind_double(stmt, 3, test.recipe.timeInterval);
    sqlite3_bind_int(stmt, 4, test.recipe.iterations);
    sqlite3_bind_int(stmt, 5, static_cast<int>(test.recipe.alg));
    sqlite3_bind_int(stmt, 6, test.recipe.catchPolynomDeg);
    sqlite3_bind_text(stmt, 7, test.recipe.firstElemData.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, test.recipe.secondElemData.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 9, test.recipe.numOfTimePoints);
    sqlite3_bind_double(stmt, 10, test.realTCA);
    sqlite3_bind_double(stmt, 11, test.realDistance);
    sqlite3_bind_double(stmt, 12, test.distance);
    sqlite3_bind_double(stmt, 13, test.TCA);
    sqlite3_bind_double(stmt, 14, test.runTime);
    sqlite3_bind_text(stmt, 15, test.description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 16, test.recipe.testId);

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
    TestInfo testInfo;

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
        // Extract data from the result set and populate the TestInfo object
        testInfo.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)); // Assuming name is the first column
        testInfo.recipe.testId = sqlite3_column_int(stmt, 1); // Assuming testId is the second column
        testInfo.recipe.format = static_cast<SatelliteDataFormat>(sqlite3_column_int(stmt, 2)); // Assuming format is the third column and is of type int
        testInfo.recipe.timeInterval = sqlite3_column_double(stmt, 3); // Assuming timeInterval is the fourth column and is of type double
        testInfo.recipe.iterations = sqlite3_column_int(stmt, 4); // Assuming iterations is the fifth column and is of type int
        testInfo.recipe.alg = static_cast<Algorithm>(sqlite3_column_int(stmt, 5)); // Assuming alg is the sixth column and is of type int
        testInfo.recipe.catchPolynomDeg = sqlite3_column_int(stmt, 6); // Assuming catchPolynomDeg is the seventh column and is of type int
        testInfo.recipe.firstElemData = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)); // Assuming firstElemData is the eighth column
        testInfo.recipe.secondElemData = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)); // Assuming secondElemData is the ninth column
        testInfo.recipe.numOfTimePoints = sqlite3_column_int(stmt, 9); // Assuming numOfTimePoints is the tenth column and is of type int
        testInfo.realTCA = sqlite3_column_double(stmt, 10); // Assuming realTCA is the eleventh column and is of type double
        testInfo.realDistance = sqlite3_column_double(stmt, 11); // Assuming realDistance is the twelfth column and is of type double
        testInfo.distance = sqlite3_column_double(stmt, 12); // Assuming distance is the thirteenth column and is of type double
        testInfo.TCA = sqlite3_column_double(stmt, 13); // Assuming TCA is the fourteenth column and is of type double
        testInfo.runTime = sqlite3_column_double(stmt, 14); // Assuming runTime is the fifteenth column and is of type double
        testInfo.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15)); // Assuming description is the sixteenth column
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return testInfo;
}

