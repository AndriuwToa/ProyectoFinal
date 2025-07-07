#ifndef CONTAMINACION_H
#define CONTAMINACION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ZONAS 10
#define MAX_DIAS 30
#define MAX_NOMBRE 50
// Limites OMS 2021 - Contaminantes
#define LIMITE_CO2 9.0    // ppm (8 horas) - equivalente a 4 mg/m3
#define LIMITE_SO2 0.021  // ppm (24 horas) - equivalente a 40 μg/m³
#define LIMITE_NO2 0.013  // ppm (24 horas) - equivalente a 25 μg/m³
#define LIMITE_PM25 15.0  // μg/m³ (24 horas)

// Limites OMS - Condiciones climaticas de confort
#define TEMP_MIN_CONFORT 18.0   // Temperatura minima (°C)
#define TEMP_MAX_CONFORT 24.0   // Temperatura maxima (°C)
#define HUMEDAD_OPTIMA 50.0     // Humedad relativa optima (%)
#define VIENTO_MAX_CONFORT 0.72 // Velocidad maxima del viento (km/h) - 0.2 m/s

typedef struct {
    float co2;
    float so2;
    float no2;
    float pm25;
} Contaminantes;

typedef struct {
    float temperatura;
    float humedad;
    float velocidad_viento;
} Clima;

typedef struct {
    char nombre[MAX_NOMBRE];
    Contaminantes niveles;
    Clima clima;
    time_t fecha;
} RegistroDiario;

typedef struct {
    int id;
    char nombre[MAX_NOMBRE];
    RegistroDiario registros[MAX_DIAS];
    int num_registros;
} Zona;

typedef struct {
    char mensaje[200];
    time_t fecha;
    int prioridad; // 1: baja, 2: media, 3: alta
} Alerta;

typedef struct {
    char zona[MAX_NOMBRE];
    char recomendacion[300];
    time_t fecha;
    int tipo; // 1: preventiva, 2: correctiva, 3: urgente
} Recomendacion;

// Funciones de gestión de zonas
void agregarZona();
void listarZonas();
void buscarZona();
void modificarZona();
void eliminarZona();

// Funciones de monitoreo
void ingresarDatosContaminacion();
void mostrarContaminacionActual();
void mostrarContaminacionHistorica();
void compararConLimitesOMS();
void compararDiasIngresados();
void compararDiasConOMS();

// Funciones de predicción
void ingresarDatosClimaticos();
void predecirContaminacion();
void mostrarPredicciones();

// Funciones de alertas
void generarAlertas();
void listarAlertas();
void guardarAlertas();

// Funciones de análisis histórico
void calcularPromediosHistoricos();
void mostrarTendencias();

// Funciones de recomendaciones
void generarRecomendaciones();
void mostrarRecomendaciones();

// Funciones de informes
void generarInformeDiario();
void generarInformeHistorico();
void exportarZonasTexto();
void exportarAlertasTexto();
void exportarRecomendacionesTexto();

// Funciones auxiliares
void limpiarBuffer();
int leerEntero(const char *mensaje);
float leerFloat(const char *mensaje);
void leerCadena(const char *mensaje, char *cadena, int longitud);
int validarFecha(const char *fecha);
void formatearFecha(time_t fecha, char *fechaStr);
void cargarDatos();
void guardarDatos();

// Variables globales
extern Zona zonas[MAX_ZONAS];
extern Alerta alertas[MAX_ZONAS * MAX_DIAS * 4]; // 4 alertas máximo por día por zona
extern Recomendacion recomendaciones[MAX_ZONAS * 3]; // 3 recomendaciones por zona maximo
extern int num_zonas;
extern int num_alertas;
extern int num_recomendaciones;

#endif