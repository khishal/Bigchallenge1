#include "header.h"

// Menyimpan ke file biner
void saveToBinary(const char *binFilename) {
    FILE *fp = fopen(binFilename, "wb");
    if (!fp) {
        printf("Gagal membuat file binary.\n");
        return;
    }

    // Format: [abjad][jumlah_kata] loop([panjang][kata][frek])
    for (int i = 0; i < ALPHABET_COUNT; i++) {
        // Tulis Abjad
        fwrite(&buckets[i].letter, sizeof(char), 1, fp);
        // Tulis Jumlah kata
        fwrite(&buckets[i].count, sizeof(int), 1, fp);

        for (int j = 0; j < buckets[i].count; j++) {
            WordInfo *w = &buckets[i].words[j];
            // Tulis Panjang
            fwrite(&w->length, sizeof(int), 1, fp);
            // Tulis Kata (sesuai panjangnya)
            fwrite(w->text, sizeof(char), w->length, fp); 
            // Tambah null terminator untuk keamanan saat baca (opsional, tapi spesifikasi bilang [kata] saja)
            // Kita ikuti spec ketat: hanya tulis karakter sebanyak length.
            
            // Tulis Frekuensi
            fwrite(&w->frequency, sizeof(int), 1, fp);
        }
    }

    fclose(fp);
    printf("Data berhasil disimpan ke '%s'\n", binFilename);
}

// Membaca dari file biner
void readFromBinary(const char *binFilename) {
    FILE *fp = fopen(binFilename, "rb");
    if (!fp) {
        printf("File binary belum ada! Silakan simpan (Pilihan 1) terlebih dahulu.\n");
        return;
    }

    // Reset buckets memori saat ini agar yakin data berasal dari file
    freeBuckets();
    initBuckets();

    for (int i = 0; i < ALPHABET_COUNT; i++) {
        char abjad;
        int jumlah_kata;

        fread(&abjad, sizeof(char), 1, fp);
        fread(&jumlah_kata, sizeof(int), 1, fp);

        buckets[i].letter = abjad;
        buckets[i].count = jumlah_kata;
        
        // Alokasi memori sesuai jumlah kata di file
        buckets[i].capacity = jumlah_kata + 1;
        buckets[i].words = (WordInfo *)malloc(sizeof(WordInfo) * buckets[i].capacity);

        for (int j = 0; j < jumlah_kata; j++) {
            int p_kata, frek;
            
            fread(&p_kata, sizeof(int), 1, fp);
            buckets[i].words[j].length = p_kata;

            // Baca kata
            fread(buckets[i].words[j].text, sizeof(char), p_kata, fp);
            buckets[i].words[j].text[p_kata] = '\0'; // Tambah null terminator manual

            fread(&frek, sizeof(int), 1, fp);
            buckets[i].words[j].frequency = frek;
        }
    }

    fclose(fp);
    printf("Data berhasil dimuat ulang dari file binary.\n");
}

// Menampilkan Top N Data
void displayTopN(int n) {
    printf("\n--- Hasil Top %d Kata per Abjad ---\n", n);
    for (int i = 0; i < ALPHABET_COUNT; i++) {
        // Tampilkan format: abjad { (kata (freq)), ... }
        
        int limit = (buckets[i].count < n) ? buckets[i].count : n;
        
        for (int j = 0; j < limit; j++) {
            printf(" (%s (%d))", buckets[i].words[j].text, buckets[i].words[j].frequency);
            if (j < limit - 1) printf(",");
        }
        
        // Jika tidak ada kata
        if (buckets[i].count == 0) {
            // Kosong, atau bisa tulis pesan khusus jika mau. Format contoh hanya { }
        }
        
        printf(" }\n");
    }
}
