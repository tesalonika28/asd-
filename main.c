#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HARGA_PER_KG 8000 // fungsi konstanta harga perkilo laundry

// Struktur node linked list
typedef struct node {
    int id;           // menandakan id pelanggan
    char nama[50];    // menandakan nama pelanggan
    float berat;      // berat laundry pelanggan
    char status[20];  // menandakan status pengerjaan laundry pelanggan
    float total;
    struct node *next;
} node;

const char FILE_NAME[] = "Laundry.txt";

// Fungsi input bertipe float
float inputFloat(char* prompt) {
    char input[100];      // menyimpan input sementara dalam bentuk string
    float value;          // variabel menyimpan hasil konversi string ke float 
    char ekstra;          // variabel mendeteksi karakter
    while (1) {           // pengulangan ketika input valid
        printf("%s", prompt);   // menampilkan baris input jika input valid 
        if (fgets(input, sizeof(input), stdin) != NULL) {  // membaca input dari keyboard
            if (sscanf(input, "%f %c", &value, &ekstra) == 1) { // membaca float jika hanya 1 float maka input valid
                return value; // mengembalikan nilai float
            } else {
                printf("input tidak valid. silahkan masukkan angka desimal!\n");
            }
        }
    }
}

// Fungsi proteksi input (validasi string yang kosong)
void inputString(char* prompt, char* str, int size) {  
    while (1) {                             // pengulangan input ketika valid
        printf("%s", prompt);              // menampilkan baris input jika valid
        fgets(str, size, stdin);          // membaca input string dari keyboard
        str[strcspn(str, "\n")] = '\0';   // mengecek apakah string Tidak kosong
        if (strlen(str) > 0) {            // keluar dari perulangan jika input sudah valid 
            break;
        }
        printf("input tidak valid. silahkan masukkan string yang valid!\n");
    }
}

// Fungsi buat masukin data dari file ke linkedlist paling belakang
void pushBack(node** head, int id, char nama[], float berat, char status[], float total) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->id = id;
    strcpy(newNode->nama, nama);
    newNode->berat = berat;
    strcpy(newNode->status, status);
    newNode->total = total;
    newNode->next = NULL;

    // memasukan node baru ke dalam list utama
    if (*head == NULL) {
        *head = newNode; //Kalo head atau start = kosong, maka node yang baru adalah head
    } else {
        node* temp = *head; // Kalo udah ada isinya, pake temp bantuan buat jalan selangkah lebih depan
        while (temp->next != NULL) {
            temp = temp->next;  // ngegeser pointer temp ke list berikutnya
        }
        temp->next = newNode; // Kalo node udah mentok (Keisi semua data yang dibutuhkan), bakal pindah ke newnode (node baru) tapi tetap terhubung dari node-node sebelumnya
    }
}

// fungsi buat ngambil ID list terakhir dari file
int increment() {
    FILE* file = fopen(FILE_NAME, "r"); // Ngebuka file dengan mode read
    if (file == NULL) return 1; // Kalo filenya kosong maka bakal ngembaliin angka 1
    // Deklarasi variabel sementara buat nampung pas sistem ngebaca
    int id, lastId = 0;
    char nama[50], status[20];
    float berat, total;

    // Looping buat ngebaca seluruh baris file sampe akhir
    while (fscanf(file, "%d %49s %f %19s %f", &id, nama, &berat, status, &total) == 5) {
        lastId = id;  //Variabel last id bakal terus ditimpa dengan yang dibaca kemudian menyimpan ID yang ditemukan paling akhir
    }
    fclose(file);
    return lastId + 1; // Ngembaliin last id + 1 buat inputan pelanggan baru
}

