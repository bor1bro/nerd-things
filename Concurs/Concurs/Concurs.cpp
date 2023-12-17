#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <windows.h>
#define kids "kids_wish1.csv"
#define ded "ded_moroz_wish.csv"
#define result "result.csv"
#define stockList_size 1000
#define wishList_size 100
#define maxGiftCount 250
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

void Progress_Bar(double progress_c)
{
    int progress_b = (progress_c / 184000) * 100;
    COORD pos = {0, 0};

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    cout << progress_b << "%" << endl;
}

bool Is_OutOfStock(unsigned short int* stockList, vector<int> wishList)
{
    for (int Index = 0; Index < wishList_size; Index++)
    {
        if (stockList[wishList[Index]] == maxGiftCount)
            return true;
    }
    return false;
}

void TempIn(string line, unsigned short int* giftCounter, vector<int> child_wish_list)
{
    double progress_counter = 0;
    ifstream work_file_kids(kids);
    ofstream work_file_result(result);
    char limiter = ',';
    char delimiter = ',';
    int intIdGift = 0;
    int intIdChild = 0;
    //going through every str
    while (getline(work_file_kids, line)) 
    {
        stringstream stream(line);
        string idGift;
        string idChild;
        bool cycling = true;
        //taking only child id from the str
        getline(stream, idChild, delimiter);
        for (int Index = 0; Index < wishList_size; Index++)
        {
            //storing all desired gifts id's into array
            getline(stream, idGift, delimiter);
            intIdGift = stoi(idGift);
            intIdChild = stoi(idChild);
            child_wish_list.push_back(intIdGift);
        }
        //giving random gifts to children
        while (cycling)
        {           
            int random_gift = rand() % stockList_size;
            for (int Index = 0; Index < wishList_size; Index++)
            {
                //if random gift is in child's wishlist, 
                //then check if there is this type of gift in stock
                            //if there is, give it to child
                //if the gift is not in wishlist, 
                //but the gifts that are in it are out of stock, 
                //give child this gift
                if ((child_wish_list[Index] == random_gift && giftCounter[random_gift] < maxGiftCount) ||
                    (child_wish_list[Index] != random_gift && Is_OutOfStock(giftCounter, child_wish_list)))
                {   
                    work_file_result << intIdChild << limiter << random_gift << "\n";
                    //counting how much of certain type of gift is left
                    giftCounter[random_gift]++;
                    progress_counter++;
                    cycling = false;
                    break;
                }
                            //if there isn't, check other gift
                //silly progress bar :3
                Progress_Bar(progress_counter);
            }
        }
        //clear previous child gifts id's data
        child_wish_list.clear();
    }   
    work_file_kids.close();
}

int main()
{
    unsigned short int GiftCounter[stockList_size];
    vector<int> ChildGiftlist;
    string Line;
    srand(time(0));

    TempIn(Line, GiftCounter, ChildGiftlist);
    ANIH();  
}