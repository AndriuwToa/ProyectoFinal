#include "funciones.h"

void menuGestionZonas();
void menuMonitoreo();
void menuPrediccion();
void menuAlertas();
void menuAnalisis();
void menuRecomendaciones();
void menuInformes();

int main() {
    cargarDatos();
    
    int opcion;
    do {
        printf("\n=== SISTEMA DE GESTION Y PREDICCION DE CONTAMINACION ===\n");
        printf("1. Gestion de Zonas\n");
        printf("2. Monitoreo Actual\n");
        printf("3. Prediccion de Contaminacion\n");
        printf("4. Alertas\n");
        printf("5. Analisis Historico\n");
        printf("6. Recomendaciones\n");
        printf("7. Informes\n");
        printf("0. Salir\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: menuGestionZonas(); break;
            case 2: menuMonitoreo(); break;
            case 3: menuPrediccion(); break;
            case 4: menuAlertas(); break;
            case 5: menuAnalisis(); break;
            case 6: menuRecomendaciones(); break;
            case 7: menuInformes(); break;
            case 0: 
                printf("Saliendo del sistema...\n");
                guardarDatos();
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
    
    return 0;
}

void menuGestionZonas() {
    int opcion;
    do {
        printf("\n=== GESTION DE ZONAS ===\n");
        printf("1. Agregar zona\n");
        printf("2. Listar zonas\n");
        printf("3. Buscar zona\n");
        printf("4. Modificar zona\n");
        printf("5. Eliminar zona\n");
        printf("0. Volver al menu principal\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: agregarZona(); break;
            case 2: listarZonas(); break;
            case 3: buscarZona(); break;
            case 4: modificarZona(); break;
            case 5: eliminarZona(); break;
            case 0: break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
}

void menuMonitoreo() {
    int opcion;
    do {
        printf("\n=== MONITOREO ACTUAL ===\n");
        printf("1. Ingresar datos de contaminacion\n");
        printf("2. Mostrar contaminacion actual\n");
        printf("3. Mostrar contaminacion historica\n");
        printf("4. Comparar con limites OMS\n");
        printf("5. Ingresar datos climaticos\n");
        printf("6. Comparar dias ingresados\n");
        printf("7. Comparar dias con estandares OMS\n");
        printf("0. Volver al menu principal\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: ingresarDatosContaminacion(); break;
            case 2: mostrarContaminacionActual(); break;
            case 3: mostrarContaminacionHistorica(); break;
            case 4: compararConLimitesOMS(); break;
            case 5: ingresarDatosClimaticos(); break;
            case 6: compararDiasIngresados(); break;
            case 7: compararDiasConOMS(); break;
            case 0: break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
}

void menuPrediccion() {
    int opcion;
    do {
        printf("\n=== PREDICCION DE CONTAMINACION ===\n");
        printf("1. Predecir contaminacion (24 horas)\n");
        printf("2. Mostrar predicciones\n");
        printf("0. Volver al menu principal\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: predecirContaminacion(); break;
            case 2: mostrarPredicciones(); break;
            case 0: break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
}

void menuAlertas() {
    int opcion;
    do {
        printf("\n=== ALERTAS ===\n");
        printf("1. Generar alertas\n");
        printf("2. Listar alertas\n");
        printf("0. Volver al menu principal\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: generarAlertas(); break;
            case 2: listarAlertas(); break;
            case 0: break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
}

void menuAnalisis() {
    int opcion;
    do {
        printf("\n=== ANALISIS HISTORICO ===\n");
        printf("1. Calcular promedios historicos (30 dias)\n");
        printf("2. Mostrar tendencias\n");
        printf("0. Volver al menu principal\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: calcularPromediosHistoricos(); break;
            case 2: mostrarTendencias(); break;
            case 0: break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
}

void menuRecomendaciones() {
    int opcion;
    do {
        printf("\n=== RECOMENDACIONES ===\n");
        printf("1. Generar recomendaciones\n");
        printf("2. Mostrar recomendaciones\n");
        printf("0. Volver al menu principal\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: generarRecomendaciones(); break;
            case 2: mostrarRecomendaciones(); break;
            case 0: break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
}

void menuInformes() {
    int opcion;
    do {
        printf("\n=== INFORMES ===\n");
        printf("1. Generar informe diario\n");
        printf("2. Generar informe historico\n");
        printf("3. Exportar zonas a texto\n");
        printf("4. Exportar alertas a texto\n");
        printf("5. Exportar recomendaciones a texto\n");
        printf("0. Volver al menu principal\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: generarInformeDiario(); break;
            case 2: generarInformeHistorico(); break;
            case 3: exportarZonasTexto(); break;
            case 4: exportarAlertasTexto(); break;
            case 5: exportarRecomendacionesTexto(); break;
            case 0: break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
}