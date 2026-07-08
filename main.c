#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HARGA_PER_KG 8000 //fungsi konstanta harga perkilo laundry
// steruktur node linked list
typedef struct {
    int id;           //menandakan id pelanggan
    char nama[50];    //menandakan nama pelanggan
    float berat;      //berat laundry pelanggan
    char status[20];  //menandakan status pengerjaan laundry pelanggan
    float total;
    struct node *next;
}node;

//fungsi input bertipe float
float inputFloat(char* prompt) {
    char input[100];      //menyipan input sementara dalam bentuk string
    float value;         //variabel menyimpan hasil konversi string ke float 
    char ekstra;         //variabel mendekteksi karakter
    while (1) {          //pengulangan ketika input valid
        printf("%s", prompt);   //menampilkan baris input jika input valid 
        if (fgets(input, sizeof(input), stdin) != NULL) {  //membaca input dari keyboard
            if (sscanf(input, "%f %c", &value, &ekstra) ==1) { //membaca float jika hnay 1 float maka input valid
                return value; //mengembalikan nilai float
            }else{
                printf("input tidak valid. silahkan masukkam angka desimal!\n");
            }
            }
        }
 }

 //fungsi untuk input string untuk validasi
 void inputstring(char* prompt, char* str, int size) {
    while (1) {
        printf("%s", prompt);
        fgets(str, size, stdin);            // Membaca input string dari keyboard
        str[strcspn(str, "\n")] = '\0';      // Mengecek apakah string tidak kosong
        if (strlen(str) > 0) {               // Keluar dari perulangan jika input sudah valid
            break;
        }
        printf("input tidak valid. silahkan masukkan string yang valid!\n");
    }
 }

