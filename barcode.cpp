#include<bitset>
#include<vector>
#include<iostream>
#include <raylib.h>
#include<unordered_map>
using namespace std;


unordered_map <char, string> code128B = {
{' '    ,"212222"},
{'!'    ,"222122"},
{'\"'   ,"222221"},
{'#'    ,"121223"},
{'$'    ,"121322"},
{'%'    ,"131222"},
{'&'    ,"122213"},
{'\''   ,"122312"},
{'('    ,"132212"},
{')'    ,"221213"},
{'*'    ,"221312"},
{'+'    ,"231212"},
{','    ,"112232"},
{'-'    ,"122132"},
{'.'    ,"122231"},
{'/'    ,"113222"},
{'0'    ,"123122"},
{'1'    ,"123221"},
{'2'    ,"223211"},
{'3'    ,"221132"},
{'4'    ,"221231"},
{'5'    ,"213212"},
{'6'    ,"223112"},
{'7'    ,"312131"},
{'8'    ,"311222"},
{'9'    ,"321122"},
{':'    ,"321221"},
{';'    ,"312212"},
{'<'    ,"322112"},
{'='    ,"322211"},
{'>'    ,"212123"},
{'?'    ,"212321"},
{'@'    ,"232121"},
{'A'    ,"111323"},
{'B'    ,"131123"},
{'C'    ,"131321"},
{'D'    ,"112313"},
{'E'    ,"132113"},
{'F'    ,"132311"},
{'G'    ,"211313"},
{'H'    ,"231113"},
{'I'    ,"231311"},
{'J'    ,"112133"},
{'K'    ,"112331"},
{'L'    ,"132131"},
{'M'    ,"113123"},
{'N'    ,"113321"},
{'O'    ,"133121"},
{'P'    ,"313121"},
{'Q'    ,"211331"},
{'R'    ,"231131"},
{'S'    ,"213113"},
{'T'    ,"213311"},
{'U'    ,"213131"},
{'V'    ,"311123"},
{'W'    ,"311321"},
{'X'    ,"331121"},
{'Y'    ,"312113"},
{'Z'    ,"312311"},
{'['    ,"332111"},
{'\\'   ,"314111"},
{']'    ,"221411"},
{'^'    ,"431111"},
{'_'    ,"111224"},
{'`'    ,"111422"},
{'a'    ,"121124"},
{'b'    ,"121421"},
{'c'    ,"141122"},
{'d'    ,"141221"},
{'e'    ,"112214"},
{'f'    ,"112412"},
{'g'    ,"122114"},
{'h'    ,"122411"},
{'i'    ,"142112"},
{'j'    ,"142211"},
{'k'    ,"241211"},
{'l'    ,"221114"},
{'m'    ,"413111"},
{'n'    ,"241112"},
{'o'    ,"134111"},
{'p'    ,"111242"},
{'q'    ,"121142"},
{'r'    ,"121241"},
{'s'    ,"114212"},
{'t'    ,"124112"},
{'u'    ,"124211"},
{'v'    ,"411212"},
{'w'    ,"421112"},
{'x'    ,"421211"},
{'y'    ,"212141"},
{'z'    ,"214121"},
{'{'    ,"412121"},
{'|'    ,"111143"},
{'}'    ,"111341"},
{'~'    ,"131141"}};
vector<int> startseq={2,1,1,2,1,4};
vector<int> stopseq={2,3,3,1,1,1,2};

int calcCheckSum(const string& text){
    int sum=104;
    int multiplier=1;
    for(int i=0;i<text.length();i++){
        sum+=multiplier*(static_cast<int>(text[i])-32);
        multiplier++;
    }
    sum=sum%103;
    return sum;
}

vector<int> extractWidths(char c){
    vector<int>arr(0);
    string num=code128B[c];
    
    for(int i=0;i<num.length();i++){
        arr.push_back(num[i]-'0');
    }
    return arr;
}
void app(vector<int>& inital,const vector<int>& final){
    inital.insert(inital.end(),final.begin(),final.end());
    
}
vector<int> allWidths(const string& buffer){
    vector<int> arr(0);
    for(int i=0;i<buffer.length();i++){
        app(arr,extractWidths(buffer[i]));
    }
    return arr;
}
void Draw(const vector<int> &widths){
    int pos_X=0;
    bool colour = true;
    double factor=2;
    
    DrawRectangle(pos_X, 0, 10*factor, 200, WHITE);
    pos_X+=10*factor;
    for(int i=0;i<widths.size();i++){
        DrawRectangle(pos_X, 0, widths[i]*factor, 200, colour ? BLACK : WHITE);
        pos_X+=widths[i]*factor;
        colour=!colour;
    }
    DrawRectangle(pos_X, 0, 10*factor, 200, WHITE);
}



int main(){
    string text;
    cin>>text;
    vector<int> complete=startseq;
    vector<int> arr=allWidths(text);
    app(complete,arr);
    app(complete,extractWidths(static_cast<char>(calcCheckSum(text)+32)));
    app(complete,stopseq);

    InitWindow(1000, 200, "Barcode");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);
        Draw(complete);
        EndDrawing();
    }
   

    CloseWindow();
}

