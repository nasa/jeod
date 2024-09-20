#include "../include/io_utils.hh"
#include "../include/orientation.hh"
#include "../include/tokenizer.hh"
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

unsigned process_header(const string s)
{
    unsigned start;
    vector<string> fields = split(s, ',');

    for(unsigned u = 0; u < fields.size(); ++u)
    {
        cout << u << ": " << fields[u] << endl;
    }

    cout << "Enter start index> ";
    cin >> start;
    return start;
}

void process_matrix(double m[3][3], unsigned start, const string l)
{
    vector<string> fields = split(l, ',');

    for(unsigned i = 0; i < 3; ++i)
    {
        for(unsigned j = 0; j < 3; ++j)
        {
            if(sscanf(fields[start++].c_str(), "%lf", &m[i][j]) != 1)
            {
                cerr << fields[start - 1] << "is not a valid double" << endl;
                exit(1);
            }
        }
    }
}

int main(int arg_c, char ** arg_v)
{
    if(arg_c != 4)
    {
        cerr << "Usage: " << arg_v[0] << " <INFILE1> <INFILE2> <OUTFILE>" << endl;
        return -1;
    }

    ifstream fis1(arg_v[1]), fis2(arg_v[2]);
    ofstream fos(arg_v[3]);

    if(!fis1.is_open())
    {
        cerr << "File " << arg_v[1] << " did not open for input." << endl;
        return -2;
    }

    if(!fis2.is_open())
    {
        cerr << "File " << arg_v[2] << " did not open for input." << endl;
        return -3;
    }

    if(!fos.is_open())
    {
        cerr << "File " << arg_v[3] << " did not open for output." << endl;
        return -4;
    }

    string line1;
    string line2;

    if(!fis1.eof())
    {
        getline(fis1, line1);
    }

    if(!fis2.eof())
    {
        getline(fis2, line2);
    }

    unsigned start = process_header(line1);

    while(true)
    {
        double m1[3][3];
        double m2[3][3];
        double delta_theta;
        double delta_omega[3];

        getline(fis1, line1);
        if(fis1.eof())
        {
            break;
        }
        process_matrix(m1, start, line1);
        getline(fis2, line2);
        if(fis2.eof())
        {
            break;
        }
        process_matrix(m2, start, line2);
        Orientation::matrix_delta(m1, m2, &delta_theta, delta_omega);
        fos << "Angle = " << delta_theta << "Axis = ";
        PRINT_VECF(delta_omega, fos);
    }

    fis1.close();
    fis2.close();
    fos.close();

    return 0;
}
