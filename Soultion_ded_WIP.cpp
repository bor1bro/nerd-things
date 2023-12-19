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
#define dedList_size 184
#define maxGiftCount 250
using namespace std;

// calling ded_search kills perfomanse
// and it's also completely not working at this state

void Progress_Bar(double progress_c)
{
    int progress_b = (progress_c / 184000) * 100;
    COORD pos = { 0, 0 };

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    cout << progress_b << "%" << endl;
}

// returns index. range: [0;184] 0 is best. 
// returns 185 if no kid id is present in given gift line
int ded_search(ifstream &work_file_ded ,int gift, int main_child_id){
    //ifstream work_file_ded(ded);
    int ded_gift_id = 0, sub_child_id = 0;
    char delimiter = ',';
    string line;
    string ded_gift;

/// THIS LITERALLY KILLS ANY REASANOBLE PERFOMANCE
/// IT'LL PROLY TAKE 24 HOURS AT THIS POINT....
    //find the line with our current gift
    for(int i = 0; i <= gift; ++i){
        getline(work_file_ded, line);  
    }
    stringstream stream_two(line);
    getline(stream_two, ded_gift, delimiter);
    ded_gift_id = stoi(ded_gift); // all this just to extract and discard the first gift id (it's not needed, but have to be discarded)
   
    //get all kids id's into one vector for easier access
    // I'll omit storing it into vector for now and just work with id's right away
   for (int Index = 0; Index < dedList_size; Index++){
        // I'll try using 'line' here, hope it won't break anything
        getline(stream_two, line, delimiter);
        sub_child_id = stoi(line);

        if(sub_child_id == main_child_id){
            work_file_ded.seekg(0);
            return Index;
        }
    }

    work_file_ded.seekg(0); // return cursor back to the start of the file
    return 185;

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
        int random_gift = 0;
        //giving gifts to children
        while (cycling)
        {
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

                    //for now I'll use this if() to not mess up things
                    //I'll include here many operations asumming that we can only enter this if() if we already have the correct gift to output...
                    if(intIdChild >= 5520){
                        if (intIdChild == 5540) {
                            cout << "1";
                        } 
                        possible_gift = gift;
                        int quality = ded_search(work_file_ded, gift, intIdChild);
                        unis = quality + Index; // calculating how good the choosen present is. '0' - is the best

                        //takenIndex++;
                        for(Index; Index < wishList_size; Index++){
                            gift = child_wish_list[takenIndex];
                            if (giftCounter[gift] < maxGiftCount){
                                quality = ded_search(work_file_ded, gift, intIdChild);

                                //if the new option has better unis value
                                if ( (quality + Index) < unis){
                                    possible_gift = gift;
                                    unis = quality + Index;
                                }
                            }
                            takenIndex++;
                        }

                        // we put back the value (best gift id) and break out of the if() statemnet to output the value into the result file
                        gift = possible_gift;
                    }

                    work_file_result << intIdChild << limiter << gift << endl;
                    if(intIdChild < 5520){
                        work_file_result << intIdChild_2 << limiter << gift << endl;
                        giftCounter[gift] ++;
                        progress_counter ++;
                    }
                    //counting how much of certain type of gift is left
                    giftCounter[gift]++;
                    cout << "jepa";
                    progress_counter ++;
                    cycling = false;
                    break;
                }
                if (giftCounter[gift] >= maxGiftCount)
                {
//optimisation?
                    if (takenIndex >= wishList_size && giftCounter[random_gift] >= maxGiftCount){
                        while(giftCounter[random_gift] >= maxGiftCount){
                            random_gift++;
                        }
                    }
//
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
                                        //random_gift = present_option;
                                        temp_gift = present_option;
                                        break;
                                    }
                                }

                                if (giftCounter[present_option] >= maxGiftCount){
                                    break;
                                }

                                takenIndex_2++;
                            }
                        }

                        if(intIdChild >= 5520) temp_gift = random_gift; // put this in else{}

                        work_file_result << intIdChild << limiter << temp_gift << endl;
                        if(intIdChild < 5520){
                            work_file_result << intIdChild_2 << limiter << temp_gift << endl;
                            giftCounter[temp_gift] ++;
                            progress_counter ++;
                        }
                        //counting how much of certain type of gift is left
                        giftCounter[temp_gift]++;
                        progress_counter ++;
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