#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "sqlite3.h"
#include "TestInfo.h"
#include <fstream>
#include <iostream>
#include <set>

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    bool createTables();
    bool createDatabase(const std::string& dbPath);

    bool createTest(TestInfo& test);
    bool editTest(const TestInfo& test);
    bool deleteTest(unsigned int testId);
    TestInfo getTestInfo(int testId);
    std::set<int> getAllTestIds();

private:
    sqlite3* db;

    bool executeSql(const std::string& sql);
    bool fileExists(const std::string& filename);
    bool tablesExist();
};

#endif // DATABASEMANAGER_H