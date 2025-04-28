#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_sali 100
#define max_rezervari 250

typedef struct {
    int numar;
    int capacitate;
    char facilitati[60];
    float pret;
} Sala_eveniment;

typedef struct {
    int numar_sala;
    char data[20];
    char client[50];
} Rezervare_sala;

Sala_eveniment sali[max_sali] = {
    {1, 30, "Parcare, Bar", 150.00},
    {2, 50, "WiFi, Parcare", 200.00},
    {3, 100, "WiFi, Aer conditionat", 350.00},
    {4, 200, "WiFi, Parcare, Aer conditionat", 500.00}
};

int numar_rezervari = 0;
Rezervare_sala rezervari[max_rezervari];

void afiseaza_rezervari();
void citire_rezervari_din_fisier();
void scriere_rezervari_in_fisier();
void rezervare_sala();
void anuleaza_rezervare();
void sali_disponibile();
void cautare_sala();
void clear_screen();
void afisare_meniu();
void revenire_la_meniu();

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void afisare_meniu() {
    printf("\n\033[1;34m%20s\033[0m\n", "=== Meniu ===");  // Albastru
    printf("\033[1;33m1.\033[0m Vizualizarea salilor disponibile\n");
    printf("\033[1;33m2.\033[0m Cautare sala\n");
    printf("\033[1;33m3.\033[0m Rezervare sala\n");
    printf("\033[1;33m4.\033[0m Anulare rezervare\n");
    printf("\033[1;33m5.\033[0m Afisarea rezervarilor\n");
    printf("\033[1;33m6.\033[0m Iesire\n");
    printf("\033[1;36mAlegeti o optiune:\033[0m ");
}

void sali_disponibile() {
    printf("\033[1;34m===== SALI DISPONIBILE =====\033[0m\n");
    for (int i = 0; i < 4; i++) {
        printf("\033[1;32mNumar sala:\033[0m %d, \033[1;32mCapacitate:\033[0m %d, \033[1;32mFacilitati:\033[0m %s, \033[1;32mPret:\033[0m %.2f\n",
               sali[i].numar, sali[i].capacitate, sali[i].facilitati, sali[i].pret);
    }
}

void cautare_sala() {
    printf("\033[1;34m===== CAUTARE SALA =====\033[0m\n");
    int capacitate_minima;
    char facilitati_dorite[60];

    printf("\033[1;36mIntroduceti capacitatea minima pentru eveniment:\033[0m ");
    scanf("%d", &capacitate_minima);
    printf("\033[1;36mIntroduceti facilitatile dorite:\033[0m ");
    scanf(" %[^\n]", facilitati_dorite);
    printf("\033[1;35mSali corespunzatoare optiunilor alese:\033[0m\n");

    int gasit = 0;
    for (int i = 0; i < 4; i++) {
        if (sali[i].capacitate >= capacitate_minima && strstr(sali[i].facilitati, facilitati_dorite)) {
            printf("\033[1;32mNumar sala:\033[0m %d, \033[1;32mCapacitate:\033[0m %d, \033[1;32mFacilitati (ex:WiFi, Parcare):\033[0m %s, \033[1;32mPret:\033[0m %.2f\n",
                   sali[i].numar, sali[i].capacitate, sali[i].facilitati, sali[i].pret);
            gasit = 1;
        }
    }
    if (!gasit) {
        printf("\033[1;31mNu s-au gasit sali care sa corespunda cerintelor!\033[0m\n");
    }
}

void citire_rezervari_din_fisier() {
    FILE *file = fopen("rezervari.txt", "r");
    if (file == NULL) {
        printf("\033[1;31mNu s-a putut deschide fisierul de rezervari.\033[0m\n");
        return;
    }

    while (fscanf(file, "%d,%19[^,],%49[^\n]", &rezervari[numar_rezervari].numar_sala, rezervari[numar_rezervari].data, rezervari[numar_rezervari].client) == 3) {
        numar_rezervari++;
    }
    fclose(file);
}

void scriere_rezervari_in_fisier() {
    FILE *file = fopen("rezervari.txt", "w");
    if (file == NULL) {
        printf("\033[1;31mNu s-a putut deschide fisierul de rezervari pentru scriere.\033[0m\n");
        return;
    }

    for (int i = 0; i < numar_rezervari; i++) {
        fprintf(file, "%d,%s,%s\n", rezervari[i].numar_sala, rezervari[i].data, rezervari[i].client);
    }
    fclose(file);
}

