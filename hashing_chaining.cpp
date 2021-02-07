#include <iostream>
#include <list>
#include <cstring>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

class HashTable {

    private:
        static const int hashGroups = 50;
        list<pair<int, string>> table[hashGroups];

    public:
        int getTableSize();
        bool isEmpty() const;
        int hashFunction(int key);
        void insertItem(int key, string value);
        void removeItem(int key, string value);
        void searchItem(int key, string value);
        void printTable();
        void loadFactor(int size);
        vector<string> readCSV();
};

int HashTable::getTableSize()
{
    return hashGroups;
}

bool HashTable::isEmpty() const 
{
    int sum;
    for (size_t i {}; i < hashGroups; i++)
    {
        sum += table[i].size();
    }
    
    if(!sum)
    {
        return true;
    }
    return false;
}

int HashTable::hashFunction(int key)
{
    return key % hashGroups;
}

void HashTable::insertItem(int key, string value)
{
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto bItr = begin(cell);
    bool keyExists = false;

    for (; bItr != end(cell); bItr ++)
    {
        if(bItr->first == key)
        {
            keyExists = true;
            bItr->second = value;
            cout << "Valor ya existente, reemplazado con exito!." << endl;
            break;
        }
    }

    if(!keyExists)
    {
        cell.emplace_back(key, value);
    }
    return;  
}

void HashTable::removeItem(int key, string value)
{

    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto bItr = begin(cell);
    bool keyExists = false;

    for (; bItr != end(cell); bItr ++)
    {
        if(bItr->second == value)
        {
            keyExists = true;
            bItr = cell.erase(bItr);
            cout << "Dato eliminado." << endl;
            break;
        }
    }

    if(!keyExists)
    {
        cout << "Valor no encontrado, no se ha eliminado el conjunto." << endl;
    }

    return;
}

void HashTable::searchItem(int key, string value)
{
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto bItr = begin(cell);
    bool keyExists = false;

    for (; bItr != end(cell); bItr ++)
    {
        if(bItr->first == key)
        {
            keyExists = true;
            cout << "\nDato con key " << bItr->first << " y Valor " << bItr->second << " encontrado!" << endl;
            break;
        }
    }

    if(!keyExists)
    {
        cout << " Valor no encontrado, no se ha encontrado el conjunto." << endl;
    }

    return;
}

void HashTable::printTable()
{
    for(int i {}; i < hashGroups; i++)
    {
        if(table[i].size() == 0) continue;

        auto bItr = table[i].begin();
        for (; bItr != table[i].end(); bItr++)
        {
            cout << "Key: " << bItr->first << "   Valor: " << bItr->second << endl;
        }
    }

    return;
}

void HashTable::loadFactor(int size)
{
    std::cout << "\nEl factor de carga de lista es de " << (float)size / 50 << std::endl;
}


vector<string> HashTable::readCSV()
{
    std::ifstream file ("listaActores.csv");

    std::vector<std::string> data;

    std::string input;

    while(file >> input)
    {
        data.push_back(input);
    }

    return data;
}

int main()
{
    HashTable HT;
    vector<string> data = HT.readCSV();
    int table_size = HT.getTableSize();

    // srand(std::time(nullptr) + rand());
    auto start = high_resolution_clock::now();
    
    for (size_t i = 0; i < table_size; i++)
    {
        // Se usa 2 veces la funcion rand para exceder el valor maximo MAX_RAND
        // Y así evitar sobreescribir datos
        HT.insertItem(rand() % 3000 + 0, data[i]);
    }

    auto stop = high_resolution_clock::now();

    cout << "\nLa insercion de datos a la tabla se produjo en "
              << duration_cast<microseconds>(stop - start).count()
              << " microsegundos.\n\n";

    HT.printTable();
    HT.searchItem(292, "Aparicio;Rafaela;70;S;34000");
    HT.loadFactor(table_size);

    return 0;
}