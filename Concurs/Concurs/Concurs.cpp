#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#define kids "kids_wish.csv"
#define ded "ded_moroz_wish.csv"
#define result "result.csv"
#define size 10
using namespace std;

int ANICH()
{
    return 0;
}

int ANIFH()
{
    return 0;
}

int ANIH()
{
    return ANICH() + ANIFH();
}

void TempIn(string line)
{
    ifstream work_file_kids(kids);
    ofstream work_file_result(result);
    char limiter = ',';
    char delimiter = ',';
    vector<int> childGift;
    int IntIdGift = 0;
    int IntIdChild = 0;
    while (getline(work_file_kids, line)) 
    {
        stringstream stream(line);
        string idGift;
        string idChild;
        bool cycling = true;
        getline(stream, idChild, delimiter);
        for (int Index = 0; Index < 100; Index++)
        {
            getline(stream, idGift, delimiter);
            IntIdGift = stoi(idGift);
            IntIdChild = stoi(idChild);
            childGift.push_back(IntIdGift);
        }
        while (cycling)
        {
            int random_gift = rand() % 100;
            for (int Index = 0; Index < 100; Index++)
            {
                if (childGift[Index] == random_gift)
                {
                    work_file_result << IntIdChild << limiter << random_gift << "\n";
                    cout << "JOPA";
                    cycling = false;
                }
            }
        }
        childGift.clear();
    }   
    work_file_kids.close();
}

int main()
{
    unsigned short int GiftCounter[1000];
    //string FileName = kids;
    string Line;
    srand(time(0));

    TempIn(Line);
    ANIH();

    
}