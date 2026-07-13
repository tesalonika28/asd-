#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HARGA_PER_KG 8000 

// Struktur node linked list - TEPAT 4 FIELD DATA UTAMA
typedef struct node {
    int id;           // 1. Field ID
    char nama[50];    // 2. Field Nama
    float berat;      // 3. Field Berat
    char status[20];  // 4. Field Status
    struct node *next;
} node;

const char FILE_NAME[] = "Laundry.txt";

// ==================== PROTEKSI INPUT ====================

float inputFloat(char* prompt) {
    char input[100];      
    float value;         
    char ekstra;         
    while (1) {          
        printf("%s", prompt);   
        if (fgets(input, sizeof(input), stdin) != NULL) {  
            if (sscanf(input, "%f %c", &value, &ekstra) == 1) { 
                if (value > 0) {
                    return value; 
                } else {
                    printf("[ERROR] Input tidak valid. Berat laundry harus lebih dari 0 kg!\n");
                }
            } else {
                printf("[ERROR] Input tidak valid. Silahkan masukkan angka yang benar!\n");
            }
        }
    }
}

void inputString(char* prompt, char* str, int size) {  
    while (1) {                               
        printf("%s", prompt);              
        if (fgets(str, size, stdin) != NULL) {
            str[strcspn(str, "\n")] = '\0';
            
            if (strlen(str) == 0) {            
                printf("[ERROR] Input tidak boleh kosong!\n");
                continue;
            }

            int adaSpasi = 0;
            for (int i = 0; i < strlen(str); i++) {
                if (isspace(str[i])) {
                    adaSpasi = 1;
                    break;
                }
            }

            if (adaSpasi) {
                printf("[ERROR] Input tidak boleh menggunakan spasi! Gunakan '_' sebagai pemisah (Contoh: Budi_Santoso).\n");
                continue;
            }

            break; 
        }
    }
}

void toLowerCase(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = tolower(src[i]);
        i++;
    }
    dest[i] = '\0';
}

// ==================== MANAJEMEN LINKED LIST ====================

void pushBack(node** head, int id, char nama[], float berat, char status[]) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->id = id;
    strcpy(newNode->nama, nama);
    newNode->berat = berat;
    strcpy(newNode->status, status);
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

int increment() {
    FILE* file = fopen(FILE_NAME, "r");
    if (file == NULL) return 1;
    int id, maxId = 0;
    char nama[50], status[20];
    float berat;
    while (fscanf(file, "%d %49s %f %19s", &id, nama, &berat, status) == 4) {
        if (id > maxId) maxId = id;
    }
    fclose(file);
    return maxId + 1;
}

void freeList(node** head) {
    while (*head != NULL) {
        node* temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

void swapNodeData(node* a, node* b) {
    int tempId = a->id;
    char tempNama[50];
    strcpy(tempNama, a->nama);
    float tempBerat = a->berat;
    char tempStatus[20];
    strcpy(tempStatus, a->status);

    a->id = b->id;
    strcpy(a->nama, b->nama);
    a->berat = b->berat;
    strcpy(a->status, b->status);

    b->id = tempId;
    strcpy(b->nama, tempNama);
    b->berat = tempBerat;
    strcpy(b->status, tempStatus);
}

node* loadFromFile() {
    FILE* file = fopen(FILE_NAME, "r");
    if (file == NULL) return NULL;

    node* head = NULL;
    int id;
    char nama[50], status[20];
    float berat;

    while (fscanf(file, "%d %49s %f %19s", &id, nama, &berat, status) == 4) {
        pushBack(&head, id, nama, berat, status);
    }
    fclose(file);
    return head;
}

void printTable(node* head) {
    if (head == NULL) {
        printf("\nTidak ada data untuk ditampilkan.\n");
        return;
    }
    printf("\n=========================================================================\n");
    printf("%-5s %-8s %-20s %-12s %-12s %-15s\n", "No", "ID", "Nama Pelanggan", "Berat (Kg)", "Status", "Total Biaya");
    printf("=========================================================================\n");
    
    node* temp = head;
    int no = 1;
    while (temp != NULL) {
        float totalBiaya = temp->berat * HARGA_PER_KG;
        printf("%-5d %-8d %-20s %-12.2f %-12s Rp %-12.0f\n", 
               no++, temp->id, temp->nama, temp->berat, temp->status, totalBiaya);
        temp = temp->next; 
    }
    printf("=========================================================================\n");
}

// Fungsi pembantu: hitung berapa banyak node dengan nama tertentu
int spotduplicate(node* head, char* nama) {
    int count = 0;
    node* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->nama, nama) == 0) count++;
        temp = temp->next;
    }
    return count;
}

