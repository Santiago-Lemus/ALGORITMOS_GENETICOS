#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>   

using namespace std;

//Declaracion de las constantes que vamos a utilizar durante todo el algoritmo
const int TAM_POBLACION = 10;
const int NUM_GENERACIONES = 100;
const double PROB_MUTACION = 0.5;
const double LIM_INF = 0.0;
const double LIM_SUP = 5.0;



// Función para generar un double aleatorio en el rango [min, max]
double my_rand_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Función cuya raíz queremos encontrar 
double funcion(double x) {
    return x * x - 2; // Raíz verdadera: sqrt(2)
}

//La funcion Fitness se encarga de evaluar cada individuo de la poblacion y asignarle un puntaje segun el problema a solucionar
double fitness(double x) {
    //Los algoritmos geneticos suelen buscar el puntaje mas alto, o sea, maximizar, en nuestro caso, aplicamos el valor absoluto y luego su negatico para que el resultado determina el que esté más cerca a cero, significa el que tenga menor valor
    return -abs(funcion(x));
}

// La funcion genera un individuo aleatorio dentro de los rangos que marcamos
double generarIndividuo() {
    return my_rand_double(LIM_INF, LIM_SUP);
}


//subrutina encargada de mutar, el simbolo & en la variable individuo significa que se pasa por referencia, esto significa que los cambios realizados durante la ejecucion se van a aplicar a la variable original dada, y no a una copia. Esto es crucial porque queremos que los cambios de mutación se apliquen al individuo real en la población
void mutar(double& individuo) {

    //El condicional se encarga de realizar la mutación solo con la probabilidad dada. Asegura que solo el 30% de la poblacion muten
    if (my_rand_double(0.0, 1.0) < PROB_MUTACION) {
        
        //Se aplica el ruido o paso de la mutacion con la condicional, esto para luego mirar si el individuo luego de la mutacion sigue manteniendose dentro del rango
        individuo += my_rand_double(-0.5, 0.5);

        if (individuo < LIM_INF) {
            individuo = LIM_INF;
        } else if (individuo > LIM_SUP) {
            individuo = LIM_SUP;
        }
    }
}


// Selección por torneo para ver los mejores puntajes Fitness para ls siguiente generacion, su funcionalidad es seleccionar dos elementos aleatorios del array poblacion y comparar su puntaje Fitness, para devolver el mejor de estos.
double torneo(double poblacion[], int size) {
    // Generación manual de índices aleatorios
    int idx_a = rand() % size;
    int idx_b = rand() % size;

    double a = poblacion[idx_a];
    double b = poblacion[idx_b];
    
    return (fitness(a) > fitness(b)) ? a : b;
}


//La funcion cruce se encarga de la reproduccion de los padres, esto se realiza de forma por promedio sencilla
double cruzar(double padre1, double padre2) {
    return (padre1 + padre2) / 2.0;
}

int main() {
    // Sembrar el generador de números aleatorios simple
    srand(static_cast<unsigned int>(time(0)));

    // Declaracion de la poblacion en un array con su tamaño
    double poblacion[TAM_POBLACION];

    //El bucle se encarga de llenar la primera generación con números totalmente aleatorios
    for (int i = 0; i < TAM_POBLACION; ++i) {
        poblacion[i] = generarIndividuo();
    }

    //La declaracion de la variable mejor se encarga de almacenar el mejor gen de cada generación, luego, la variable mejorFitness guarda el puntaje Fitness de este gen
    double mejor = poblacion[0];
    double mejorFitness = fitness(mejor);


    //El siguiente bucle se encarga de ejercutar las siguientes generaciones hasta el numero maximo de generaciones
    for (int gen = 1; gen <= NUM_GENERACIONES; ++gen) {
        //Este array temporal nuevaPoblacion servirá para construir la próxima generación de individuos antes de que reemplace a la actual
        double nuevaPoblacion[TAM_POBLACION];


        //Este el el código central para los algoritmos geneticos, inicialmente crea los hijos de la siguiente generación
        for (int i = 0; i < TAM_POBLACION; ++i) {

            //Se generan los dos padres aleatorios utilizando la funcion torneo
            double padre1 = torneo(poblacion, TAM_POBLACION);
            double padre2 = torneo(poblacion, TAM_POBLACION);

            //La funcion cruzar para realizar la reproduccion entre los dos padres
            double hijo = cruzar(padre1, padre2);

            //EL hijo resultante se muta con la probabilidad de mutacion marcada anteriormente
            mutar(hijo);

            //Los hijos ya mutados se añaden a la nueva poblacion en la posicion actual
            nuevaPoblacion[i] = hijo;

            //Llenado la siguiente generacion, se compara su puntajeFitness con el mejor hasta ahora, si es mejor se actualiza para el nuevo
            if (fitness(hijo) > mejorFitness) {
                mejor = hijo;
                mejorFitness = fitness(hijo);
            }
        }

        // EL ciclo for copia todos los elementos de la nueva poblacion a la poblacion general, para que así el proceso se repita con la generacion creada
        for (int i = 0; i < TAM_POBLACION; ++i) {
            poblacion[i] = nuevaPoblacion[i];
        }

        cout << "Generacion " << gen
                  << " | Mejor x: " << mejor
                  << " | f(x): " << funcion(mejor)
                  << " | |f(x)|: " << abs(funcion(mejor)) << endl;

        // Detener si f(x) ya es suficientemente cercano a 0
        if (abs(funcion(mejor)) < 0.001) {
            cout << " Raiz aproximada encontrada.\n";
            break;
        }
    }

    cout << "\n Aproximación de raíz:\n";
    cout << "x =" << mejor << "\n";
    cout << "f(x) = " << funcion(mejor) << "\n";

    return 0;
}