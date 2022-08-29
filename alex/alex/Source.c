#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum { ID, INT, REAL, STR, VAR, FUNCTION, IF, ELSE, WHILE, END, RETURN, COLON, SEMICOLON, LPAR, RPAR, COMMA, OR, AND, NOT, EQUAL, NOTEQUAL, LESS, ASSIGN, ADD, SUB, MUL, DIV, FINISH, TYPE_INT, TYPE_STR, TYPE_REAL};

typedef struct {
	int cod;
	int linie;
	union {
		char s[100]; //id si string
		double r; //pt real
		int i; //pt int
	 };
}Atom;

Atom atomi[10000]; //vect de atomi extrasi din fisierul de intrare
int nAtomi = 0; //nr de atomi din vect atomi
int linie = 1;

char buffin[30001];
char* pch; //cusor la caract curent din buffin
FILE* fis = NULL;

void addAtom(int a) {
	atomi[nAtomi].cod = a;
	atomi[nAtomi].linie = linie;
	nAtomi++;
}

void ERR(char* e) {
	printf("EROARE: %s", e);
	exit(0);
}

//la fiec apel ret codul unui atom
int GetNextTk() {
	int state = 0;
	char buf[100];
	int n = 0;
	for (;;) {
		int ch = *pch;
		//printf("#%d %c(%d)\n", state, ch, ch); //pt debugging 
		switch (state) {
		case 0:
			//testam toate tranzitiile posibile din acel state
			if ((isalpha(ch)) || (ch == '_')) {
				state = 1;
				pch++;
				buf[n++] = ch;
			}
			else if (isdigit(ch)) {
				buf[n++] = ch;
				state = 3;
				pch++;
			}
			else if ((ch == ' ') || (ch == '\r') || (ch == '\t') || (ch == '\n')) {
				pch++;
				if (ch == '\n')
					linie++;
			}
			else if (ch == '\"') {
				state = 9;
				pch++;
			}

			else if (ch == ':') {
				state = 11;
				pch++;
			}
			else if (ch == ';') {
				state = 12;
				pch++;
			}
			else if (ch == '(') {
				state = 13;
				pch++;
			}
			else if (ch == ')') {
				state = 14;
				pch++;
			}
			else if (ch == ',') {
				state = 15;
				pch++;
			}
			else if (ch == '!') {
				state = 20;
				pch++;
			}
			else if (ch == '<') {
				state = 26;
				pch++;
			}
			else if (ch == '+') {
				state = 27;
				pch++;
			}
			else if (ch == '-') {
				state = 28;
				pch++;
			}
			else if (ch == '*') {
				state = 29;
				pch++;
			}
			else if (ch == '/') {
				state = 30;
				pch++;
			}
			else if (ch == '\0') {
				state = 32;
				pch++;
			}
			else if (ch == '#') {
				state = 31;
				pch++;
			}
			else if (ch == '|') {
				state = 16;
				pch++;
			}
			else if (ch == '=') {
				state = 23;
				pch++;
			}
			else if (ch == '&') {
				state = 18;
				pch++;
			}
			else if (ch == EOF) {
				state = 32;
			}

			break;
		case 1:
			if ((isalnum(ch)) || (ch == '_'))
			{
				pch++;
				buf[n++] = ch;
			}
			else state = 2;
			break;
		case 2:
			buf[n] = '\0';
			if (strcmp(buf, "var") == 0) {
				addAtom(VAR); //adaugare atom in lista de atomi
				return VAR;

			}
			else if (strcmp(buf, "function") == 0) {
				addAtom(FUNCTION);
				return FUNCTION;
			}
			else if (strcmp(buf, "if") == 0) {
				addAtom(IF);
				return IF;
			}
			else if (strcmp(buf, "else") == 0) {
				addAtom(ELSE);
				return ELSE;
			}
			else if (strcmp(buf, "while") == 0) {
				addAtom(WHILE);
				return WHILE;
			}
			else if (strcmp(buf, "end") == 0) {
				addAtom(END);
				return END;
			}
			else if (strcmp(buf, "return") == 0) {
				addAtom(RETURN);
				return RETURN;
			}
			else if (strcmp(buf, "int") == 0) {
				addAtom(TYPE_INT);
				return TYPE_INT;
			}
			else if (strcmp(buf, "real") == 0) {
				addAtom(TYPE_REAL);
				return TYPE_REAL;
			}
			else if (strcmp(buf, "str") == 0){
				addAtom(TYPE_STR);
				return TYPE_STR;
		}
			else {
				addAtom(ID);
				strcpy(atomi[nAtomi - 1].s, buf);
				return ID;
			}
			atomi[nAtomi - 1].linie = linie;
			return ID;

		case 3:
			if (isdigit(ch)) {
				pch++;
				buf[n++] = ch;
			}
			else if (ch == '.') {
				pch++;
				buf[n++] = ch;
				state = 5;
			}
			else state = 4;
			break;

		case 4:
			buf[n] = '\0';
			addAtom(INT);
			atomi[nAtomi - 1].i = atoi(buf);
			return INT;

		case 5:

			if (isdigit(ch)) {
				buf[n++] = ch;
				pch++;
				state = 6;
			}
			break;

		case 6:
			if (isdigit(ch)) {
				buf[n++] = ch;
				pch++;
			}
			else state = 7;
			break;

		case 7:
			buf[n] = '\0';
			addAtom(REAL);
			atomi[nAtomi - 1].r = atof(buf);
			return REAL;

		case 8:
			//greseala la numerotarea mea pe diagrama
			break;

		case 9:
			if (ch != '\"') {
				buf[n++] = ch;
				pch++;
			}
			else {
				pch++;
				state = 10;
			}
			break;

		case 10:
			buf[n] = '\0';
			addAtom(STR);
			strcpy(atomi[nAtomi - 1].s, buf);
			return STR;

		case 11:
			addAtom(COLON);
			return COLON;

		case 12:
			addAtom(SEMICOLON);
			return SEMICOLON;

		case 13:
			addAtom(LPAR);
			return LPAR;

		case 14:
			addAtom(RPAR);
			return RPAR;

		case 15:
			addAtom(COMMA);
			return COMMA;

		case 16:
			if (ch == '|') {
				state = 17;
			}
			else ERR("Caracter nerecunoscut - un singur !");
			break;

		case 17:
			addAtom(OR);
			return OR;

		case 18:
			if (ch == '&') {
				state = 19;
			}
			else ERR("Caracter nerecunoscut - un singur &");
			break;

		case 19:
			addAtom(AND);
			return AND;

		case 20:
			if (ch == '=') {
				state = 22;
			}
			else {
				state = 21;
			}
			break;
			
		case 21:
			addAtom(NOT);
			return NOT;

		case 22:
			addAtom(NOTEQUAL);
			return NOTEQUAL;

		case 23:
			if (ch == '=') {
				state = 25;
			}
			else {
				state = 24;
			}
			break;

		case 24:
			addAtom(EQUAL);
			return EQUAL;

		case 25:
			addAtom(ASSIGN);
			return ASSIGN;

		case 26:
			addAtom(LESS);
			return LESS;

		case 27:
			addAtom(ADD);
			return ADD;

		case 28:
			addAtom(SUB);
			return SUB;

		case 29:
			addAtom(MUL);
			return MUL;

		case 30:
			addAtom(DIV);
			return DIV;

		case 31:
			if ((ch != '\r') && (ch != '\n') && (ch != '\0'))
				pch++;
			else state = 0;
			break;

		case 32:
			addAtom(FINISH);
			return FINISH;

		default: printf("\nStare invalida! %d\n",state);
		}
	}
}

