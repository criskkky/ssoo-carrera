#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

class Auto {
public:
    Auto(int id, int distanciaTotal) : id(id), distanciaRecorrida(0), distanciaTotal(distanciaTotal), terminado(false) {}

    void avanzar() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distDist(1, 10);
        std::uniform_int_distribution<> velocidadDist(100, 500);

        while (!terminado) {
            int avance = distDist(gen);
            distanciaRecorrida += avance;

            if (distanciaRecorrida >= distanciaTotal) {
                distanciaRecorrida = distanciaTotal;
                terminado = true;
            }

            {
                std::lock_guard<std::mutex> lock(mutex);
                std::cout << "Auto" << id << " avanza " << avance << " metros (total: " << distanciaRecorrida << " metros)" << std::endl;
                if (terminado) {
                    std::cout << "Auto" << id << " termina la carrera!" << std::endl;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(velocidadDist(gen)));
        }
    }

    int getId() const { return id; }
    int getDistanciaRecorrida() const { return distanciaRecorrida; }
    bool haTerminado() const { return terminado; }

private:
    int id;
    int distanciaRecorrida;
    int distanciaTotal;
    bool terminado;
    static std::mutex mutex;
};

std::mutex Auto::mutex;

class Carrera {
public:
    Carrera(int distanciaTotal, int numAutos) : distanciaTotal(distanciaTotal), numAutos(numAutos) {
        for (int i = 0; i < numAutos; ++i) {
            autos.emplace_back(i, distanciaTotal);
        }
    }

    void iniciar() {
        std::vector<std::thread> threads;
        for (auto& auto_ : autos) {
            threads.emplace_back(&Auto::avanzar, &auto_);
        }

        for (auto& t : threads) {
            t.join();
        }

        mostrarResultados();
    }

private:
    void mostrarResultados() {
        std::sort(autos.begin(), autos.end(), [](const Auto& a, const Auto& b) {
            return a.getDistanciaRecorrida() > b.getDistanciaRecorrida();
        });

        std::cout << "\nResultados finales:" << std::endl;
        std::cout << "Lugar\tAuto" << std::endl;
        std::cout << "----------------" << std::endl;
        for (int i = 0; i < numAutos; ++i) {
            std::cout << i + 1 << "\tAuto" << autos[i].getId() << std::endl;
        }
    }

    std::vector<Auto> autos;
    int distanciaTotal;
    int numAutos;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia_total> <numero_de_autos>" << std::endl;
        return 1;
    }

    int distanciaTotal = std::stoi(argv[1]);
    int numAutos = std::stoi(argv[2]);

    std::cout << "Distancia total carrera: " << distanciaTotal << " metros" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    Carrera carrera(distanciaTotal, numAutos);
    carrera.iniciar();

    return 0;
}