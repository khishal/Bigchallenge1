#include "header.h"

LetterBucket buckets[ALPHABET_COUNT];

void initBuckets() {
    for (int i = 0; i < ALPHABET_COUNT; i++) {
        buckets[i].letter = 'a' + i;
        buckets[i].count = 0;
        buckets[i].capacity = 10;
        buckets[i].words = (WordInfo *)malloc(sizeof(WordInfo) * buckets[i].capacity);
    }
}

void freeBuckets() {
    for (int i = 0; i < ALPHABET_COUNT; i++) {
        free(buckets[i].words);
        buckets[i].count = 0;
    }
}

// Fungsi untuk membersihkan kata (lowercase, remove digits/punct)
int cleanWord(char *dest, const char *src) {
    int j = 0;
    for (int i = 0; src[i] != '\0'; i++) {
        if (isalpha(src[i])) {
            dest[j++] = tolower(src[i]);
        }
    }
    dest[j] = '\0';
    return j; // return length
}

// Menambahkan kata ke struktur data
void addWord(char *rawWord) {
    char cleaned[MAX_WORD_LEN];
    int len = cleanWord(cleaned, rawWord);

    if (len == 0) return; // Abaikan jika kosong setelah dibersihkan

    int idx = cleaned[0] - 'a';
    if (idx < 0 || idx >= 26) return; // Safety check

    // Cek apakah kata sudah ada (untuk update frekuensi)
    for (int i = 0; i < buckets[idx].count; i++) {
        if (strcmp(buckets[idx].words[i].text, cleaned) == 0) {
            buckets[idx].words[i].frequency++;
            return;
        }
    }

    // Jika kata baru, cek kapasitas array
    if (buckets[idx].count >= buckets[idx].capacity) {
        buckets[idx].capacity *= 2;
        buckets[idx].words = (WordInfo *)realloc(buckets[idx].words, sizeof(WordInfo) * buckets[idx].capacity);
    }

    // Simpan kata baru
    strcpy(buckets[idx].words[buckets[idx].count].text, cleaned);
    buckets[idx].words[buckets[idx].count].length = len;
    buckets[idx].words[buckets[idx].count].frequency = 1;
    buckets[idx].count++;
}

// Logika Sorting (Bubble Sort sesuai saran)
void sortData() {
    for (int b = 0; b < ALPHABET_COUNT; b++) {
        int n = buckets[b].count;
        if (n < 2) continue;

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                WordInfo *w1 = &buckets[b].words[j];
                WordInfo *w2 = &buckets[b].words[j+1];

                // Aturan sorting:
                // 1. Frekuensi (Descending)
                // 2. Panjang Kata (Descending)
                // 3. Lexicographical/Nilai ASCII (Descending) "alamat > alamak"
                
                int swap = 0;
                if (w2->frequency > w1->frequency) swap = 1;
                else if (w2->frequency == w1->frequency) {
                    if (w2->length > w1->length) swap = 1;
                    else if (w2->length == w1->length) {
                        if (strcmp(w2->text, w1->text) > 0) swap = 1;
                    }
                }

                if (swap) {
                    WordInfo temp = *w1;
                    *w1 = *w2;
                    *w2 = temp;
                }
            }
        }
    }
}

void processInputFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Gagal membuka file input!\n");
        exit(1);
    }

    char line[1024];
    int inside_target_tag = 0; // 1 jika di dalam <title> atau <body>
    int inside_url = 0;        // 1 jika di dalam <url>

    while (fgets(line, sizeof(line), fp)) {
        // Hapus newline
        line[strcspn(line, "\n")] = 0;

        // Cek Tag Pembuka/Penutup Sederhana
        if (strstr(line, "<title>") || strstr(line, "<body>")) {
            inside_target_tag = 1;
            // Lanjut parsing baris ini, tapi kita asumsikan tag ada di baris sendiri atau awal
            // Untuk simplifikasi, kita ambil token setelah tag jika ada di baris yang sama
            char *start = strstr(line, ">");
            if (start) strcpy(line, start + 1); 
        }
        if (strstr(line, "</title>") || strstr(line, "</body>")) {
            // Proses sisa baris sebelum tag penutup
            char *end = strstr(line, "<");
            if (end) *end = '\0';
            inside_target_tag = 0; // Set off setelah baris ini diproses (logic sederhana)
            // Note: Idealnya flag dimatikan setelah tokenisasi baris ini selesai.
            // Namun untuk project ini, kita proses baris ini, lalu matikan flag untuk baris berikutnya.
        }

        if (strstr(line, "<url>")) inside_url = 1;
        if (strstr(line, "</url>")) {
            inside_url = 0;
            continue; // Skip baris penutup url
        }

        // Jika dalam URL, abaikan
        if (inside_url) continue;

        // Jika tidak dalam target tag (<title> atau <body>), abaikan
        if (!inside_target_tag && !strstr(line, "</title>") && !strstr(line, "</body>")) continue;
        
        // Tokenisasi
        char *token = strtok(line, " \t\r\n");
        while (token != NULL) {
            // Hindari membaca tag itu sendiri sebagai kata
            if (token[0] != '<') {
                addWord(token);
            }
            token = strtok(NULL, " \t\r\n");
        }
        
        // Safety switch off jika tag penutup ditemukan di baris ini
        if (strstr(line, "</title>") || strstr(line, "</body>")) inside_target_tag = 0;
    }

    fclose(fp);
    printf("Proses pembacaan file teks selesai.\n");
    sortData(); // Urutkan data segera setelah dibaca
}
