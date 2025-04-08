#include "../include/example.hpp"
#include "../include/subtitleFactory.hpp"
#include "../include/tokenizator.hpp"

#include <clocale>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
	std::setlocale(LC_ALL, ".UTF8");
	try
	{
		if (argc < 3)
		{
			throw std::invalid_argument("Неверное количество аргументов. Ожидается 2 файла.");
		}
		string input_file_name = argv[1];
		string output_file_name = argv[2];
		std::fstream input_file(input_file_name, std::fstream::in | std::fstream::out);
		if (!input_file.is_open())
		{
			throw std::invalid_argument("Не удалось открыть входной файл: " + input_file_name);
		}
		Subtitles *subtitle = SubtitleFactory::createSubtitle(input_file_name);
		int output_token = sub_tokenization(output_file_name);
		int input_token = sub_tokenization(input_file_name);
		int op = 0;
		std::cout << "1: Считать входной файл.\n2: Завершить работу.\n";
		std::cin >> op;
		if (op == 1)
		{
			subtitle->read(input_file);
			std::cout << "Файл прочитан.\n";
		}
		else
		{
			std::cout << "Завершение работы.\n";
			delete subtitle;
			return 0;
		}
		while (true)
		{
			std::cout << "Операции:\n1: Записать в выходной файл.\n2: Удалить стили.\n3: Установить стиль по "
						 "умолчанию.\n4: Изменить дельту времени.\n5: Найти коллизии.\n6: Выход.\n";
			std::cin >> op;
			if (op == 1)
			{
				std::fstream output_file(output_file_name, std::fstream::out);
				if (!output_file.is_open())
				{
					throw std::invalid_argument("Не удалось открыть выходной файл: " + output_file_name);
				}
				subtitle->write(output_file, output_token);
				output_file.close();
				std::cout << "Данные записаны.\n";
			}
			else if (op == 2)
			{
				subtitle->removeFormatting(input_file);
				std::cout << "Стили удалены.\n";
			}
			else if (op == 3)
			{
				subtitle->set_style();
				std::cout << "Установлен стиль по умолчанию.\n";
			}
			else if (op == 4)
			{
				std::string delta;
				int time_option;
				std::cout << "Введите дельту (формат HH:MM:SS,mmm):\n";
				std::cin >> delta;
				std::cout << "Выберите опцию: 1 - start & end, 2 - start only, 3 - end only:\n";
				std::cin >> time_option;
				subtitle->change_delta(time_option, delta);
			}
			else if (op == 5)
			{
				Subtitles *collisions = subtitle->find_collisions();
				std::fstream temp("collisions.txt", std::fstream::out);
				collisions->write(temp, input_token);
				temp.close();
				std::cout << "Коллизии записаны в collisions.txt.\n";
				delete collisions;
			}
			else if (op == 6)
			{
				break;
			}
			else
			{
				std::cout << "Неверная операция.\n";
			}
		}
		delete subtitle;
	} catch (const std::exception &e)
	{
		std::cerr << "Ошибка: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
