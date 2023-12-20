//Выполнено командой Кочетков А.М., Бормотов А.А. 22ВВВ3

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
#define dedList_size 184
#define maxGiftCount 250
#define good_children 110
using namespace std;

void Progress_Bar(double progress_c)
{
    int progress_b = (progress_c / 184000) * 100;
    COORD pos = { 0, 0 };

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    cout << progress_b << "%" << endl;
}

vector<vector<int>> ded_copy(ifstream& work_file_ded, vector<vector<int>> children) {
    char delimiter = ',';
    string line;
    string idChild, idGift;
    int intIdChild = 0, intIdGift = 0;

    int iterator = 0;

    while (getline(work_file_ded, line)) {
        stringstream stream(line);

        getline(stream, idChild, delimiter);
        intIdChild = stoi(idChild);
        children[iterator][0] = intIdChild; // save child id

        for (int Index = 0; Index < good_children; Index++) {
            getline(stream, idGift, delimiter);
            intIdGift = stoi(idGift);
            children[iterator][Index] = intIdGift;
        }
        iterator++;
    }

    return children;
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
    int present_option = 0;
    int temp = 0;
    int takenIndex_2 = 0;
    int intIdChild_2 = 0;
    vector<int> child_wish_list_2;
    int gift_pos_1 = 0, gift_pos_2 = 0;
    int temp_gift = 0;
    //mine for ded
    int unis = 0;
    int possible_gift = 0;
    ifstream work_file_ded(ded);

    work_file_result << "ChildId" << limiter << "GiftId" << endl;

    vector<vector<int>> children(1000, vector<int>(good_children));
    children = ded_copy(work_file_ded, children);
    work_file_ded.close();

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
        if (intIdChild < 5520) {
            //take the next line (second sibling)
            getline(work_file_kids, line);
            stringstream stream_2(line); //convert it

            getline(stream_2, idChild_2, delimiter); // we have to do it to cut off the id from the string of 100 present id's
            intIdChild_2 = stoi(idChild_2);

            for (int Index = 0; Index < wishList_size; Index++) {
                //storing all desired gifts id's into array
                getline(stream_2, idGift, delimiter);
                intIdGift = stoi(idGift);
                child_wish_list_2.push_back(intIdGift);
            }
        }
        ///
        int random_gift = 0;

        //giving gifts to children
        while (cycling)
        {
            bool best_taken = false;
            int best_gift = -1;
            for (int Index = 0; Index < wishList_size; Index++)
            {
                int gift = child_wish_list[takenIndex];
                bool suitable = 0;

                gift_pos_1 = Index;
                //if gift is in child's wishlist, 
                //then check if there is this type of gift in stock
                            //if there is, give it to child
                //if the gift is not in wishlist, 
                //but the gifts that are in it are out of stock, 
                //give child this gift
                if (giftCounter[gift] < maxGiftCount)
                {
                    if (intIdChild < 5520) {
                        for (int Index_2 = 0; Index_2 < wishList_size; Index_2++) {
                            gift_pos_2 = Index_2;

                            present_option = child_wish_list_2[takenIndex_2];
                            if (giftCounter[present_option] < maxGiftCount) {
                                //fuck
                                if (gift_pos_1 <= gift_pos_2) break;
                                else {
                                    gift = present_option;
                                    break;
                                }
                            }

                            if (giftCounter[present_option] >= maxGiftCount) {
                                break;
                            }

                            takenIndex_2++;
                        }
                    }
                    else {
                        if (!best_taken) {
                            best_gift = gift; // saving the first (left most) gift suitable for this kid
                            best_taken = true;
                        }

                        for (int j = 0; j < good_children; j++) {
                            if (children[gift][j] == intIdChild) {
                                best_gift = gift; // change the gift to better option (the gift IN the kid list AND IN ded list)
                                suitable = true;
                                break;
                            }
                        }

                        if (suitable == false && takenIndex < wishList_size - 1) {
                            takenIndex++;
                            continue;
                        }

                        takenIndex++;
                        gift = best_gift;
                    }

                    work_file_result << intIdChild << limiter << gift << endl;
                    if (intIdChild < 5520) {
                        work_file_result << intIdChild_2 << limiter << gift << endl;
                        giftCounter[gift]++;
                        progress_counter++;
                    }
                    //counting how much of certain type of gift is left
                    giftCounter[gift]++;

                    progress_counter++;
                    cycling = false;
                    break;
                }
                if (giftCounter[gift] >= maxGiftCount)
                {
                    //optimisation?
                    if (takenIndex >= wishList_size - 1 && giftCounter[random_gift] >= maxGiftCount) {
                        while (giftCounter[random_gift] >= maxGiftCount) {
                            random_gift++;
                        }
                    }
                    //
                    if (takenIndex >= wishList_size - 1 && giftCounter[random_gift] < maxGiftCount)
                    {
                        if (intIdChild < 5520) {
                            gift_pos_1 = 0;
                            for (int Index_2 = 0; Index_2 < wishList_size; Index_2++) {
                                gift_pos_2 = Index_2;

                                present_option = child_wish_list_2[takenIndex_2];
                                if (giftCounter[present_option] < maxGiftCount) {
                                    //fuck
                                    if (gift_pos_1 <= gift_pos_2) break;
                                    else {
                                        //random_gift = present_option;
                                        temp_gift = present_option;
                                        break;
                                    }
                                }

                                if (giftCounter[present_option] >= maxGiftCount) {
                                    break;
                                }

                                takenIndex_2++;
                            }
                        }

                        if (intIdChild >= 5520) temp_gift = random_gift; // put this in else{}

                        if (best_taken)
                        {
                            temp_gift = best_gift;
                        }

                        work_file_result << intIdChild << limiter << temp_gift << endl;
                        if (intIdChild < 5520) {
                            work_file_result << intIdChild_2 << limiter << temp_gift << endl;
                            giftCounter[temp_gift]++;
                            progress_counter++;
                        }
                        //counting how much of certain type of gift is left
                        giftCounter[temp_gift]++;
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

    time_t start = clock();
    TempIn(Line, GiftCounter, ChildGiftlist);
    time_t stop = clock();
    double time = (stop - start) / 1000.0;

    cout << "Time elapsed: " << time << endl;
}