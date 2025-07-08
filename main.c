#include "funciones.h"

void menuGestionZonas();
void menuMonitoreo();
void menuPrediccion();
void menuAlertas();
void menuAnalisis();
void menuRecomendaciones();
void menuInformes();
int validarNombreCiudad() {
    return strlen(nombre_ciudad) > 0;
}

int validarMinimoZonas() {
    return num_zonas >= 5;
}

int main() {
    cargarDatos();
    
    int opcion;
    do {
        if (strlen(nombre_ciudad) > 0) {
            printf("\n=== SISTEMA DE GESTION Y PREDICCION DE CONTAMINACION %s (actualmente %d zonas) ===\n", nombre_ciudad, num_zonas);
        } else {
            printf("\n=== SISTEMA DE GESTION Y PREDICCION DE CONTAMINACION ===\n");
        }
        printf("1. Agregar nombre de la ciudad\n");
        printf("2. Gestion de Zonas\n");
        printf("3. Monitoreo Actual\n");
        printf("4. Prediccion de Contaminacion\n");
        printf("5. Alertas\n");
        printf("6. Analisis Historico\n");
        printf("7. Recomendaciones\n");
        printf("8. Informes\n");
        printf("0. Salir\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: 
                agregarNombreCiudad(); 
                break;
            case 2: 
                if (!validarNombreCiudad()) {
                    printf("Error, no se puede continuar si no se agrego el nombre de la ciudad.\n");
                    break;
                }
                menuGestionZonas(); 
                break;
            case 3: 
                if (!validarNombreCiudad()) {
                    printf("Error, no se puede continuar si no se agrego el nombre de la ciudad.\n");
                    break;
                }
                if (!validarMinimoZonas()) {
                    printf("Error, no se puede continuar si no se agregan minimo 5 zonas.\n");
                    break;
                }
                menuMonitoreo(); 
                break;
            case 4: 
                if (!validarNombreCiudad()) {
                    printf("Error, no se puede continuar si no se agrego el nombre de la ciudad.\n");
                    break;
                }
                if (!validarMinimoZonas()) {
                    printf("Error, no se puede continuar si no se agregan minimo 5 zonas.\n");
                    break;
                }
                menuPrediccion(); 
                break;
            case 5: 
                if (!validarNombreCiudad()) {
                    printf("Error, no se puede continuar si no se agrego el nombre de la ciudad.\n");
                    break;
                }
                if (!validarMinimoZonas()) {
                    printf("Error, no se puede continuar si no se agregan minimo 5 zonas.\n");
                    break;
                }
                menuAlertas(); 
                break;
            case 6: 
                if (!validarNombreCiudad()) {
                    printf("Error, no se puede continuar si no se agrego el nombre de la ciudad.\n");
                    break;
                }
                if (!validarMinimoZonas()) {
                    printf("Error, no se puede continuar si no se agregan minimo 5 zonas.\n");
                    break;
                }
                menuAnalisis(); 
                break;
            case 7: 
                if (!validarNombreCiudad()) {
                    printf("Error, no se puede continuar si no se agrego el nombre de la ciudad.\n");
                    break;
                }
                if (!validarMinimoZonas()) {
                    printf("Error, no se puede continuar si no se agregan minimo 5 zonas.\n");
                    break;
                }
                menuRecomendaciones(); 
                break;
            case 8: 
                if (!validarNombreCiudad()) {
                    printf("Error, no se puede continuar si no se agrego el nombre de la ciudad.\n");
                    break;
                }
                if (!validarMinimoZonas()) {
                    printf("Error, no se puede continuar si no se agregan minimo 5 zonas.\n");
                    break;
                }
                menuInformes(); 
                break;
            case 0: 
                printf("Saliendo del sistema...\n");
                guardarDatos();
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while (opcion != 0);
}

void menuGestionZonas() {
    int opcion;
    do {
        printf("\n=== GESTION DE ZONAS (actualmente %d zonas) ===\n", num_zonas);
        printf("1. Agregar zona\n");
        printf("2. Gestion automatica de zonas\n");
        printf("3. Listar zonas\n");
        printf("4. Buscar zona\n");
        printf("5. Modificar zona\n");
        printf("6. Eliminar zona\n");
        printf("0. Volver al menu principal\n");
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: agregarZona(); break;
            case 2: agregarZonasAutomaticas(); break;
            case 3: listarZonas(); break;
            case 4: buscarZona(); break;
            case 5: modificarZona(); break;
            case 6: eliminarZona(); break;
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