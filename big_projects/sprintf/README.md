Условия работы:

Написать программу, содержащую функцию, позволяющую преобразовать числа в текстовой записи из одной системы счисления в другую с дополнительным форматированием. Использовать стандартные функции можно, но на защите вы должны быть способны быстро реализовать используемые библиотечные функции вручную (соответственно, нельзя использовать функции подобные sprintf, т.к. быстро их написать невозможно). Преобразуемое число 128-битное со знаком в формате дополнение до 2 (two’s component).
Число может быть в 16, 10, 8 или 2 системе счисления. Система счисления определяется префиксом по правилам С.
Перед числом может быть символ '-', который следует интерпретировать строго как "инвертировать все биты входного числа и добавить 1", что полезно для ввода отрицательных чисел (но не единственный способ получить отрицательное число). Буквы на входе могут быть любого регистра.
Форматирование выходного числа задаётся форматом, аналогичным используемому в printf. Подробнее: printf, snprintf, ... - cppreference.com.
Поддерживаемые спецификаторы конверсии: 'b', 'o', 'd', 'x', 'X'. Поддерживаемые флаги: ' ' (пробел), '-', '+', '#', '0', field width. 
Выходное число во всех системах счисления – со знаком (в отличие от поведения в С для недесятичных систем счисления).
'#' при 'd', должен игнорироваться, но не для других систем счисления.
Требования к реализации
Прототип функции, которую нужно реализовать в программе:
void print(char *out_buf, const char *format, const char *number);
Для вывода полученного результата нужно использовать puts(out_buf);
	Гарантируется, что длина выходной строки не превышает 255.
Аргументы программе передаются через командную строку:
<format> <number>
	Пример вызова программы с аргументами: lab2.exe "%d" "0" 
В стандартный поток вывода должна быть записана строка результата с символом перевода строки в конце.
Если на вход программе поданы некорректные данные (формат без % или неподдерживаемый формат), то необходимо завершаться с человекочитаемым сообщением о возникшей ошибке (по-английски) и кодом 1.
