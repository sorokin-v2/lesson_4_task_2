﻿#include <iostream>
#include <fstream>
#include <string>



class Address {

public:
    void set_city(std::string city) { this->city = city; }
    void set_street(std::string street) { this->street = street; }
    void set_house_num(std::string house_num) { this->house_num = house_num; }
    void set_apart_num(int apart_num) { this->apart_num = apart_num; }
    std::string get_city() { return city; }
    //std::string get_street() { return street; }
    //std::string get_house_num() { return house_num; }
    //int get_apart_num() { return apart_num; }
    std::string get_full_address()
    {
        return city + ", " + street + ", " + house_num + ", " + std::to_string(apart_num);
    }

    Address(std::string new_city = "Не указан", std::string new_street = "Не указана", std::string new_house_num = "Не указан", int new_apart_num = 0) :
        city{ new_city }, street{ new_street }, house_num{ new_house_num }, apart_num{ new_apart_num } {}   //Чтобы можно было создать массив объектов нужен конструктор по-умолчанию


private:

    std::string city;
    std::string street;
    std::string house_num;
    int apart_num{ 0 };

};

bool read_address(std::fstream& fin, std::string& city, std::string& street, std::string& house_num, int& apart_num)
{
    while (!fin.eof())
    {
        if ((fin >> city) && (fin >> street) && (fin >> house_num) && (fin >> apart_num)) return true;
        else return false;
    }
    return false;
}

void sort(Address* addresses, int size)
{
    //Пузырьковая сортировка
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size - 1; j++)
        {
            if (addresses[j].get_city() > addresses[j + 1].get_city())
            {
                Address tmp = addresses[j];
                addresses[j] = addresses[j + 1];
                addresses[j + 1] = tmp;
            }
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    const std::string in_file_name = "in.txt";
    std::fstream fin(in_file_name, std::fstream::in);
    if (fin.is_open())
    {
        const int min_addr_count = 1;       //Минимальное количество адресов в файле
        int addr_count = 0;
        if (fin >> addr_count)
        {
            if (addr_count < min_addr_count)
            {
                std::cout << "В файле указано количество адресов " << addr_count << " что меньше минимального " << min_addr_count << std::endl;
                return 1;
            }
            std::string city, street, house_num;
            int apart_num = 0;
            Address* addr_list = new Address[addr_count];
            for (int i = 0; i < addr_count; ++i)                //Читаем адреса из файла
            {
                if (read_address(fin, city, street, house_num, apart_num))
                {
                    addr_list[i].set_city(city);
                    addr_list[i].set_street(street);
                    addr_list[i].set_house_num(house_num);
                    addr_list[i].set_apart_num(apart_num);
                }
                else
                {
                    std::cout << "Ошибка чтения " << i << " ого/его адреса" << std::endl;
                    delete[] addr_list;
                    addr_list = nullptr;
                    fin.close();
                    return 2;
                }
            }
            fin.close();
            //Сортируем массив
            sort(addr_list, addr_count);
            //Выводми адреса в выходной файл
            const std::string out_file_name = "out.txt";
            std::fstream fout(out_file_name, std::fstream::out);
            if (fout.is_open())
            {
                if (fout << addr_count << std::endl)
                {
                    for (int i = 0 ; i < addr_count; ++i)
                    {
                        if (!(fout << addr_list[i].get_full_address() << std::endl))
                        {
                            std::cout << "Ошибка записи " << i << "(ого/его) адреса в файл " << out_file_name << std::endl;
                            delete[] addr_list;
                            addr_list = nullptr;
                            fout.close();
                            return 3;
                        }
                    }
                    fout.close();
                }
                else
                {
                    std::cout << "Ошибка записи количества адресов в файл " << out_file_name << std::endl;
                    delete[] addr_list;
                    addr_list = nullptr;
                    fout.close();
                    return 4;
                }
            }
            else
            {
                std::cout << "Ошибка создания выходного файла " << out_file_name << std::endl;
                delete[] addr_list;
                addr_list = nullptr;
                return 5;
            }

            delete[] addr_list;
            addr_list = nullptr;
        }
        else
        {
            std::cout << "Ошибка чтения количества адресов из файла " << in_file_name << std::endl;
            return 6;
        }
    }
    else
    {
        std::cout << "Ошибка открытия файла " << in_file_name << std::endl;
        return 7;
    }
}

