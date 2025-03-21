#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

int n;


int main(){
    cin >> n;
    int mas[n + 1];
    mas[1] = 0;
    for(int i = 2; i <= n; i ++){
        mas[i] = mas[i - 1] + 1;
        if (i % 2 == 0) mas[i] = min(mas[i], mas[i / 2] + 1);
        if(i % 3 == 0) mas[i] = min(mas[i], mas[i / 3] + 1);
    }
    cout << mas[n] << "\n";
    int mm[mas[n] + 1];
    int j = mas[n] - 1;
    mm[j + 1] = n;
    for(int i = n; i > 1 && j >= 0; ){
        if(i % 3 == 0 && mas[i] == mas[i / 3] + 1){
            mm[j] = i / 3;
            i = i  / 3;
        }
        else{
            if(i % 2 == 0 && mas[i] == mas[i / 2] + 1){
                mm[j] = i / 2;
                i = i / 2;
            }
            else{
                mm[j] = i - 1;
                i = i - 1;
            }
        }
        j--;

    }
    for(int i = 0; i < mas[n] + 1; i++){
        cout << mm[i] << " ";
    }
    return 0;
}