// Fungsi pembantu: tampilkan semua data dengan nama tertentu, lalu minta user pilih ID
// Mengembalikan ID yang dipilih user, atau -1 jika tidak valid
int idduplicated(node* head, char* nama) {
    node* temp = head;
    node* hasilHead = NULL;

    while (temp != NULL) {
        if (strcmp(temp->nama, nama) == 0) {
            pushBack(&hasilHead, temp->id, temp->nama, temp->berat, temp->status);
        }
        temp = temp->next;
    }

    printf("\n[INFO] Ditemukan lebih dari satu data dengan nama '%s'. Silahkan pilih berdasarkan ID:\n", nama);
    printTable(hasilHead);
    freeList(&hasilHead);

    char idInput[10];
    int idDipilih;
    while (1) {
    inputString("Masukkan ID data yang ingin diproses (atau 0 untuk batal): ", idInput, sizeof(idInput));
    idDipilih = atoi(idInput);

    if (idDipilih == 0) return -1; // opsi batal manual

    temp = head;
    while (temp != NULL) {
        if (temp->id == idDipilih && strcmp(temp->nama, nama) == 0) {
            return idDipilih;
        }
        temp = temp->next;
    }
    printf("[ERROR] ID tidak ditemukan atau tidak sesuai nama tersebut. Coba lagi.\n");
}
}

// ==================== OPERASI CRUD ====================

void createLaundry() {
    char nama[50];
    float berat;

    FILE* file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        printf("Gagal membuka database file!\n");
        return;
    }

    int id = increment(); 
    printf("\nID Pelanggan Baru : %d\n", id);
    inputString("Masukkan Nama Pelanggan : ", nama, sizeof(nama));
    berat = inputFloat("Masukkan Berat (kg)     : ");
    char status[] = "Diproses";

    fprintf(file, "%d %s %.2f %s\n", id, nama, berat, status);
    fclose(file);
    printf("=> [SUKSES] Data Berhasil Ditambahkan ke File Database.\n");
}

void readLaundry() {
    node* head = loadFromFile();
    if (head == NULL) {
        printf("\nBelum ada data laundry atau file tidak ditemukan.\n");
        return;
    }
    printTable(head);
    freeList(&head); 
}

// [U] - UPDATE DATA (sudah mendukung pilih ID kalau nama duplikat)
void editLaundry() {
    node* head = loadFromFile();
    if (head == NULL) {
        printf("\nBelum ada data laundry untuk di-edit.\n");
        return; 
    }

    char cari[50];
    inputString("\nMasukkan nama pelanggan yang ingin di-edit statusnya: ", cari, sizeof(cari));

    int jumlah = spotduplicate(head, cari);

    if (jumlah == 0) {
        printf("[INFO] Data atas nama '%s' tidak ditemukan.\n", cari);
        freeList(&head);
        return;
    }

    int idTarget = -1;
    if (jumlah > 1) {
        idTarget = idduplicated(head, cari);
        if (idTarget == -1) {
            freeList(&head);
            return;
        }
    }

    node* temp = head;
    int ketemu = 0;

    while (temp != NULL) {
        int cocok = (jumlah > 1)
            ? (temp->id == idTarget && strcmp(temp->nama, cari) == 0)
            : (strcmp(temp->nama, cari) == 0);

        if (cocok) {
            printf("Data ditemukan! Status saat ini: %s\n", temp->status);
            printf("Pilih Status Baru (1. Diproses / 2. Selesai): ");

            char pilihanInput[10];
            while (1) {
                inputString("", pilihanInput, sizeof(pilihanInput));
                if (pilihanInput[0] == '1') { strcpy(temp->status, "Diproses"); break; }
                else if (pilihanInput[0] == '2') { strcpy(temp->status, "Selesai"); break; }
                else printf("[ERROR] Pilihan tidak valid! Ketik 1 atau 2.\n");
            }

            ketemu = 1;
            printf("Status Berhasil Diperbarui.\n");
            break; 
        }
        temp = temp->next;
    }

    if (ketemu) {
        FILE* tulisKembali = fopen(FILE_NAME, "w"); 
        node* curr = head;
        while (curr != NULL) {
            fprintf(tulisKembali, "%d %s %.2f %s\n", curr->id, curr->nama, curr->berat, curr->status);
            curr = curr->next;
        }
        fclose(tulisKembali);
        printf("[SUKSES] File database berhasil diperbarui.\n");
    } else {
        printf("[INFO] Data atas nama '%s' tidak ditemukan.\n", cari);
    }

    freeList(&head); 
}

