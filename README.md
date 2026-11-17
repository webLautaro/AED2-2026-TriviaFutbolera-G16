# Trivia Futbolera — Proyecto Integrador AED II (2026)

**Cátedra:** Algoritmos y Estructuras de Datos II — FaCENA (UNNE)  
**Lenguaje:** C  

## Descripción
Aplicación de consola desarrollada en C que implementa un juego de preguntas y respuestas (trivia) sobre historia del fútbol en cuatro categorías (**Balón de Oro**, **Mundial de Fútbol**, **UEFA Champions League** y **Copa Libertadores**).  

La partida se desarrolla en **3 etapas de dificultad progresiva** (Fácil, Medio y Difícil), clasificadas automáticamente a partir del año detectado en el enunciado de cada pregunta, e incorpora un sistema de pistas (descarte de opciones) con penalización dinámica de puntaje, revisión final de respuestas y tabla de posiciones persistente por categoría.

## Estructuras de Datos y TADs Implementados
- **`tPregunta` (`tPregunta.h` / `tPregunta.c`):** TAD basado en una **Lista Enlazada dinámica** que carga y filtra preguntas desde el archivo de texto `preguntas.txt`, clasifica la dificultad según el año y elimina cada pregunta respondida para evitar repeticiones en la misma partida.
- **`tHistorial` (`tHistorial.h` / `tHistorial.c`):** TAD basado en una **Pila dinámica** que registra las respuestas del jugador durante la partida para mostrarlas en orden inverso al finalizar.
- **`tRanking` (`tRanking.h` / `tRanking.c`):** TAD encargado de la **persistencia en archivo binario (`ranking.dat`)** y del **ordenamiento por Burbuja** (de mayor a menor puntaje) filtrado por categoría.

## Estructura del Proyecto

    ├── TriviaFutbol.dev   # Archivo de proyecto para Dev-C++
    ├── main.c             # Menú principal y flujo de partida
    ├── tPregunta.h / .c   # TAD Lista Enlazada de preguntas
    ├── tHistorial.h / .c  # TAD Pila LIFO de historial
    ├── tRanking.h / .c    # TAD Archivo binario y ranking
    ├── preguntas.txt      # Banco de 224 preguntas históricas
    └── ranking.dat        # Archivo binario generado automáticamente al guardar puntajes

## Compilación y Ejecución
 Abrir `TriviaFutbol.dev` con **Dev-C++** y presionar `F11` (Compilar y Ejecutar).
 Una vez hecho eso, podrá ejecutar la trivia desde el .exe.

## Uso de Inteligencia Artificial
En cumplimiento con las pautas de la cátedra, se detalla el uso de herramientas de IA (Gemini) como apoyo durante el desarrollo:
- **Funciones auxiliares de librería estándar:** Consulta e integración de `system("cls")` (`<stdlib.h>`) para limpiar la consola entre pantallas, y de `srand(time(NULL))` junto con `rand()` (`<time.h>` y `<stdlib.h>`) para generar la selección aleatoria de preguntas y el descarte de opciones en el sistema de pistas.
- **Banco de datos:** Asistencia en la recopilación y formateo del archivo `preguntas.txt` con las 224 preguntas históricas de las cuatro categorías.
- **Documentación y estilo:** Revisión de comentarios descriptivos en los módulos `.h` y `.c`.

Todo el código fue adaptado a los contenidos vistos en la asignatura, probado en Dev-C++ y revisado por los integrantes del equipo.

## Integrantes — Grupo 16
- **Castillo, Lautaro**
- **Franco, Matias**
- **Gómez, Tobias Juan Cruz**
- **Franco, Luciano Gabriel**
- **Chamorro Guzowski, Lautaro Ezequiel**
- **Cabral, Gonzalo Dante**