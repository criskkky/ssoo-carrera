# 🏎️ Simulador de Carrera de Autos 🏁

¡Bienvenido al **Simulador de Carrera de Autos**! Este programa simula una emocionante carrera entre varios autos, utilizando **programación multihebras** en C++ para hacerla más dinámica y realista.

## 🚀 Pre-requisitos

Antes de comenzar, asegúrate de tener instalados los siguientes programas:

- **g++**: El compilador de C++.
- **make**: Herramienta para gestionar la compilación del proyecto.

Instálalos usando el siguiente comando:

```bash
sudo apt install g++ make
```

## 🔧 Compilación

Para compilar el simulador, simplemente ejecuta:

```bash
make
```

Esto generará un archivo ejecutable llamado `carrera`.

## 🏁 Ejecución

Para iniciar una carrera, utiliza el siguiente comando:

```bash
./carrera <distancia_total> <numero_de_autos>
```

### Parámetros:

- `<distancia_total>`: La distancia total de la carrera en metros.
- `<numero_de_autos>`: El número de autos que participarán en la carrera.

### Ejemplo:

```bash
./carrera 100 3
```

En este ejemplo, se simulará una carrera de **100 metros** con **3 autos** compitiendo.

## 🧹 Limpieza

Para eliminar el archivo ejecutable generado tras la compilación, ejecuta:

```bash
make clean
```

---

Creado por [**@criskkky**](https://github.com/criskkky) y [**@John-Henriquez**](https://github.com/John-Henriquez) 🛣️