void rezervare_sala() {
    printf("\033[1;34m===== REZERVARE SALA =====\033[0m\n");
    if (numar_rezervari >= max_rezervari) {
        printf("\033[1;31mNu se poate rezerva, numar maxim de rezervari atins.\033[0m\n");
        return;
    }

    int numar_sala;
    printf("\033[1;36mIntroduceti numarul salii:\033[0m ");
    scanf("%d", &numar_sala);
    int gasit = 0;
    for (int i = 0; i < 4; i++) {
        if (sali[i].numar == numar_sala) {
            gasit = 1;
            break;
        }
    }
    if (!gasit) {
        printf("\033[1;31mSala cu acest numar nu exista!\033[0m\n");
        return;
    }

    printf("\033[1;36mIntroduceti numele clientului:\033[0m ");
    scanf(" %[^\n]", rezervari[numar_rezervari].client);
    printf("\033[1;36mIntroduceti data rezervarii (zz.ll.aaaa):\033[0m ");
    scanf(" %[^\n]", rezervari[numar_rezervari].data);
    rezervari[numar_rezervari].numar_sala = numar_sala;
    numar_rezervari++;
    scriere_rezervari_in_fisier();
    printf("\033[1;32mRezervare efectuata cu succes!\033[0m\n");
    afiseaza_rezervari();
}

void anuleaza_rezervare() {
    printf("\033[1;34m===== ANULARE REZERVARE =====\033[0m\n");
    if (numar_rezervari == 0) {
        printf("\033[1;31mNu exista rezervari de anulat.\033[0m\n");
        return;
    }

    char client[50], perioada[20];
    printf("\033[1;36mIntroduceti numele clientului:\033[0m ");
    scanf(" %[^\n]", client);
    printf("\033[1;36mIntroduceti perioada (zz.ll.aaaa):\033[0m ");
    scanf(" %[^\n]", perioada);
    for (int i = 0; i < numar_rezervari; i++) {
        if (strcmp(rezervari[i].client, client) == 0 && strcmp(rezervari[i].data, perioada) == 0) {
            for (int j = i; j < numar_rezervari - 1; j++) {
                rezervari[j] = rezervari[j + 1];
            }
            numar_rezervari--;
            scriere_rezervari_in_fisier();
            printf("\033[1;32mRezervare anulata cu succes!\033[0m\n");
            afiseaza_rezervari();
            return;
        }
    }
    printf("\033[1;31mRezervarea nu a fost gasita!\033[0m\n");
}

void afiseaza_rezervari() {
    printf("\033[1;34m===== AFISARE REZERVARI =====\033[0m\n");
    if (numar_rezervari == 0) {
        printf("\033[1;31mNu exista rezervari.\033[0m\n");
        return;
    }

    printf("\033[1;36mRezervari existente:\033[0m\n");
    for (int i = 0; i < numar_rezervari; i++) {
        printf("\033[1;32mSala:\033[0m %d, \033[1;32mClient:\033[0m %s, \033[1;32mData:\033[0m %s\n",
               rezervari[i].numar_sala, rezervari[i].client, rezervari[i].data);
    }
}

void revenire_la_meniu() {
    printf("\n\033[1;36mApasati ENTER pentru a reveni la meniul principal...\033[0m\n");
    #ifdef _WIN32
        system("pause");
    #else
        getchar();
        getchar();
    #endif
    clear_screen();
}

int main() {
    citire_rezervari_din_fisier();
    int optiune;
    do {
        afisare_meniu();
        scanf("%d", &optiune);
        switch (optiune) {
            case 1:
                sali_disponibile();
                revenire_la_meniu();
                break;
            case 2:
                cautare_sala();
                revenire_la_meniu();
                break;
            case 3:
                rezervare_sala();
                revenire_la_meniu();
                break;
            case 4:
                anuleaza_rezervare();
                revenire_la_meniu();
                break;
            case 5:
                afiseaza_rezervari();
                revenire_la_meniu();
                break;
            case 6:
                clear_screen();
                printf("\033[1;32mIesire din program.\033[0m\n");
                break;
            default:
                printf("\033[1;31mOptiune invalida! Introduceti o optiune valida.\033[0m\n");
        }
    } while (optiune != 6);
    return 0;
}