// [D] - DELETE DATA (sudah mendukung pilih ID kalau nama duplikat)
void deleteLaundry() {
    node* head = loadFromFile();
    if (head == NULL) {
        printf("\nBelum ada data laundry untuk dihapus.\n");
        return;
    }

    char hapus[50];
    inputString("\nMasukkan nama pelanggan yang ingin dihapus: ", hapus, sizeof(hapus));

    int jumlah = spotduplicate(head, hapus);

    if (jumlah == 0) {
        printf("[INFO] Data atas nama '%s' tidak ditemukan.\n", hapus);
        freeList(&head);
        return;
    }

    int idTarget = -1;
    if (jumlah > 1) {
        idTarget = idduplicated(head, hapus);
        if (idTarget == -1) {
            freeList(&head);
            return;
        }
    }

    node* temp = head;
    node* prev = NULL;
    int ketemu = 0;

    while (temp != NULL) {
        int cocok = (jumlah > 1)
            ? (temp->id == idTarget && strcmp(temp->nama, hapus) == 0)
            : (strcmp(temp->nama, hapus) == 0);

        if (cocok) {
            ketemu = 1;
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }
            printf("Data ID %d atas nama '%s' berhasil dihapus dari RAM.\n", temp->id, hapus);
            free(temp); 
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (ketemu) {
        FILE* tulisKembali = fopen(FILE_NAME, "w");
        node* curr = head;
        while (curr != NULL) {
            fprintf(tulisKembali, "%d %s %.2f %s\n", curr->id, curr->nama, curr->berat, curr->status);
            curr = curr->next;
        }
        fclose(tulisKembali);
        printf("[SUKSES] File database berhasil diperbarui.\n");
    } else {
        printf("[INFO] Data atas nama '%s' tidak ditemukan.\n", hapus);
    }

    freeList(&head); 
}

// ==================== FITUR BARU: SEARCHING & SORTING ====================

void searchLaundry() {
    node* head = loadFromFile();
    if (head == NULL) {
        printf("\nBelum ada data laundry untuk dicari.\n");
        return;
    }

    char keyword[50];
    inputString("\nMasukkan nama pelanggan yang dicari: ", keyword, sizeof(keyword));

    char keywordLow[50];
    toLowerCase(keywordLow, keyword);

    node* temp = head;
    node* searchResultHead = NULL;
    int ketemu = 0;

    while (temp != NULL) {
        char namaLow[50];
        toLowerCase(namaLow, temp->nama);

        if (strstr(namaLow, keywordLow) != NULL) {
            pushBack(&searchResultHead, temp->id, temp->nama, temp->berat, temp->status);
            ketemu = 1;
        }
        temp = temp->next;
    }

    if (ketemu) {
        printf("\n[HASIL PENCARIAN] Ditemukan data dengan keyword '%s':", keyword);
        printTable(searchResultHead);
    } else {
        printf("\n[INFO] Data dengan keyword nama '%s' tidak ditemukan.\n", keyword);
    }

    freeList(&head);
    freeList(&searchResultHead);
}

void sortLaundry() {
    node* head = loadFromFile();
    if (head == NULL) {
        printf("\nBelum ada data laundry untuk diurutkan.\n");
        return;
    }

    char menuSort[10];
    printf("\n--- PILIHAN SORTING DATA ---");
    printf("\n1. Urutkan Berdasarkan ID (Urut Naik / Ascending)");
    printf("\n2. Urutkan Berdasarkan BERAT (Urut Turun / Descending)");
    inputString("\nPilih jenis sorting (1-2): ", menuSort, sizeof(menuSort));
    
    if (menuSort[0] != '1' && menuSort[0] != '2') {
    printf("[ERROR] Pilihan sorting tidak valid! Harus 1 atau 2.\n");
    freeList(&head);
    return;
}

    if (head->next == NULL) {
        printTable(head);
        freeList(&head);
        return;
    }

    int swapped;
    node* ptr1;
    node* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            int butuhTukar = 0;

            if (menuSort[0] == '1') {
                if (ptr1->id > ptr1->next->id) {
                    butuhTukar = 1;
                }
            } else if (menuSort[0] == '2') {
                if (ptr1->berat < ptr1->next->berat) {
                    butuhTukar = 1;
                }
            }

            if (butuhTukar) {
                swapNodeData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    printf("\n[SUKSES] Data Berhasil Diurutkan.:");
    printTable(head);
    freeList(&head); 
}

// ==================== MAIN MENU PROGRAM ====================
int main() {
    char menuInput[10];
    int pilihan;

    do {
        printf("\n=== SISTEM MANAJEMEN LAUNDRY ===");
        printf("\n1. Tambah Transaksi (Create)");
        printf("\n2. Lihat Semua Transaksi (Read)");
        printf("\n3. Edit Status Laundry (Update)");
        printf("\n4. Hapus Transaksi (Delete)");
        printf("\n5. Cari Transaksi (Searching - Nama)");
        printf("\n6. Urutkan Transaksi (Sorting - 2 Atribut)");
        printf("\n7. Keluar");
        inputString("\nPilih menu (1-7): ", menuInput, sizeof(menuInput));
        
        pilihan = atoi(menuInput); 

        switch (pilihan) {
            case 1: createLaundry(); break;
            case 2: readLaundry(); break;
            case 3: editLaundry(); break;
            case 4: deleteLaundry(); break;
            case 5: searchLaundry(); break;
            case 6: sortLaundry(); break;
            case 7: printf("\nTerima kasih! Program laundry selesai.\n"); break;
            default: printf("\n[ERROR] Pilihan tidak valid! Masukkan angka antara 1 sampai 7.\n");
        }
    } while (pilihan != 7);

    return 0;
}