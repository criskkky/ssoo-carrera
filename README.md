# 🏎️ Simulador de Carrera de Autos 🏁

¡Bienvenido al **Simulador de Carrera de Autos**! Este programa simula una competencia entre varios autos, aprovechando la **programación multihilo** en C++ para ofrecer una experiencia dinámica y realista.

## Descripción del Proyecto
El proyecto recrea una carrera automovilística mediante el uso de hilos independientes en C++. Cada auto es representado por un hilo que avanza de manera aleatoria hasta completar una distancia predeterminada. La clase `Carrera` gestiona la simulación, mientras que cada participante es una instancia de la clase `Auto`.

### Detalles del Funcionamiento
- **Clase `Auto`**:
  - Simboliza a un auto de la carrera, identificado por un ID único, con una distancia recorrida y una meta que debe alcanzar.
  - El método `avanzar` permite que el auto progrese de manera aleatoria:
    - El avance es un número aleatorio entre 1 y 10 metros.
    - Tras avanzar, el auto se "pausa" por un tiempo aleatorio entre 100 y 500 milisegundos, simulando diferentes velocidades.
    - Al completar la distancia, el auto se marca como finalizado y se guarda su posición de llegada.
  - Para evitar conflictos de concurrencia, el acceso a la consola y al vector de posiciones está protegido con `std::mutex`.

- **Clase `Carrera`**:
  - Gestiona la carrera, creando un hilo por cada auto participante. Los autos avanzan en paralelo, y al terminar todos, se muestran los resultados en orden de llegada.
  - Utiliza un vector protegido por un mutex para almacenar las posiciones de llegada, garantizando un acceso seguro ante la concurrencia.

### Características Técnicas
- **Multihilo**: Cada auto corre en su propio hilo, lo que permite simulación simultánea para todos los competidores.
- **Aleatoriedad**: El avance y la velocidad de los autos se determinan mediante un generador de números aleatorios.
- **Sincronización**: El uso de `std::mutex` asegura que el acceso a los recursos compartidos sea seguro, evitando interferencias entre hilos.

> [!NOTE]
> Los comandos presentados a continuación deben ejecutarse en la terminal de Linux.

## 🚀 Pre-requisitos
Asegúrate de tener los siguientes programas instalados:
- **g++**: Compilador de C++.
- **make**: Herramienta para gestionar la compilación del proyecto.

Instala ambos requisitos con el siguiente comando:
```bash
sudo apt install g++ make
```

## 🔧 Compilación
- **Paso 1**: Clonar el Repositorio y Acceder al Directorio
```bash
git clone https://github.com/criskkky/ssoo-carrera.git && cd ssoo-carrera
```
Este comando descarga el repositorio del simulador y cambia el directorio al proyecto recién clonado.

- **Paso 2**: Compilar el Proyecto
```bash
make
```
Esto generará un ejecutable llamado `carrera`.

## 🏁 Ejecución
Para iniciar una carrera, usa el siguiente comando:
```bash
./carrera <distancia_total> <numero_de_autos>
```

### Parámetros de Entrada
- `<distancia_total>`: La distancia total de la carrera (en metros).
- `<numero_de_autos>`: La cantidad de autos que competirán.

### Ejemplo
```bash
./carrera 100 3
```
Este comando simulará una carrera de **100 metros** con **3 autos**.

## 🧹 Limpieza
Para eliminar los archivos generados tras la compilación, ejecuta:

```bash
make clean
```
---
Creado por [**@criskkky**](https://github.com/criskkky) y [**@John-Henriquez**](https://github.com/John-Henriquez) 🛣️