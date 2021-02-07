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
        static const int hashGroups = 512; // Debe de ser potencia de 2
        vector<string> table = vector<string>(hashGroups, "EMPTY");
        bool exists = false;
        int total_colisions = 0;
        int filled_positions = 0;
    
    public:
        vector<string>& getTable();
        vector<string> readCSV();
        int hashFunction(int key);
        int reHashFunction(int key);
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

int HashTable::reHashFunction(int key)
{
    int reHashnNum = key / hashGroups;

    if(reHashnNum % 2 == 0)
        return reHashnNum;
    else
        return reHashnNum + 1;
}

void HashTable::insertItem(int key, string value)
{
    unsigned int hashValue = hashFunction(key);
    bool success = false;
    int reHashValue = 0;
    int it = 1;

    hashValue = reHashFunction(key) + 1; //Tiene que ser impar

    while(filled_positions < 512)
    {
        if(table[hashValue] == "EMPTY")
        {
            table[hashValue] = value;
            filled_positions++;
        }
        else
        {
            reHashValue = (key + hashValue * it) % hashGroups; 

            if(table[reHashValue] == "EMPTY")
            {
                table[reHashValue] = value;
                filled_positions++;
                success = true; 
            }
            else
                total_colisions++; 
            
            if(!success)
                it++;
            else 
                return;  
        }  
    } 
}

void HashTable::removeItem(int key, string value)
{
    unsigned int hashValue = hashFunction(key);
    bool success = false;
    bool deleted = false;
    int reHashValue = 0;
    int it = 1;
    int while_iterations = 0;

    hashValue = reHashFunction(key) + 1; //Tiene que ser impar

    while(!deleted && while_iterations < total_colisions) // Segunda condicion para que salga en caso de no encontrar el dato buscado
    {
        if(table[hashValue] == value)
        {
            cout << "\nDato: " << table[hashValue] << " eliminado." << endl;
            table.erase(table.begin() + hashValue);
            deleted = true;
        }
        else
        {
            reHashValue = (key + hashValue * it) % hashGroups; 

            if(table[reHashValue] == value)
            {
                cout << "\nDato: " << table[reHashValue] << " eliminado." << endl;
                table.erase(table.begin() + reHashValue);
                success = true;
                deleted = true;
            }
            
            if(!success)
                it++;
            else 
                return;  
        }
        while_iterations++;  
    }

    if(while_iterations >= total_colisions)
        cout << "\nNo se han podidio encontrar los datos! Ningun dato ha sido eliminado!" << endl;   
}

void HashTable::searchItem(int key, string value)
{
    unsigned int hashValue = hashFunction(key);
    bool success = false;
    bool found = false;
    int reHashValue = 0;
    int it = 1;
    int while_iterations = 0;

    hashValue = reHashFunction(key) + 1; //Tiene que ser impar

    while(!found && while_iterations < total_colisions) // Segunda condicion para que salga en caso de no encontrar el dato buscado
    {
        if(table[hashValue] == value)
        {
            cout << "\nValor: " << table[hashValue] << " encontrado con exito!." << endl;
            found = true;
        }
        else
        {
            reHashValue = (key + hashValue * it) % hashGroups; 

            if(table[reHashValue] == value)
            {   
                cout << "\nValor: " << table[reHashValue] << " encontrado con exito!." << endl;
                success = true;
                found = true;
            }
            
            if(!success)
                it++;
            else 
                return;  
        }
        while_iterations++;   
    }

    if(while_iterations >= total_colisions)
        cout << "\nNo se han podido encontrar los datos! Vuelva a intentarlo de nuevo" << endl;    
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
    int filled_pos = 0;

    for (size_t i = 0; i < hashGroups; i++)
    {
        if(table[i] != "EMPTY")
            filled_pos++;
    }

    std::cout << "\nEl factor de carga de lista es de " << (float)size / filled_pos << std::endl;
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
        HT.insertItem(rand() + rand(), data[i]);
    }

    auto stop = high_resolution_clock::now();

    cout << "\nLa insercion de datos a la tabla se produjo en "
              << duration_cast<microseconds>(stop - start).count()
              << " microsegundos.\n\n";

    HT.printTable();
    HT.removeItem(rand() + rand(), "Arias;Imanol;55;N;10000AAH");
    HT.removeItem(rand() + rand(), "Gil;Ariadna;40;S;19000");
    HT.searchItem(rand() + rand(), "Pataky;Elsa;32;N;27000");
    HT.loadFactor(table_size);
    
    return 0;
}