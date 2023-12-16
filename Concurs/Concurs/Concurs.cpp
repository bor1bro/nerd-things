#include <iostream>
#include <sstream>
#include <fstream>
#define filename "kids_wish.csv"
using namespace std;
int main()
{

    ifstream work_file(filename);
    unsigned short int GiftCounter[1000];
    string line;
    char delimiter = ',';
    while (getline(work_file, line))
    {
        int Counter = 0;
        stringstream stream(line);
        string idGift;
        for (int Index = 0; Index < 100; Index++)
        {
            getline(stream, idGift, delimiter);
            cout << idGift << " ";
            Counter++;
        }
        cout << endl;
        cout << endl;
        cout << Counter << endl;
        cout << endl;
        cout << endl;
    }
    work_file.close();
}