const char* nume(int cod) {
	switch (cod) {
	case 0: return "ID";
	case INT: return "INT";
	case 2: return "REAL";
	case 3: return "STR";
	case 4: return "VAR";
	case 5: return "FUNCTION";
	case 6: return "IF";
	case 7: return "ELSE";
	case 8: return "WHILE";
	case 9: return "END";
	case 10: return "RETURN";
	case 11: return "COLON";
	case 12: return "SEMICOLON";
	case 13: return "LPAR";
	case 14: return "RPAR";
	case 15: return "COMMA";
	case 16: return "AND";
	case 17: return "OR";
	case 18: return "NOT";
	case 19: return "EQUAL";
	case 20: return "NOTEQUAL";
	case 21: return "LESS";
	case 22: return "ASSIGN";
	case 23: return "ADD";
	case 24: return "SUB";
	case 25: return "MUL";
	case 26: return "DIV";
	case 27: return "FINISH";
	case 28: return "TYPE_INT";
	case 29: return "TYPE_STR";
	case 30: return "TYPE_REAL";
	default: return "Eroare";
	}
}


int main() {
	FILE* fis;
	fis = fopen("1.q", "rb");
	if (fis == NULL) {
		printf("Nu s-a putut deschide fisierul!");
		return -1;
	}
	int n = fread(buffin, 1, 30000, fis); //ret nr de elem citite integral
	buffin[n] = '\0';
	fclose(fis);
	pch = buffin; //initializare pe prima pozitie
	while (GetNextTk() != FINISH) {
	}
	int j, i;
	for (j = 1; j <= linie; j++) {
		printf("\nLINIA %d:", j);
		for (i = 0; i <= nAtomi; i++) {
			if (atomi[i].linie == j) {
				if (atomi[i].cod == 1)
					printf("%s %d ", nume(atomi[i].cod), atomi[i].i);
				else if (atomi[i].cod == 2)
					printf("%s %f ", nume(atomi[i].cod), atomi[i].r);
				else
					printf("%s %s ", nume(atomi[i].cod), atomi[i].s);
			}
		}
	}
}