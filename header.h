#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define ALPHABET_COUNT 26

// Struktur untuk menyimpan data kata
typedef struct {
    char text[MAX_WORD_LEN];
    int length;
    int frequency;
} WordInfo;

// Struktur untuk mengelompokkan kata berdasarkan abjad
typedef struct {
    char letter;
    int count;          // Jumlah kata unik dalam bucket ini
    int capacity;       // Kapasitas array dinamis
    WordInfo *words;    // Array of words
} LetterBucket;

// Variabel Global (extern agar bisa diakses file lain)
extern LetterBucket buckets[ALPHABET_COUNT];

// Prototipe Fungsi
void initBuckets();
void freeBuckets();
void processInputFile(const char *filename);
void addWord(char *rawWord);
void sortData(); 
void saveToBinary(const char *binFilename);
void readFromBinary(const char *binFilename);
void displayTopN(int n);

#endif
