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
        static const int hashGroups = 500;
        vector<string> table = vector<string>(hashGroups, "EMPTY");
        bool exists = false;
        int total_colisions = 0;

    public:
        vector<string>& getTable();
        vector<string> readCSV();
        int hashFunction(int key);
        bool isEmpty() const;
        void insertItem(int key, string value);
        void removeItem(int key, string value);
        void searchItem(int key, string value);
        void searchTable(int key);
        void printTable();
        void loadFactor(int size);      
};

vector<string>& HashTable::getTable()
{
    return table;
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

    if(table[hashValue] != "EMPTY")
    {
        while(table[hashValue] != "EMPTY")
        {
            hashValue += 1;
            total_colisions++;

            if(hashValue == hashGroups)
                hashValue = 0;
        }
    }
  
    table[hashValue] = value;

    return;
}

void HashTable::removeItem(int key, string value)
{
    int hashValue = hashFunction(key);
    bool deleted = false;
    int while_iterations = 0;

    while(!deleted && while_iterations < total_colisions)
    {
        if(table[hashValue] == value)
        {
            cout << "\nValor " << table[hashValue] << " borrado con exito!" << "\n";
            table.erase(table.begin() + hashValue);
            deleted = true;
        }
        else
            hashValue += 1;

        if(hashValue == hashGroups)
            hashValue = 0;

        while_iterations++;
    }
    
    if(!deleted)
        cout << "\nNo se han podidio encontrar los datos! Ningun dato ha sido eliminado!  " << endl;
}

void HashTable::searchItem(int key, string value)
{
    int hashValue = hashFunction(key);
    bool found = false;
    int while_iterations = 0;

    while(!found && while_iterations < total_colisions)
    {
        if(table[hashValue] == value)
        {
            cout << "\nValor " << table[hashValue] << " encontrado con exito!" << "\n";
            found = true;
        }
        else
            hashValue += 1;

        if(hashValue == hashGroups)
            hashValue = 0;

        while_iterations++;
    }
    
    if(!found)
        cout << "\nNo se han podidio encontrar los datos! Vuelva a buscar de nuevo  " << endl;
}

void HashTable::printTable()
{
    for (size_t i = 0; i < table.size(); i++)
    {
        cout << table[i] << "\n";
    }

    cout << "\nSe han producido " << total_colisions << " colisiones." << endl; 

    return;
}

void HashTable::loadFactor(int size)
{
    int filled_positions = 0;

    for (size_t i = 0; i < hashGroups; i++)
    {
        if(table[i] != "EMPTY")
            filled_positions++;
    }

    std::cout << "\nEl factor de carga de lista es de " << (float)size / filled_positions << std::endl;
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
    int table_size = HT.getTable().size();

    auto start = high_resolution_clock::now();
    
    for (size_t i = 0; i < table_size; i++)
    {
        // Se usa 2 veces la funcion rand para exceder el valor maximo MAX_RAND
        // Y así evitar sobreescribir datos.
        HT.insertItem(rand() * (rand() % 15000 + 1), data[i]);
    }

    auto stop = high_resolution_clock::now();

    cout << "\nLa insercion de datos a la tabla se produjo en "
              << duration_cast<microseconds>(stop - start).count()
              << " microsegundos.\n\n";

    HT.printTable();
    HT.removeItem(rand() + rand(), "Chico;Florinda;70;N;60000");
    HT.removeItem(rand() + rand(), "Romero;Irene;47;N;584810SSS");
    HT.searchItem(rand() + rand(), "Pavez;Terele;62;S;20000");
    HT.loadFactor(table_size);

    return 0;
}