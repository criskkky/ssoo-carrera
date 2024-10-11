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
    void avanzar() {
        // Generadores aleatorios para simular el avance y la velocidad de cada auto
        std::random_device rd; // Generador de números aleatorios
        std::mt19937 gen(rd()); // Generador de números aleatorios
        std::uniform_int_distribution<> distDist(1, 10); // Avance entre 1 y 10 metros
        std::uniform_int_distribution<> velocidadDist(100, 500); // Tiempo de espera entre 100 y 500 ms

        // Mientras el auto no haya terminado la carrera
        while (!terminado) {
            int avance = distDist(gen); // Calcular el avance aleatorio
            distanciaRecorrida += avance; // Sumarlo a la distancia recorrida

            // Si el auto ha recorrido la distancia total o más, se marca como terminado
            if (distanciaRecorrida >= distanciaTotal) {
                distanciaRecorrida = distanciaTotal;
                terminado = true;
            }

            // Bloquear el acceso a la consola para evitar condiciones de carrera en las salidas
            {
                std::lock_guard<std::mutex> lock(mutex);
                std::cout << "Auto" << id << " avanza " << avance << " metros (total: " << distanciaRecorrida << " metros)" << std::endl;
                if (terminado) {
                    std::cout << "Auto" << id << " termina la carrera!" << std::endl;
                }
            }

            // Dormir el hilo durante un tiempo aleatorio para simular la velocidad del auto
            std::this_thread::sleep_for(std::chrono::milliseconds(velocidadDist(gen)));
        }
    }

    // Métodos getter para obtener información del auto
    int getId() const { return id; }
    int getDistanciaRecorrida() const { return distanciaRecorrida; }
    bool haTerminado() const { return terminado; }

private:
    int id; // ID del auto
    int distanciaRecorrida; // Distancia que ha recorrido hasta el momento
    int distanciaTotal; // Distancia total que debe recorrer
    bool terminado; // Indica si el auto ha terminado la carrera
    static std::mutex mutex; // Mutex para proteger la salida por consola
};

// Inicialización del mutex estático
std::mutex Auto::mutex;

// Clase Carrera que gestiona la carrera y los autos participantes
class Carrera {
public:
    // Constructor: inicializa la carrera con la distancia total y el número de autos
    Carrera(int distanciaTotal, int numAutos) : distanciaTotal(distanciaTotal), numAutos(numAutos) {
        // Crear y agregar autos al vector
        for (int i = 0; i < numAutos; ++i) {
            autos.emplace_back(i, distanciaTotal);
        }
    }

    // Método para iniciar la carrera
    void iniciar() {
        std::vector<std::thread> threads; // Vector de hilos para ejecutar a cada auto

        // Crear un hilo para cada auto
        for (auto& auto_ : autos) {
            threads.emplace_back(&Auto::avanzar, &auto_);
        }

        // Esperar a que todos los hilos terminen (autos terminen la carrera)
        for (auto& t : threads) {
            t.join();
        }

        // Mostrar los resultados finales de la carrera
        mostrarResultados();
    }

private:
    // Método para mostrar los resultados finales ordenados por distancia recorrida
    void mostrarResultados() {
        // Ordenar los autos por la distancia recorrida (de mayor a menor)
        std::sort(autos.begin(), autos.end(), [](const Auto& a, const Auto& b) {
            return a.getDistanciaRecorrida() > b.getDistanciaRecorrida();
        });

        // Mostrar los resultados en orden
        std::cout << "\nResultados finales:" << std::endl;
        std::cout << "Lugar\tAuto" << std::endl;
        std::cout << "----------------" << std::endl;
        for (int i = 0; i < numAutos; ++i) {
            std::cout << i + 1 << "\tAuto" << autos[i].getId() << std::endl;
        }
    }

    std::vector<Auto> autos; // Vector de autos participantes
    int distanciaTotal; // Distancia total de la carrera
    int numAutos; // Número de autos en la carrera
};

int main(int argc, char* argv[]) {
    // Validar los argumentos de entrada
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia_total> <numero_de_autos>" << std::endl;
        return 1;
    }

    // Obtener la distancia total y el número de autos de los argumentos
    int distanciaTotal = std::stoi(argv[1]);
    int numAutos = std::stoi(argv[2]);

    // Mostrar los detalles de la carrera
    std::cout << "Distancia total carrera: " << distanciaTotal << " metros" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    // Crear e iniciar la carrera
    Carrera carrera(distanciaTotal, numAutos);
    carrera.iniciar();

    return 0;
}
