#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
	char nume[30];
	unsigned int an;
	unsigned int note[10];
	float medie;
	struct student* next;
};
struct student* prim = NULL;
int COUNT = 0;

void creare_lista() {
	int optiune = -1;
	if (prim != NULL) {
		puts("Lista nu este goala! Doriti sa o stergeti? 0-NU / 1-DA");
		scanf_s("%d", &optiune);
	}
	if (optiune == 1) {
		prim = NULL;
	}
}

void afisare(int ok) {
	puts("LISTA STUDENTILOR\n");
	struct student* q;
	q = prim;
	if (!prim)
	{
		printf("Lista de studenti este vida!");
	}
	else {
		while (q) {
			if (ok == 1 || ok == 4)
				printf("%s, student/a in anul:%d, are media %f", q->nume, q->an, q->medie);
			else
				if (ok == 3)
					printf("%-15s   media %f", q->nume, q->medie);
			if (ok == 1) {
				printf("\n");
				for (int i = 0; i < 10; i++) {
					printf("Curs #%2d: nota:%d\n", i + 1, *(q->note + i));
				}
			}
			printf("\n");
			q = q->next;
		}
	}
}

void afisare_anul_3() {
	struct student* p;
	int ok = 0;
	puts("LISTA STUDENTI DIN ANUL 3 CU MEDIA > 8.00\n");
	for (p = prim; p; p = p->next) {
		if (p->an == 3 && p->medie >= 8.00) {
			ok = 1;
			printf("%-15s, student/a in anul %d, are media generala %f\n", p->nume, p->an, p->medie);
		}
	}
	if (!ok) {
		printf("Nu exista studenti din anul 3 cu media > 8.00\n");
	}
}

struct student* creare_nod(char nume[], int an, int note_stud[]) {
	struct student* nod;
	int total = 0;
	nod = (struct student*)malloc(sizeof(struct student));
	if (!nod) {
		printf("Eroare la alocare");
		exit(EXIT_FAILURE);
	}
	strcpy(nod->nume, nume);
	nod->an = an;
	for (int i = 0; i < 10; i++) {
		nod->note[i] = note_stud[i];
		total += nod->note[i];
	}
	nod->medie = total / 10.0;
}

struct student* inregistrare_inceput(char nume[], int an, int note_stud[]) {
	COUNT++;
	struct student* nod = creare_nod(nume, an, note_stud);
	nod->next = prim;
	prim = nod;
	return prim;
}

struct student* inregistrare_sfarsit(char nume[], int an, int note_stud[]) {
	COUNT++;
	struct student* nod = creare_nod(nume, an, note_stud);
	nod->next = NULL;
	if (prim == NULL)
		return nod;
	struct student* p;
	for (p = prim; p->next; p = p->next);
	p->next = nod;
	nod->next = NULL;
	return prim;
}

struct student* inregistrare_dupa_k(char nume[], int an, int k, int note_stud[]) {
	COUNT++;
	struct student* p;
	struct student* nod = creare_nod(nume, an, note_stud);
	int count = 0;
	if (prim == NULL && k == 0) {
		prim = inregistrare_inceput(nume, an, note_stud);
	}
	else {
		for (p = prim; p && count != k; p = p->next) {
			count++;
		}
		if (p != NULL) {
			nod->next = p->next;
			p->next = nod;
			return prim;
		}
	}
}

void citire_date() {
	int an, k, ok = 0, note[10], option;
	char* end, buf[40], nume[40];
	printf("Nume:");
	fgets(nume, 40, stdin);
	fgets(nume, 40, stdin);
	nume[strlen(nume) - 1] = 0;
	printf("An:");
	//oblig user-ul sa citeasca aici un numar
	do {
		if (!fgets(buf, sizeof buf, stdin)) {
			break;
		}
		buf[strlen(buf) - 1] = 0;
		an = strtol(buf, &end, 10);
		if (an >= 1 && an <= 6)
			ok = 1;
	} while (end != buf + strlen(buf) || !ok);
	printf("Note:\n");
	for (int i = 0; i < 10; i++) {
		printf("Nota pentru cursul #%d :", i + 1);
		scanf_s("%d", &note[i]);
		while (note[i] <= 0 || note[i] > 10) {
			printf("Nota adaugata nu e valida! Cititi din nou o nota de la 1-10\n");
			printf("Nota pentru cursul #%d :", i + 1);
			scanf_s("%d", &note[i]);
		}
	}
	if (COUNT == 0)
		prim = inregistrare_inceput(nume, an, &note);
	else
	{
		printf("Dupa care student doriti sa-l introduceti pe cel nou?\n");
		scanf_s("%d", &k);
		if (k == 0)
			prim = inregistrare_inceput(nume, an, &note);
		else
			if (k > COUNT) {
				do {
					puts("Doriti sa-l adaugati la inceput(0) sau la sfarsit(1)?");
					if (scanf_s("%d", &option) != 1) {
						printf("Optiune gresita! Aceasta nu este un numar");
						return EXIT_FAILURE;
					}
					switch (option) {
					case 0:
						prim = inregistrare_inceput(nume, an, &note);
						break;
					case 1:
						prim = inregistrare_sfarsit(nume, an, &note);
						break;
					}
				} while (option != 0 && option != 1);

			}
			else {
				prim = inregistrare_dupa_k(nume, an, k - 1, &note);
			}
	}
}

