#include "FileReader.h"

sFileData FileReader::readDataFromFile(string filePath)
{
    sFileData fileData;
    vector<vector<string>> content;
    vector<string> row;
    string line, word;
    int size, r;
    //Lest start by opening the file we want to read

    fstream file(filePath, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
    {
        cout << "Could not open the file\n";
        ///
    }
    //lets get the number of rows!
    size = content.size() - 1;//The first row is just index names
    //Lets create the arrays for the data!
    fileData.data = new sPointData[size];
    fileData.timePoints = new double[size];
    fileData.size = size;
    //Lets get the data into the arrays!
    for (r = 0; r < size; r++) {
        fileData.timePoints[r] = stod(content.at(r + 1).at(TIMES_COL));
        fileData.data[r].r1x = stod(content.at(r + 1).at(R1X_COL));
        fileData.data[r].r1y = stod(content.at(r + 1).at(R1Y_COL));
        fileData.data[r].r1z = stod(content.at(r + 1).at(R1Z_COL));
        fileData.data[r].r2x = stod(content.at(r + 1).at(R2X_COL));
        fileData.data[r].r2y = stod(content.at(r + 1).at(R2Y_COL));
        fileData.data[r].r2z = stod(content.at(r + 1).at(R2Z_COL));
        fileData.data[r].v1x = stod(content.at(r + 1).at(V1X_COL));
        fileData.data[r].v1y = stod(content.at(r + 1).at(V1Y_COL));
        fileData.data[r].v1z = stod(content.at(r + 1).at(V1Z_COL));
        fileData.data[r].v2x = stod(content.at(r + 1).at(V2X_COL));
        fileData.data[r].v2y = stod(content.at(r + 1).at(V2Y_COL));
        fileData.data[r].v2z = stod(content.at(r + 1).at(V2Z_COL));
    }

    return fileData;
}

