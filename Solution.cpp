#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <windows.h>
#define kids "kids_wish.csv"
#define ded "ded_moroz_wish.csv"
#define result "result.csv"
#define stockList_size 1000
#define wishList_size 100
#define maxGiftCount 250
using namespace std;

void Progress_Bar(double progress_c)
{
    int progress_b = (progress_c / 184000) * 100;
    COORD pos = { 0, 0 };

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    cout << progress_b << "%" << endl;
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

//mine
    int present_option = 0, present_option_2 = 0, present_final = 0;
    int temp = 0;
    int takenIndex_2 = 0;
    int intIdChild_2 = 0;
    vector<int> child_wish_list_2;
    int gift_pos_1 = 0, gift_pos_2 = 0;
//    

    work_file_result << "ChildId" << limiter << "GiftId" << endl;

    //going through every str
    while (getline(work_file_kids, line))
    {
        stringstream stream(line);
        string idGift;
        string idChild;
        bool cycling = true;
        //index to iterate through gifts that are given to child
        int takenIndex = 0;
        int takenGiftCounter = 0;
        //taking only child id from the str
        getline(stream, idChild, delimiter);
        intIdChild = stoi(idChild);

        for (int Index = 0; Index < wishList_size; Index++)
        {
            //storing all desired gifts id's into array
            getline(stream, idGift, delimiter);
            intIdGift = stoi(idGift);
            //intIdChild = stoi(idChild);
            child_wish_list.push_back(intIdGift);
        }
///
        string idChild_2;
        if(intIdChild < 5520){
            //take the next line (second sibling)
            getline(work_file_kids, line);
            stringstream stream_2(line); //convert it

            getline(stream_2, idChild_2, delimiter); // we have to do it to cut off the id from the string of 100 present id's
            intIdChild_2 = stoi(idChild_2);

            for (int Index = 0; Index < wishList_size; Index++){
                //storing all desired gifts id's into array
                getline(stream_2, idGift, delimiter);
                intIdGift = stoi(idGift);
                child_wish_list_2.push_back(intIdGift);
            }
        }
///
        //giving gifts to children
        while (cycling)
        {
            int random_gift = rand() % stockList_size;
            //int gift = child_wish_list[takenIndex];
            for (int Index = 0; Index < wishList_size; Index++)
            {
                int gift = child_wish_list[takenIndex];

                gift_pos_1 = Index;
                //if gift is in child's wishlist, 
                //then check if there is this type of gift in stock
                            //if there is, give it to child
                //if the gift is not in wishlist, 
                //but the gifts that are in it are out of stock, 
                //give child this gift
                if (giftCounter[gift] < maxGiftCount)
                {
                    if(intIdChild < 5520){
                        for(int Index_2 = 0; Index_2 < wishList_size; Index_2++){
                            gift_pos_2 = Index_2;

                            present_option = child_wish_list_2[takenIndex_2];
                            if (giftCounter[present_option] < maxGiftCount){
                                //fuck
                                if(gift_pos_1 <= gift_pos_2) break;
                                else {
                                    gift = present_option;
                                    break;
                                }
                            }

                            if (giftCounter[present_option] >= maxGiftCount){
                                break;
                            }

                            takenIndex_2++;
                        }
                    }


                    work_file_result << intIdChild << limiter << gift << endl;
                    if(intIdChild < 5520){
                        work_file_result << intIdChild_2 << limiter << gift << endl;
                        giftCounter[gift]++;
                    }
                    //counting how much of certain type of gift is left
                    giftCounter[gift]++;
                    progress_counter++;
                    cycling = false;
                    break;
                }
                if (giftCounter[gift] >= maxGiftCount)
                {
                    if (takenIndex >= wishList_size && giftCounter[random_gift] < maxGiftCount)
                    {
                        if(intIdChild < 5520){
                        gift_pos_1 = 0;
                        for(int Index_2 = 0; Index_2 < wishList_size; Index_2++){
                            gift_pos_2 = Index_2;

                            present_option = child_wish_list_2[takenIndex_2];
                            if (giftCounter[present_option] < maxGiftCount){
                                //fuck
                                if(gift_pos_1 <= gift_pos_2) break;
                                else {
                                    random_gift = present_option;
                                    break;
                                }
                            }

                            if (giftCounter[present_option] >= maxGiftCount){
                                break;
                            }

                            takenIndex_2++;
                        }
                    }

                        work_file_result << intIdChild << limiter << random_gift << endl;
                        if(intIdChild < 5520){
                            work_file_result << intIdChild_2 << limiter << random_gift << endl;
                            giftCounter[random_gift]++;
                        }
                        //counting how much of certain type of gift is left
                        giftCounter[random_gift]++;
                        progress_counter++;
                        cycling = false;
                        break;
                    }
                    //if there isn't, check other gift
                    if (takenIndex < wishList_size - 1)
                    {
                        takenIndex++;
                    }
                }

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
    unsigned short int GiftCounter[stockList_size] = {};
    vector<int> ChildGiftlist;
    string Line;
    srand(time(0));

    TempIn(Line, GiftCounter, ChildGiftlist);
}