struct student* eliminare_nepromovati() {
	struct student* p = prim, * aux = NULL;

	//daca apare la inceputul listei o medie < 5.00
	while (p && p->medie < 5.00) {  //ne oprim la p cu media <5.00
		prim = p->next;
		free(p);
		COUNT--;
		p = prim;
	}

	//daca apare in interiorul listei
	while (p) {
		while (p && p->medie >= 5.00) {
			aux = p;
			p = p->next;
		}
		if (p == NULL) //nu s-a gasit student cu media mai mica decat 5.00
			return prim;
		aux->next = p->next;
		free(p);
		COUNT--;
		p = aux->next;
	}
	return prim;
}

struct student* mergeSortedList(struct student* lista1, struct student* lista2) {
	struct student* rezultat = NULL;

	if (lista1 == NULL)
		return lista2;
	else if (lista2 == NULL)
		return lista1;
	if ((lista1->medie > lista2->medie) || (((lista1->medie == lista2->medie) && (strcmp(lista1->nume, lista2->nume) < 0))))
	{
		rezultat = lista1;
		rezultat->next = mergeSortedList(lista1->next, lista2);
	}
	else {
		rezultat = lista2;
		rezultat->next = mergeSortedList(lista1, lista2->next);
	}
	return rezultat;
}

void splitList(struct student* source, struct student** front, struct student** back) {
	struct student* p, * q;
	q = source;
	p = source->next;   //parcurge lista de 2x mai rapid

	while (p) {
		p = p->next;
		if (p) {
			q = q->next;
			p = p->next;
		}
	}
	*front = source;  //pointer la lista 1
	*back = q->next;  //q->next pointeaza spre mijlocul listei	(lista 2)
	q->next = NULL;  //stergem referinta catre "a doua parte" a listei
}


void sortare(struct student** thead) {
	struct student* prim = *thead;
	struct student* p, * q;
	if ((prim == NULL) || (prim->next == NULL)) {
		return;
	}

	splitList(prim, &p, &q);
	sortare(&p);
	sortare(&q);
	*thead = mergeSortedList(p, q);
}

void meniu() {
	puts("\n");
	puts("1. Creare lista studenti");
	puts("2. Afisare lista studenti");
	puts("3. Afisare nume si medie generala");
	puts("4. Afisare in ordine descrescatoare a mediei");
	puts("5. Afisare studenti in anul 3 care au media peste 8.00");
	puts("6. Inregistrare student nou");
	puts("7. Eliminare studenti nepromovati (sub 5.00)");
	puts("8. Iesire");
	puts("Introduceti optiunea: ");
	puts("\n");
}

int main() {
	int option;
	do {
		meniu();
		if (scanf_s("%d", &option) != 1) {
			printf("Optiune gresita! Aceasta nu este un numar");
			return EXIT_FAILURE;
		}
		switch (option) {
		case 1:
			creare_lista();
			break;
		case 2:
			afisare(1);
			break;
		case 3:
			afisare(3);
			break;
		case 4:
			sortare(&prim);
			afisare(4);
			break;
		case 5:
			afisare_anul_3(5);
			break;
		case 6:
			citire_date();
			break;
		case 7:
			prim = eliminare_nepromovati();
			break;
		case 8:
			printf("EXIT\n");
			exit(0);
			break;
		default:
			puts("\nOptiunea aleasa nu e valida! Introduceti alta optiune\n");
		}

	} while (option != 8);
}