// Fungsi buat menghapus memory linked list di RAM
void freeList(node** head) {
    while (*head != NULL) {
        node* temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

// == CRUD ygy ==

// ini create
void createLaundry() {
    char nama[50];
    float berat;
    
    // Ngebuka file dalam mode append lalu proteksi jika file tidak dapat diakses
    FILE* file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    // Proses input laundry dengan id otomatis urut
    int id = increment(); // ID otomatis urut
    printf("ID Pelanggan : %d\n", id);
    inputString("Masukkan Nama Pelanggan: ", nama, sizeof(nama));
    berat = inputFloat("Masukkan Berat (kg)    : ");
    
    float total = berat * HARGA_PER_KG;
    char status[] = "Diproses";

    // Simpan ke file teks menggunakan spasi sebagai pemisah
    fprintf(file, "%d %s %f %s %f\n", id, nama, berat, status, total);
    fclose(file);
    printf("Data Berhasil Ditambahkan.\n");
}

// ini read, intinya mindahin data dari file ke RAM, trus dicetak deh
void readLaundry() {
    FILE* file = fopen(FILE_NAME, "r"); // Seperti biasa ngebuka file dalam mode read sekarang dengan ada proteksi
    if (file == NULL) {
        printf("Belum ada data laundry atau file tidak ditemukan.\n");
        return;
    }

    node* head = NULL;
    int id;
    char nama[50], status[20];
    float berat, total;

    // Load data dari file dimasukkan ke Linked List dulu (Wajib Linked List)
    while (fscanf(file, "%d %49s %f %19s %f", &id, nama, &berat, status, &total) == 5) {
        pushBack(&head, id, nama, berat, status, total);
    }
    fclose(file);

    if (head == NULL) {
        printf("Belum ada data laundry.\n");
        return;
    }

    printf("\n=========================================================================\n");
    printf("%-5s %-8s %-20s %-12s %-12s %-15s\n", "No", "ID", "Nama Pelanggan", "Berat (Kg)", "Status", "Total Biaya");
    printf("=========================================================================\n");
    
    node* temp = head;
    int no = 1;
    while (temp != NULL) {
        printf("%-5d %-8d %-20s %-12.2f %-12s Rp %-12.0f\n", no++, temp->id, temp->nama, temp->berat, temp->status, temp->total);
        temp = temp->next; // Turun ke node berikutnya
    }
    printf("=========================================================================\n");

    freeList(&head); // Bersihkan memori RAM
}

// update yhahaha
void editLaundry() {
    FILE* file = fopen(FILE_NAME, "r"); // Buka file untuk read
    if (file == NULL) {
        printf("Belum ada data laundry atau file tidak ditemukan.\n");
        return;
    } // ngecek kalo file ngga ada bakal printf itu

    // 1. LOAD DATA DARI FILE KE RAM (LINKED LIST)
    node* head = NULL; // pointer mulai dari awal sampe ketemu null
    int id;
    char nama[50], status[20];
    float berat, total;

    while (fscanf(file, "%d %49s %f %19s %f", &id, nama, &berat, status, &total) == 5) {
        pushBack(&head, id, nama, berat, status, total); // Ngebaca data lalu memasukannya ke dalam ram
    }
    fclose(file); // Setelah memasukannya ke ram, filenya ditutup kemudian proses selanjutnya langsung dari ram buat ringan

    if (head == NULL) {
        printf("Belum ada data laundry untuk di-edit.\n");
        return; // ngecek kalo langsung ketemu NULL
    }

    // 2. PROSES SEARCH & UPDATE DI DALAM RAM
    char cari[50];
    inputString("Masukkan nama pelanggan yang ingin di-edit statusnya: ", cari, sizeof(cari));

    node* temp = head;
    int ketemu = 0;

    while (temp != NULL) {
        if (strcmp(temp->nama, cari) == 0) {
            printf("Data ditemukan! Status saat ini: %s\n", temp->status);
            printf("Pilih Status Baru (1. Diproses / 2. Selesai): ");
            // Ngecek jika temp yang ditemukan bukan NULL maka baru bisa edit

            char pilihanInput[10];
            inputString("", pilihanInput, sizeof(pilihanInput)); 
            
            // Ubah data langsung di alamat RAM-nya
            if (pilihanInput[0] == '2') strcpy(temp->status, "Selesai");
            else strcpy(temp->status, "Diproses");

            ketemu = 1;
            printf("Status Berhasil Diperbarui di RAM.\n");
            break; // Keluar dari loop jika sudah ketemu
        }
        temp = temp->next;
    }

    // 3. JIKA ADA PERUBAHAN, TULIS KEMBALI SELURUH LINKED LIST KE FILE (SINKRONISASI)
    if (ketemu) {
        FILE* tulisKembali = fopen(FILE_NAME, "w"); // Mode "w" menghapus isi lama, diganti yang baru
        node* curr = head;
        while (curr != NULL) {
            fprintf(tulisKembali, "%d %s %f %s %f\n", curr->id, curr->nama, curr->berat, curr->status, curr->total);
            curr = curr->next;
        }
        fclose(tulisKembali);
        printf("[SUKSES] File database berhasil diperbarui.\n");
    } else {
        printf("Data atas nama '%s' tidak ditemukan.\n", cari);
    }

    freeList(&head); // Bersihkan memori RAM
}

// ini delete y
void deleteLaundry() {
    FILE* file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Belum ada data laundry atau file tidak ditemukan.\n");
        return;
    }

    // 1. LOAD DATA DARI FILE KE RAM (LINKED LIST)
    node* head = NULL;
    int id;
    char nama[50], status[20];
    float berat, total;

    while (fscanf(file, "%d %49s %f %19s %f", &id, nama, &berat, status, &total) == 5) {
        pushBack(&head, id, nama, berat, status, total);
    }
    fclose(file);

    if (head == NULL) {
        printf("Belum ada data laundry untuk dihapus.\n");
        return;
    }

    // 2. PROSES SEARCH & DELETE DI DALAM RAM (MUTUSIN RANTAI POINTER)
    char hapus[50];
    inputString("Masukkan nama pelanggan yang ingin dihapus: ", hapus, sizeof(hapus));

    node* temp = head;
    node* prev = NULL;
    int ketemu = 0;

    while (temp != NULL) {
        if (strcmp(temp->nama, hapus) == 0) {
            ketemu = 1;
            
            if (prev == NULL) {
                // Kasus 1: Yang dihapus ternyata node pertama (Head)
                head = temp->next;
            } else {
                // Kasus 2: Yang dihapus ada di tengah atau di ujung akhir
                prev->next = temp->next;
            }
            
            free(temp); // Hapus dari memori RAM
            printf("Data atas nama '%s' berhasil dihapus dari RAM.\n", hapus);
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    // 3. TULIS KEMBALI SISA LINKED LIST YANG MASIH ADA KE FILE
    if (ketemu) {
        FILE* tulisKembali = fopen(FILE_NAME, "w");
        node* curr = head;
        while (curr != NULL) {
            fprintf(tulisKembali, "%d %s %f %s %f\n", curr->id, curr->nama, curr->berat, curr->status, curr->total);
            curr = curr->next;
        }
        fclose(tulisKembali);
        printf("[SUKSES] File database berhasil diperbarui.\n");
    } else {
        printf("Data atas nama '%s' tidak ditemukan.\n", hapus);
    }

    freeList(&head); // Bersihkan sisa memori RAM
}
