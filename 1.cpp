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

// Функция для замены пробелов на подчеркивания при сохранении в файл
void spacesToUnderscores(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == ' ') str[i] = '_';
    }
}

// Функция для замены подчеркиваний на пробелы при чтении из файла
void underscoresToSpaces(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == '_') str[i] = ' ';
    }
}

// Функция, загружающая файл books.txt
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
        // Восстанавливаем пробелы в названиях и авторах
        underscoresToSpaces(books[bookscount].name);
        underscoresToSpaces(books[bookscount].author);
        ++bookscount;
    }

    fclose(file);
}

// Функция, записывающая в файл books.txt новую книгу
void saveBook(const char *filename, Book book)
{
    FILE *fout = fopen("books.txt", "a");

    if (fout == NULL)
    {
        printf("Ошибка при сохранении файла\n");
        return;
    }

    // Создаем копии для сохранения (заменяем пробелы на подчеркивания)
    char name_copy[100], author_copy[50];
    strcpy(name_copy, book.name);
    strcpy(author_copy, book.author);
    spacesToUnderscores(name_copy);
    spacesToUnderscores(author_copy);

    fprintf(fout, "%s %s %d %.2f\n", name_copy, author_copy, book.year, book.rating);

    fclose(fout);
}

// Функция для безопасного ввода числа
int inputNumber() {
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    return atoi(buffer);
}

// Функция для безопасного ввода дробного числа
float inputFloat() {
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    return atof(buffer);
}

// Функция ввода пользователем новой книги
void addBook()
{
    if (bookscount >= MAX_BOOKS) {
        printf("Вы прочитали максимальное количество книг (%d)!\n", MAX_BOOKS);
        return;
    }

    Book newbook;
    char buffer[100];

    printf("Введите название книги: ");
    fgets(newbook.name, sizeof(newbook.name), stdin);
    newbook.name[strcspn(newbook.name, "\n")] = '\0';

    printf("Введите автора: ");
    fgets(newbook.author, sizeof(newbook.author), stdin);
    newbook.author[strcspn(newbook.author, "\n")] = '\0';

    printf("Год прочтения: ");
    newbook.year = inputNumber();

    printf("Ваша оценка (0-10): ");
    newbook.rating = inputFloat();

    // Проверка корректности введенных данных
    if (newbook.rating < 0 || newbook.rating > 10) {
        printf("Ошибка: оценка должна быть от 0 до 10!\n");
        return;
    }

    if (newbook.year < 1000 || newbook.year > 2100) {
        printf("Ошибка: некорректный год!\n");
        return;
    }

    // Добавляем книгу в массив
    books[bookscount] = newbook;
    bookscount++;

    saveBook("books.txt", newbook);

    printf("Книга добавлена!\n");
}

// Функция отображения всех книг
void showBooks()
{
    if (bookscount == 0) {
        printf("Нет прочитанных книг\n");
        return;
    }

    printf("\n");
    printf("-------------------------------- Все книги --------------------------------\n");
    printf("Название                          Автор                     Год     Оценка\n");
    printf("---------------------------------------------------------------------------\n");
    
    for (int i = 0; i < bookscount; ++i) {
        printf("%-33s %-25s %-8d %-6.2f\n", 
            books[i].name, 
            books[i].author, 
            books[i].year, 
            books[i].rating);
    }
    printf("---------------------------------------------------------------------------\n");
    printf("Всего книг: %d\n\n", bookscount);
}

// Функция отображения статистики
void showStat() 
{
    if (bookscount == 0) {
        printf("Нет данных о прочитанных книгах\n");
        return;
    }

    printf("\n----------- Статистика -----------\n");
    printf("Количество прочитанных книг: %d\n", bookscount);

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

    printf("Средняя оценка: %.2f\n", total / bookscount);
    printf("Самая высоко оцененная книга: %s (%.2f)\n\n", books[m].name, max);
}

// Меню
void showMenu()
{
    printf("\n=== Трекер книг ===\n");
    printf("1. Добавить книгу\n");
    printf("2. Показать все книги\n");
    printf("3. Показать статистику\n");
    printf("4. Выход\n");
    printf("Выберите действие: ");
}

// Главная функция
int main()
{
    int choice = 0;
    char input[100];

    // Настройка кодировки для Windows
    #ifdef _WIN32
        system("chcp 1251 > nul");
    #endif

    loadBooks("books.txt");

    do
    {
        showMenu();
        
        // Используем fgets для ввода числа вместо scanf
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
            printf("Выход из программы.\n");
            break;
        
        default:
            printf("Неверный выбор. Введите 1-4.\n");
            break;
        }
    
    } while (choice != 4);
    
    return 0;
}