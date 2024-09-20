#include <fstream>

#include "../include/read_trk_csv.hh"
#include <iostream>

using namespace std;

ReadTrkCsv::ReadTrkCsv(string fileName)
{
    ifstream ifs(fileName);
    string line;
    vector<double *> rows;

    getline(ifs, header);
    while(getline(ifs, line))
    {
        vector<double> rowVector;
        const char * first = line.c_str();
        const char * last = first + line.length();
        auto * start = const_cast<char *>(first);
        char * next;
        double v = strtod(start, &next);

        while(start < last)
        {
            rowVector.push_back(v);
            start = next + 1;
            v = strtod(start, &next);
        }

        if(numCols == 0)
        {
            numCols = rowVector.size();
        }
        if(rowVector.size() == numCols)
        {
            auto * row = new double[numCols];
            double * t = row;
            copy(rowVector.begin(), rowVector.end(), t);
            rows.push_back(row);
        }
    }

    values = new double *[numRows = rows.size()];

    double ** t = values;

    copy(rows.begin(), rows.end(), t);

    // cout << "Read " << numRows << " rows of " << numCols << " items\n";
    // cout << "from " << fileName << endl;
}

ReadTrkCsv::~ReadTrkCsv()
{
    for(unsigned int ii = 0; ii < numRows; ++ii)
    {
        delete[] values[ii];
    }

    delete[] values;
}
