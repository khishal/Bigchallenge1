#include "header.h"

// Nama file binary default
#define BIN_FILE "output.bin"

int main(int argc, char *argv[]) {
    // Cek Command Line Argument
    if (argc < 2) {
        printf("Usage: %s <nama_file_input.txt>\n", argv[0]);
        return 1;
    }

    char *inputFile = argv[1];
    int choice;
    int dataSaved = 0; // Flag untuk memastikan poin 1 dijalankan sebelum poin 2

    initBuckets();
    
    // Langkah 1-5: Baca dan proses file teks saat program mulai
    printf("Membaca file: %s ...\n", inputFile);
    processInputFile(inputFile);

    do {
        // Tampilan Menu
        printf("\n==================================\n");
        printf(" MENU PROJECT BIG CHALLENGE\n");
        printf("==================================\n");
        printf("1) Simpan luaran dalam file binari\n");
        printf("2) Tampilkan di layar monitor maksimum n buah kata\n");
        printf("3) Selesai\n");
        printf("Pilihan anda: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Simpan ke binary
                saveToBinary(BIN_FILE);
                dataSaved = 1;
                break;

            case 2: {
                int n;
                printf("Masukkan nilai n (0 < n <= 25): ");
                scanf("%d", &n);
                if (n <= 0 || n > 25) {
                    printf("Nilai n tidak valid!\n");
                    break;
                }

                // Jika belum disimpan (Poin 1), simpan dulu lalu baca balik
                if (!dataSaved) {
                    printf("Perhatian: Data belum disimpan ke binary. Menyimpan otomatis...\n");
                    saveToBinary(BIN_FILE);
                    dataSaved = 1;
                }

                // Wajib baca dari binary sebelum tampil
                readFromBinary(BIN_FILE);
                displayTopN(n);
                break;
            }

            case 3:
                printf("Program selesai.\n");
                break;

            default:
                printf("Pilihan tidak valid.\n");
        }

    } while (choice != 3);

    freeBuckets();
    return 0;
}
