#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

// Clase Auto que representa cada auto participante en la carrera
class Auto {
public:
    // Constructor: inicializa el auto con su ID y la distancia total de la carrera
    Auto(int id, int distanciaTotal) : id(id), distanciaRecorrida(0), distanciaTotal(distanciaTotal), terminado(false) {}

    // Método que simula el avance del auto
    void avanzar(std::vector<int>& posiciones, std::mutex& posicionesMutex) {
        std::random_device rd; // Generador de números aleatorios
        std::mt19937 gen(rd()); // Motor de números aleatorios
        std::uniform_int_distribution<> distDist(1, 10); // Distribución uniforme para el avance
        std::uniform_int_distribution<> velocidadDist(100, 500); // Distribución uniforme para la velocidad

        while (!terminado) {
            int avance = distDist(gen); // Avance aleatorio entre 1 y 10 metros
            distanciaRecorrida += avance; // Actualizar la distancia recorrida

            if (distanciaRecorrida >= distanciaTotal) { // Si la distancia recorrida supera la distancia total
                distanciaRecorrida = distanciaTotal; // Ajustar la distancia recorrida a la distancia total
                terminado = true; // Marcar el auto como terminado
            }

            {
                std::lock_guard<std::mutex> lock(mutex); // Bloquear el acceso a la consola
                std::cout << "Auto" << id << " avanza " << avance << " metros (total: " << distanciaRecorrida << " metros)" << std::endl; // Mostrar el avance del auto
                if (terminado) { // Si el auto ha terminado la carrera
                    std::cout << "Auto" << id << " termina la carrera!" << std::endl; // Mostrar un mensaje de finalización
                    std::lock_guard<std::mutex> posicionesLock(posicionesMutex); // Bloquear el acceso al vector de posiciones
                    posiciones.push_back(id); // Registrar la posición de llegada
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(velocidadDist(gen))); // Dormir un tiempo aleatorio para simular la velocidad
        }
    }

    int getId() const { return id; } // Método para obtener el ID del auto
    int getDistanciaRecorrida() const { return distanciaRecorrida; } // Método para obtener la distancia recorrida
    bool haTerminado() const { return terminado; } // Método para verificar si el auto ha terminado la carrera

private: // Declaración de atributos privados
    int id;
    int distanciaRecorrida;
    int distanciaTotal;
    bool terminado;
    static std::mutex mutex;
};

std::mutex Auto::mutex; // Inicialización del mutex estático, mutex: protege el acceso a la consola.

class Carrera {
public:
    Carrera(int distanciaTotal, int numAutos) : distanciaTotal(distanciaTotal), numAutos(numAutos) {
        for (int i = 0; i < numAutos; ++i) {
            autos.emplace_back(i, distanciaTotal);
        }
    }

    void iniciar() {
        std::vector<std::thread> threads;
        std::vector<int> posiciones; // Vector para registrar el orden de llegada
        std::mutex posicionesMutex;  // Mutex para proteger el acceso a posiciones

        for (auto& auto_ : autos) {
            threads.emplace_back(&Auto::avanzar, &auto_, std::ref(posiciones), std::ref(posicionesMutex));
        }

        for (auto& t : threads) {
            t.join();
        }

        mostrarResultados(posiciones); // Mostrar los resultados según el orden de llegada
    }

private:
    void mostrarResultados(const std::vector<int>& posiciones) {
        std::cout << "\nResultados finales:" << std::endl;
        std::cout << "Lugar\tAuto" << std::endl;
        std::cout << "----------------" << std::endl;
        for (size_t i = 0; i < posiciones.size(); ++i) {
            std::cout << i + 1 << "\tAuto" << posiciones[i] << std::endl;
        }
    }

    std::vector<Auto> autos; // Vector de autos
    int distanciaTotal; // Distancia total de la carrera
    int numAutos; // Número de autos
};

int main(int argc, char* argv[]) {
    // Verificar que el número de argumentos sea correcto (3: el nombre del programa, la distancia total y el número de autos)
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia_total> <numero_de_autos>" << std::endl;
        return 1; // Salir si no se proporcionan los argumentos necesarios
    }

    // Convertir los argumentos de entrada (cadena de caracteres) a enteros
    int distanciaTotal = std::stoi(argv[1]);
    int numAutos = std::stoi(argv[2]);

    // Obtener el número máximo de hilos que el hardware puede soportar simultáneamente
    int numMaxHilos = std::thread::hardware_concurrency();

    // Verificar si el número de autos es demasiado grande para el sistema
    if (numAutos > numMaxHilos * 2) {
        std::cerr << "El número de autos es demasiado alto para tu sistema. "
                  << "Intenta con un número menor o igual a " << numMaxHilos * 2 << "." << std::endl;
        return 1; // Terminar el programa si hay demasiados autos
    }

    // Mostrar la configuración de la carrera en la consola
    std::cout << "Distancia total carrera: " << distanciaTotal << " metros" << std::endl;
    std::cout << "Número de autos: " << numAutos << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    // Crear la carrera con la distancia total y el número de autos
    Carrera carrera(distanciaTotal, numAutos);

    // Iniciar la simulación de la carrera
    carrera.iniciar();

    return 0; // Indicar que el programa terminó correctamente
}
