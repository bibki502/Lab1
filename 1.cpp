#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BOOKS 1000

int bookscount = 0;

struct Book
{
    char name[100];
    char author[50];
    int year;
    float rating;
};

Book books[MAX_BOOKS];

// ������� ��� ������ �������� �� ������������� ��� ���������� � ����
void spacesToUnderscores(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == ' ') str[i] = '_';
    }
}

// ������� ��� ������ ������������� �� ������� ��� ������ �� �����
void underscoresToSpaces(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == '_') str[i] = ' ';
    }
}

// �������, ����������� ���� books.txt
void loadBooks(const char *filename)
{
    FILE *file = fopen("books.txt", "r");
    
    if (file == NULL) {
        return;
    }

    bookscount = 0;
    while (bookscount < MAX_BOOKS &&
           fscanf(file, "%99s %49s %d %f",
                  books[bookscount].name,
                  books[bookscount].author,
                  &books[bookscount].year,
                  &books[bookscount].rating) == 4)
    {
        // ��������������� ������� � ��������� � �������
        underscoresToSpaces(books[bookscount].name);
        underscoresToSpaces(books[bookscount].author);
        ++bookscount;
    }

    fclose(file);
}

// �������, ������������ � ���� books.txt ����� �����
void saveBook(const char *filename, Book book)
{
    FILE *fout = fopen("books.txt", "a");

    if (fout == NULL)
    {
        printf("������ ��� ���������� �����\n");
        return;
    }

    // ������� ����� ��� ���������� (�������� ������� �� �������������)
    char name_copy[100], author_copy[50];
    strcpy(name_copy, book.name);
    strcpy(author_copy, book.author);
    spacesToUnderscores(name_copy);
    spacesToUnderscores(author_copy);

    fprintf(fout, "%s %s %d %.2f\n", name_copy, author_copy, book.year, book.rating);

    fclose(fout);
}

// ������� ��� ����������� ����� �����
int inputNumber() {
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    return atoi(buffer);
}

// ������� ��� ����������� ����� �������� �����
float inputFloat() {
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    return atof(buffer);
}

// ������� ����� ������������� ����� �����
void addBook()
{
    if (bookscount >= MAX_BOOKS) {
        printf("�� ��������� ������������ ���������� ���� (%d)!\n", MAX_BOOKS);
        return;
    }

    Book newbook;
    char buffer[100];

    printf("������� �������� �����: ");
    fgets(newbook.name, sizeof(newbook.name), stdin);
    newbook.name[strcspn(newbook.name, "\n")] = '\0';

    printf("������� ������: ");
    fgets(newbook.author, sizeof(newbook.author), stdin);
    newbook.author[strcspn(newbook.author, "\n")] = '\0';

    printf("��� ���������: ");
    newbook.year = inputNumber();

    printf("���� ������ (0-10): ");
    newbook.rating = inputFloat();

    // �������� ������������ ��������� ������
    if (newbook.rating < 0 || newbook.rating > 10) {
        printf("������: ������ ������ ���� �� 0 �� 10!\n");
        return;
    }

    if (newbook.year < 1000 || newbook.year > 2100) {
        printf("������: ������������ ���!\n");
        return;
    }

    // ��������� ����� � ������
    books[bookscount] = newbook;
    bookscount++;

    saveBook("books.txt", newbook);

    printf("����� ���������!\n");
}

// ������� ����������� ���� ����
void showBooks()
{
    if (bookscount == 0) {
        printf("��� ����������� ����\n");
        return;
    }

    printf("\n");
    printf("-------------------------------- ��� ����� --------------------------------\n");
    printf("��������                          �����                     ���     ������\n");
    printf("---------------------------------------------------------------------------\n");
    
    for (int i = 0; i < bookscount; ++i) {
        printf("%-33s %-25s %-8d %-6.2f\n", 
            books[i].name, 
            books[i].author, 
            books[i].year, 
            books[i].rating);
    }
    printf("---------------------------------------------------------------------------\n");
    printf("����� ����: %d\n\n", bookscount);
}

// ������� ����������� ����������
void showStat() 
{
    if (bookscount == 0) {
        printf("��� ������ � ����������� ������\n");
        return;
    }

    printf("\n----------- ���������� -----------\n");
    printf("���������� ����������� ����: %d\n", bookscount);

    float max = 0;
    float total = 0;
    int m = 0;

    for (int i = 0; i < bookscount; ++i)
    {
        total = total + books[i].rating;

        if (books[i].rating > max) {
            max = books[i].rating;
            m = i;
        }
    }

    printf("������� ������: %.2f\n", total / bookscount);
    printf("����� ������ ��������� �����: %s (%.2f)\n\n", books[m].name, max);
}

// ����
void showMenu()
{
    printf("\n=== ������ ���� ===\n");
    printf("1. �������� �����\n");
    printf("2. �������� ��� �����\n");
    printf("3. �������� ����������\n");
    printf("4. �����\n");
    printf("�������� ��������: ");
}

// ������� �������
int main()
{
    int choice = 0;
    char input[100];

    // ��������� ��������� ��� Windows
    #ifdef _WIN32
        system("chcp 1251 > nul");
    #endif

    loadBooks("books.txt");

    do
    {
        showMenu();
        
        // ���������� fgets ��� ����� ����� ������ scanf
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            showBooks();
            break;
        case 3:
            showStat();
            break;
        case 4:
            printf("����� �� ���������.\n");
            break;
        
        default:
            printf("�������� �����. ������� 1-4.\n");
            break;
        }
    
    } while (choice != 4);
    
    return 0;
}