#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

const int POP_SIZE = 50;
const int MAX_GEN = 1000;
const double MUTATION_RATE = 0.05;

string TARGET; // frase objetivo definida por el usuario

// Genera un carácter ASCII visible aleatorio
char randomChar() {
    return (rand() % 95) + 32; // ASCII 32 a 126
}

// Mutación leve de una cadena
string mutateCopy(const string& original) {
    string copy = original;
    for (size_t i = 0; i < copy.length(); ++i)
        if ((double)rand() / RAND_MAX < MUTATION_RATE)
            copy[i] = randomChar();
    return copy;
}

// Evaluación: cuántos caracteres coinciden con el objetivo
int fitness(const string& chrom) {
    int score = 0;
    for (size_t i = 0; i < TARGET.length(); ++i)
        if (chrom[i] == TARGET[i]) ++score;
    return score;
}

// Selección por torneo
string select(const vector<string>& population) {
    string a = population[rand() % POP_SIZE];
    string b = population[rand() % POP_SIZE];
    return fitness(a) > fitness(b) ? a : b;
}

// Cruce uniforme
string crossover(const string& p1, const string& p2) {
    string child;
    for (size_t i = 0; i < TARGET.length(); ++i)
        child += (rand() % 2) ? p1[i] : p2[i];
    return child;
}

// Mutación directa sobre una cadena
void mutate(string& chrom) {
    for (size_t i = 0; i < chrom.length(); ++i)
        if ((double)rand() / RAND_MAX < MUTATION_RATE)
            chrom[i] = randomChar();
}

int main() {
    srand(time(0));

    cout << "Ingresa la frase objetivo (puede contener espacios y cualquier simbolo):\n> ";
    getline(cin, TARGET);

    vector<string> seeds;
    cout << "Introduce 3 frases iniciales de exactamente " << TARGET.length() << " caracteres:\n";
    for (int i = 0; i < 3; ++i) {
        string input;
        do {
            cout << "Frase " << i + 1 << ": ";
            getline(cin, input);
            if (input.length() != TARGET.length())
                cout << "La frase debe tener exactamente " << TARGET.length() << " caracteres.\n";
        } while (input.length() != TARGET.length());
        seeds.push_back(input);
    }

    // Inicializar población
    vector<string> population = seeds;
    while (population.size() < POP_SIZE) {
        string base = seeds[rand() % seeds.size()];
        population.push_back(mutateCopy(base));
    }

    // Evolución
    for (int gen = 1; gen <= MAX_GEN; ++gen) {
        sort(population.begin(), population.end(), [](const string& a, const string& b) {
            return fitness(a) > fitness(b);
        });

        cout << "Generacion " << gen << " | Mejor: " << population[0]
             << " | Fitness: " << fitness(population[0]) << endl;

        if (population[0] == TARGET) {
            cout << "Frase objetivo alcanzada en la generacion " << gen << "!\n";
            break;
        }

        vector<string> new_population;
        while (new_population.size() < POP_SIZE) {
            string p1 = select(population);
            string p2 = select(population);
            string child = crossover(p1, p2);
            mutate(child);
            new_population.push_back(child);
        }

        population = new_population;
    }

    return 0;
}
