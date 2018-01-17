/* 
 * File:   permute.cpp
 * Author: venera-varbanova
 *
 * Created on January 17, 2018, 6:20 PM
 */

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

static void shiftFirstToBack(char* str, int len) {
    if (len < 2) return;
    char first = str[0];
    for (int i = 0; i < len - 1; i++) {
        str[i] = str[i + 1];
    }
    str[len - 1] = first;
}

static void permute(char* str, int len) {
    if (len == 0) {
        return;
    }

    if (len == 1) {
        printf("%s\n", str);
        return;
    }

    for (int i = 0; i < len; i++) {
        permute(str, len - 1);
        shiftFirstToBack(str, len);
    }
}

/*
 *  Print all permutations of a string
 */
int main(int argc, char** argv) {

    cout << "Pritinging permutations of ABC" << endl;
    char str[] = "ABC";
    permute(str, strlen(str));

    cout << "Pritinging permutations of ABCD" << endl;
    char str2[] = "ABCD";
    permute(str2, strlen(str2));

    return 0;
}

