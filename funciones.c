#include "funciones.h"
#include <math.h>

Zona zonas[MAX_ZONAS];
Alerta alertas[MAX_ZONAS * MAX_DIAS * 4];
Recomendacion recomendaciones[MAX_ZONAS * 3];
char nombre_ciudad[MAX_NOMBRE] = "";
int num_zonas = 0;
int num_alertas = 0;
int num_recomendaciones = 0;

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leerEntero(const char *mensaje) {
    int valor;
    char buffer[100];
    
    while (1) {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%d", &valor) == 1) {
                return valor;
            }
        }
        printf("Error, Debe ingresar un numero entero valido.\n");
    }
}

float leerFloat(const char *mensaje) {
    float valor;
    char buffer[100];
    
    while (1) {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%f", &valor) == 1) {
                return valor;
            }
        }
        printf("Error, Debe ingresar un numero valido.\n");
    }
}

void leerCadena(const char *mensaje, char *cadena, int longitud) {
    printf("%s", mensaje);
    if (fgets(cadena, longitud, stdin)) {
        size_t len = strlen(cadena);
        if (len > 0 && cadena[len-1] == '\n') {
            cadena[len-1] = '\0';
        } else {
            limpiarBuffer();
        }
    }
}

int validarFecha(const char *fecha) {
    if (strlen(fecha) != 10) return 0;
    if (fecha[2] != '/' || fecha[5] != '/') return 0;
    
    int dia = atoi(fecha);
    int mes = atoi(fecha + 3);
    int anio = atoi(fecha + 6);
    
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || anio < 2000 || anio > 2100) {
        return 0;
    }
    
    return 1;
}

void formatearFecha(time_t fecha, char *fechaStr) {
    struct tm *tm_info = localtime(&fecha);
    strftime(fechaStr, 11, "%d/%m/%Y", tm_info);
}

void agregarNombreCiudad() {
    leerCadena("Ingrese el nombre de la ciudad: ", nombre_ciudad, MAX_NOMBRE);
    printf("Ciudad ingresada exitosamente.\n");
    guardarDatos();
}

void cargarDatos() {
    FILE *archivo;

    archivo = fopen("zonas.dat", "rb");
    if (archivo) {
        num_zonas = fread(zonas, sizeof(Zona), MAX_ZONAS, archivo);
        fclose(archivo);
    }
    archivo = fopen("alertas.dat", "rb");
    if (archivo) {
        num_alertas = fread(alertas, sizeof(Alerta), MAX_ZONAS * 2, archivo);
        fclose(archivo);
    }
    archivo = fopen("recomendaciones.dat", "rb");
    if (archivo) {
        num_recomendaciones = fread(recomendaciones, sizeof(Recomendacion), MAX_ZONAS * 3, archivo);
        fclose(archivo);
    }
    archivo = fopen("ciudad.dat", "rb");
    if (archivo) {
        fread(nombre_ciudad, sizeof(char), MAX_NOMBRE, archivo);
        fclose(archivo);
    }
}

void guardarDatos() {
    FILE *archivo;

    archivo = fopen("zonas.dat", "wb");
    if (archivo) {
        fwrite(zonas, sizeof(Zona), num_zonas, archivo);
        fclose(archivo);
    }

    archivo = fopen("alertas.dat", "wb");
    if (archivo) {
        fwrite(alertas, sizeof(Alerta), num_alertas, archivo);
        fclose(archivo);
    }

    archivo = fopen("recomendaciones.dat", "wb");
    if (archivo) {
        fwrite(recomendaciones, sizeof(Recomendacion), num_recomendaciones, archivo);
        fclose(archivo);
    }

    archivo = fopen("ciudad.dat", "wb");
    if (archivo) {
        fwrite(nombre_ciudad, sizeof(char), MAX_NOMBRE, archivo);
        fclose(archivo);
    }
}

int encontrarSiguienteID() {
    for (int id = 1; id <= 9999; id++) {
        int id_usado = 0;
        for (int i = 0; i < num_zonas; i++) {
            if (zonas[i].id == id) {
                id_usado = 1;
                break;
            }
        }
        if (!id_usado) {
            return id;
        }
    }
    return -1; 
}

void agregarZona() {
    int cantidad = leerEntero("Cuantas zonas desea agregar? ");
    
    if (cantidad <= 0) {
        printf("Cantidad no valida.\n");
        return;
    }
    
    printf("\n=== AGREGANDO %d ZONA(S) ===\n", cantidad);
    
    for (int i = 0; i < cantidad; i++) {
        Zona nueva;
        nueva.id = encontrarSiguienteID();
        
        if (nueva.id == -1) {
            printf("Error: No hay IDs disponibles.\n");
            break;
        }
        
        nueva.num_registros = 0;
        
        printf("\n--- Zona %d de %d ---\n", i + 1, cantidad);
        leerCadena("Nombre de la zona: ", nueva.nombre, MAX_NOMBRE);
        
        zonas[num_zonas++] = nueva;
        printf("Zona agregada exitosamente con ID: %d\n", nueva.id);
        
        
        if (i < cantidad - 1) {
            printf("Siguiente zona...\n");
        }
    }
    
    printf("\n=== RESUMEN ===\n");
    printf("Se agregaron %d zona(s) exitosamente.\n", cantidad);
    printf("Total de zonas registradas: %d\n", num_zonas);
    
    guardarDatos();
}

void listarZonas() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== LISTA DE ZONAS ===\n");
    printf("ID\tNombre\n");
    for (int i = 0; i < num_zonas; i++) {
        printf("%d\t%s\n", zonas[i].id, zonas[i].nombre);
    }
}

void buscarZona() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== BUSCAR ZONA ===\n");
    printf("1. Por ID\n");
    printf("2. Por nombre\n");
    printf("0. Cancelar\n");
    
    int opcion = leerEntero("Seleccione una opcion: ");
    if (opcion == 0) return;
    
    char busqueda[50];
    if (opcion == 1) {
        int id = leerEntero("Ingrese el ID de la zona: ");
        for (int i = 0; i < num_zonas; i++) {
            if (zonas[i].id == id) {
                printf("\nZona encontrada:\n");
                printf("ID: %d\nNombre: %s\n", zonas[i].id, zonas[i].nombre);
                return;
            }
        }
        printf("No se encontro una zona con ID %d\n", id);
    } else if (opcion == 2) {
        leerCadena("Ingrese el nombre o parte del nombre: ", busqueda, 50);
        printf("\nResultados de busqueda:\n");
        int encontrados = 0;
        for (int i = 0; i < num_zonas; i++) {
            if (strstr(zonas[i].nombre, busqueda) != NULL) {
                printf("ID: %d, Nombre: %s\n", zonas[i].id, zonas[i].nombre);
                encontrados++;
            }
        }
        if (encontrados == 0) {
            printf("No se encontraron zonas que coincidan con '%s'\n", busqueda);
        }
    }
}

void modificarZona() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    listarZonas();
    int id = leerEntero("Ingrese el ID de la zona a modificar: ");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].id == id) {
            printf("Modificando zona: %s\n", zonas[i].nombre);
            char nuevoNombre[MAX_NOMBRE];
            leerCadena("Nuevo nombre (dejar vacio para mantener): ", nuevoNombre, MAX_NOMBRE);
            if (strlen(nuevoNombre) > 0) {
                strcpy(zonas[i].nombre, nuevoNombre);
                printf("Zona modificada exitosamente.\n");
                guardarDatos();
            }
            return;
        }
    }
    
    printf("No se encontro una zona con ID %d\n", id);
}

void eliminarZona() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    listarZonas();
    int id = leerEntero("Ingrese el ID de la zona a eliminar: ");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].id == id) {
            for (int j = i; j < num_zonas - 1; j++) {
                zonas[j] = zonas[j + 1];
            }
            num_zonas--;
            printf("Zona eliminada exitosamente.\n");
            guardarDatos();
            return;
        }
    }
    
    printf("No se encontro una zona con ID %d\n", id);
}

void gestionAutomaticaDatos() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas. Agregue zonas primero.\n");
        return;
    }
    
    listarZonas();
    int id = leerEntero("Ingrese el ID de la zona: ");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].id == id) {
            // Limpiar registros existentes
            zonas[i].num_registros = 0;
            
            // Determinar región basada en la ciudad
            char ciudadLower[MAX_NOMBRE];
            strcpy(ciudadLower, nombre_ciudad);
            for (int j = 0; ciudadLower[j]; j++) {
                ciudadLower[j] = tolower(ciudadLower[j]);
            }
            
            int esSierra = 0;
            if (strcmp(ciudadLower, "quito") == 0 || strcmp(ciudadLower, "cuenca") == 0 || 
                strcmp(ciudadLower, "loja") == 0 || strcmp(ciudadLower, "ambato") == 0 || 
                strcmp(ciudadLower, "banos") == 0) {
                esSierra = 1;
            }
            
            // Generar datos para los últimos 30 días
            time_t ahora = time(NULL);
            for (int dia = 29; dia >= 0; dia--) {
                RegistroDiario nuevo;
                
                // Calcular fecha (30 días hacia atrás desde hoy)
                time_t fecha_dia = ahora - (dia * 24 * 3600);
                nuevo.fecha = fecha_dia;
                
                // Generar datos base según región
                if (esSierra) {
                    // Datos típicos de sierra (altitud, clima más fresco)
                    nuevo.niveles.co2 = 380.0 + (rand() % 50);
                    nuevo.niveles.so2 = 0.008 + ((rand() % 20) / 1000.0);
                    nuevo.niveles.no2 = 0.008 + ((rand() % 15) / 1000.0);
                    nuevo.niveles.pm25 = 8.0 + (rand() % 12);
                    
                    nuevo.clima.temperatura = 12.0 + (rand() % 16);
                    nuevo.clima.humedad = 45.0 + (rand() % 30);
                    nuevo.clima.velocidad_viento = 5.0 + (rand() % 15);
                } else {
                    // Datos típicos de costa (nivel del mar, más cálido y húmedo)
                    nuevo.niveles.co2 = 400.0 + (rand() % 60);
                    nuevo.niveles.so2 = 0.012 + ((rand() % 25) / 1000.0);
                    nuevo.niveles.no2 = 0.010 + ((rand() % 18) / 1000.0);
                    nuevo.niveles.pm25 = 12.0 + (rand() % 15);
                    
                    nuevo.clima.temperatura = 22.0 + (rand() % 12);
                    nuevo.clima.humedad = 60.0 + (rand() % 35);
                    nuevo.clima.velocidad_viento = 8.0 + (rand() % 20);
                }
                
                // Variación por zona (cada zona tiene características ligeramente diferentes)
                float variacion_zona = (zonas[i].id % 5) * 0.1;
                nuevo.niveles.co2 += nuevo.niveles.co2 * variacion_zona;
                nuevo.niveles.so2 += nuevo.niveles.so2 * variacion_zona;
                nuevo.niveles.no2 += nuevo.niveles.no2 * variacion_zona;
                nuevo.niveles.pm25 += nuevo.niveles.pm25 * variacion_zona;
                
                nuevo.clima.temperatura += (zonas[i].id % 3) - 1;
                nuevo.clima.humedad += (zonas[i].id % 4) * 2;
                nuevo.clima.velocidad_viento += (zonas[i].id % 3);
                
                // Añadir variación semanal (fines de semana menos contaminación)
                struct tm *tm_info = localtime(&fecha_dia);
                if (tm_info->tm_wday == 0 || tm_info->tm_wday == 6) { // Domingo o Sábado
                    nuevo.niveles.co2 *= 0.85;
                    nuevo.niveles.so2 *= 0.80;
                    nuevo.niveles.no2 *= 0.75;
                    nuevo.niveles.pm25 *= 0.90;
                }
                
                zonas[i].registros[zonas[i].num_registros++] = nuevo;
            }
            
            printf("Se han generado los datos para los ultimos 30 dias de manera exitosa.\n");
            guardarDatos();
            return;
        }
    }
    
    printf("No se encontro una zona con ID %d\n", id);
}

void ingresarDatosContaminacion() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas. Agregue zonas primero.\n");
        return;
    }
    
    listarZonas();
    int id = leerEntero("Ingrese el ID de la zona: ");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].id == id) {
            // Verificar si ya hay datos automáticos
            if (zonas[i].num_registros > 0) {
                printf("Esta zona ya tiene %d registros.\n", zonas[i].num_registros);
                printf("1. Modificar un registro existente\n");
                printf("2. Agregar nuevos registros\n");
                printf("0. Cancelar\n");
                
                int opcion = leerEntero("Seleccione una opcion: ");
                if (opcion == 0) return;
                
                if (opcion == 1) {
                    // Modificar registro existente
                    char fechaBuscar[11];
                    leerCadena("Ingrese la fecha a modificar (DD/MM/AAAA): ", fechaBuscar, 11);
                    while (!validarFecha(fechaBuscar)) {
                        printf("Fecha invalida. Formato DD/MM/AAAA.\n");
                        leerCadena("Ingrese la fecha a modificar (DD/MM/AAAA): ", fechaBuscar, 11);
                    }
                    
                    struct tm tm_buscar = {0};
                    sscanf(fechaBuscar, "%d/%d/%d", &tm_buscar.tm_mday, &tm_buscar.tm_mon, &tm_buscar.tm_year);
                    tm_buscar.tm_mon -= 1;
                    tm_buscar.tm_year -= 1900;
                    time_t fecha_buscar = mktime(&tm_buscar);
                    
                    // Buscar el registro
                    int encontrado = -1;
                    for (int j = 0; j < zonas[i].num_registros; j++) {
                        char fechaExistente[11];
                        formatearFecha(zonas[i].registros[j].fecha, fechaExistente);
                        if (strcmp(fechaBuscar, fechaExistente) == 0) {
                            encontrado = j;
                            break;
                        }
                    }
                    
                    if (encontrado == -1) {
                        printf("No se encontro un registro para la fecha %s\n", fechaBuscar);
                        return;
                    }
                    
                    printf("Modificando datos para %s:\n", fechaBuscar);
                    zonas[i].registros[encontrado].niveles.co2 = leerFloat("CO2 (ppm): ");
                    zonas[i].registros[encontrado].niveles.so2 = leerFloat("SO2 (ppm): ");
                    zonas[i].registros[encontrado].niveles.no2 = leerFloat("NO2 (ppm): ");
                    zonas[i].registros[encontrado].niveles.pm25 = leerFloat("PM2.5 (ug/m3): ");
                    
                    printf("Datos climaticos:\n");
                    zonas[i].registros[encontrado].clima.temperatura = leerFloat("Temperatura (grados C): ");
                    zonas[i].registros[encontrado].clima.humedad = leerFloat("Humedad (%): ");
                    zonas[i].registros[encontrado].clima.velocidad_viento = leerFloat("Velocidad del viento (km/h): ");
                    
                    printf("Registro modificado exitosamente.\n");
                    guardarDatos();
                    return;
                }
                // Si opcion == 2, continúa con el flujo normal para agregar
            }
            
            int numDias;
            do {
                numDias = leerEntero("Cuantos dias de datos desea ingresar (minimo 3, maximo 30): ");
                if (numDias < 3 || numDias > 30) {
                    printf("Debe ingresar entre 3 y 30 dias.\n");
                }
            } while (numDias < 3 || numDias > 30);

            if (zonas[i].num_registros + numDias > MAX_DIAS) {
                printf("No se pueden agregar %d dias. La zona tiene %d registros, maximo %d.\n", 
                       numDias, zonas[i].num_registros, MAX_DIAS);
                return;
            }
            
            printf("\nIngresando datos para %d dias en la zona %s:\n", numDias, zonas[i].nombre);

            for (int dia = 1; dia <= numDias; dia++) {
                RegistroDiario nuevo;
                char fechaStr[11];
                
                printf("\n--- DIA %d de %d ---\n", dia, numDias);
                
                leerCadena("Fecha (DD/MM/AAAA): ", fechaStr, 11);
                while (!validarFecha(fechaStr)) {
                    printf("Fecha invalida. Formato DD/MM/AAAA.\n");
                    leerCadena("Fecha (DD/MM/AAAA): ", fechaStr, 11);
                }
                
                struct tm tm = {0};
                sscanf(fechaStr, "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year);
                tm.tm_mon -= 1;    
                tm.tm_year -= 1900; 
                nuevo.fecha = mktime(&tm);
                
                nuevo.niveles.co2 = leerFloat("CO2 (ppm): ");
                nuevo.niveles.so2 = leerFloat("SO2 (ppm): ");
                nuevo.niveles.no2 = leerFloat("NO2 (ppm): ");
                nuevo.niveles.pm25 = leerFloat("PM2.5 (ug/m3): ");
                
                printf("Ingrese datos climaticos:\n");
                nuevo.clima.temperatura = leerFloat("Temperatura (grados C): ");
                nuevo.clima.humedad = leerFloat("Humedad (%): ");
                nuevo.clima.velocidad_viento = leerFloat("Velocidad del viento (km/h): ");
                
                zonas[i].registros[zonas[i].num_registros++] = nuevo;
                printf("Datos del dia %d registrados exitosamente.\n", dia);
            }
            
            printf("\nTodos los datos han sido registrados exitosamente.\n");
            printf("Total de registros en la zona: %d\n", zonas[i].num_registros);
            guardarDatos();
            return;
        }
    }
    
    printf("No se encontro una zona con ID %d\n", id);
}

void mostrarContaminacionActual() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== CONTAMINACION ACTUAL ===\n");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].num_registros == 0) {
            printf("Zona %s: No hay datos registrados.\n", zonas[i].nombre);
            continue;
        }
        
        RegistroDiario ultimo = zonas[i].registros[zonas[i].num_registros - 1];
        char fechaStr[11];
        formatearFecha(ultimo.fecha, fechaStr);
        
        printf("\nZona: %s (%s)\n", zonas[i].nombre, fechaStr);
        printf("Contaminantes:\n");
        printf("  CO2: %.2f ppm\n", ultimo.niveles.co2);
        printf("  SO2: %.3f ppm\n", ultimo.niveles.so2);
        printf("  NO2: %.3f ppm\n", ultimo.niveles.no2);
        printf("  PM2.5: %.2f ug/m3\n", ultimo.niveles.pm25);
        printf("Clima:\n");
        printf("  Temperatura: %.1f grados C\n", ultimo.clima.temperatura);
        printf("  Humedad: %.1f%%\n", ultimo.clima.humedad);
        printf("  Viento: %.1f km/h\n", ultimo.clima.velocidad_viento);
    }
}

void mostrarContaminacionHistorica() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== CONTAMINACION HISTORICA ===\n");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].num_registros == 0) {
            printf("Zona %s: No hay datos registrados.\n", zonas[i].nombre);
            continue;
        }
        
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("Total de registros: %d\n", zonas[i].num_registros);
        printf("----------------------------------------\n");
        
        for (int j = 0; j < zonas[i].num_registros; j++) {
            char fechaStr[11];
            formatearFecha(zonas[i].registros[j].fecha, fechaStr);
            
            printf("\nDia %d (%s):\n", j + 1, fechaStr);
            printf("Contaminantes:\n");
            printf("  CO2: %.2f ppm\n", zonas[i].registros[j].niveles.co2);
            printf("  SO2: %.3f ppm\n", zonas[i].registros[j].niveles.so2);
            printf("  NO2: %.3f ppm\n", zonas[i].registros[j].niveles.no2);
            printf("  PM2.5: %.2f ug/m3\n", zonas[i].registros[j].niveles.pm25);
            printf("Clima:\n");
            printf("  Temperatura: %.1f grados C\n", zonas[i].registros[j].clima.temperatura);
            printf("  Humedad: %.1f%%\n", zonas[i].registros[j].clima.humedad);
            printf("  Viento: %.1f km/h\n", zonas[i].registros[j].clima.velocidad_viento);
        }
        printf("========================================\n");
    }
}

void compararConLimitesOMS() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== COMPARACION CON LIMITES OMS ===\n");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].num_registros == 0) {
            printf("Zona %s: No hay datos registrados.\n", zonas[i].nombre);
            continue;
        }
        
        RegistroDiario ultimo = zonas[i].registros[zonas[i].num_registros - 1];
        char fechaStr[11];
        formatearFecha(ultimo.fecha, fechaStr);
        
        printf("\nZona: %s (%s)\n", zonas[i].nombre, fechaStr);
        
        printf("CO2: %.2f ppm ", ultimo.niveles.co2);
        if (ultimo.niveles.co2 > LIMITE_CO2) {
            printf("(EXCEDE el limite OMS de %.1f ppm)\n", LIMITE_CO2);
        } else {
            printf("(Dentro del limite OMS)\n");
        }
        
        printf("SO2: %.3f ppm ", ultimo.niveles.so2);
        if (ultimo.niveles.so2 > LIMITE_SO2) {
            printf("(EXCEDE el limite OMS de %.3f ppm)\n", LIMITE_SO2);
        } else {
            printf("(Dentro del limite OMS)\n");
        }
        
        printf("NO2: %.3f ppm ", ultimo.niveles.no2);
        if (ultimo.niveles.no2 > LIMITE_NO2) {
            printf("(EXCEDE el limite OMS de %.3f ppm)\n", LIMITE_NO2);
        } else {
            printf("(Dentro del limite OMS)\n");
        }
        
        printf("PM2.5: %.2f ug/m3 ", ultimo.niveles.pm25);
        if (ultimo.niveles.pm25 > LIMITE_PM25) {
            printf("(EXCEDE el limite OMS de %.1f ug/m3)\n", LIMITE_PM25);
        } else {
            printf("(Dentro del limite OMS)\n");
        }
    }
}

void ingresarDatosClimaticos() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    listarZonas();
    int id = leerEntero("Ingrese el ID de la zona: ");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].id == id) {
            if (zonas[i].num_registros == 0) {
                printf("No hay registros de contaminacion para esta zona.\n");
                return;
            }
            
            RegistroDiario *ultimo = &zonas[i].registros[zonas[i].num_registros - 1];
            
            printf("Ingrese datos climaticos actualizados para la zona %s:\n", zonas[i].nombre);
            ultimo->clima.temperatura = leerFloat("Temperatura (grados C): ");
            ultimo->clima.humedad = leerFloat("Humedad (%): ");
            ultimo->clima.velocidad_viento = leerFloat("Velocidad del viento (km/h): ");
            
            printf("Datos climaticos actualizados exitosamente.\n");
            guardarDatos();
            return;
        }
    }
    
    printf("No se encontro una zona con ID %d\n", id);
}

void predecirContaminacion() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== PREDICCION DE CONTAMINACION PARA LAS PROXIMAS 24 HORAS ===\n");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].num_registros < 3) {
            printf("Zona %s: No hay suficientes datos historicos (minimo 3 dias)\n", zonas[i].nombre);
            continue;
        }

        RegistroDiario *ultimos[3];
        for (int j = 0; j < 3; j++) {
            ultimos[j] = &zonas[i].registros[zonas[i].num_registros - 1 - j];
        }

        Contaminantes prediccion;
        prediccion.co2 = (ultimos[0]->niveles.co2 * 0.5 + ultimos[1]->niveles.co2 * 0.3 + ultimos[2]->niveles.co2 * 0.2);
        prediccion.so2 = (ultimos[0]->niveles.so2 * 0.5 + ultimos[1]->niveles.so2 * 0.3 + ultimos[2]->niveles.so2 * 0.2);
        prediccion.no2 = (ultimos[0]->niveles.no2 * 0.5 + ultimos[1]->niveles.no2 * 0.3 + ultimos[2]->niveles.no2 * 0.2);
        prediccion.pm25 = (ultimos[0]->niveles.pm25 * 0.5 + ultimos[1]->niveles.pm25 * 0.3 + ultimos[2]->niveles.pm25 * 0.2);

        float factor_clima = 1.0;
        RegistroDiario *actual = ultimos[0];

        if (actual->clima.velocidad_viento < 5.0) factor_clima *= 1.2;
        else if (actual->clima.velocidad_viento > 15.0) factor_clima *= 0.8;

        if (actual->clima.humedad < 30.0) factor_clima *= 1.1;

        prediccion.co2 *= factor_clima;
        prediccion.so2 *= factor_clima;
        prediccion.no2 *= factor_clima;
        prediccion.pm25 *= factor_clima;

        printf("\nZona: %s\n", zonas[i].nombre);
        printf("Prediccion para las proximas 24 horas:\n");
        printf("  CO2: %.2f ppm\n", prediccion.co2);
        printf("  SO2: %.3f ppm\n", prediccion.so2);
        printf("  NO2: %.3f ppm\n", prediccion.no2);
        printf("  PM2.5: %.2f ug/m3\n", prediccion.pm25);
        
        
        if (prediccion.co2 > LIMITE_CO2 || prediccion.so2 > LIMITE_SO2 || 
            prediccion.no2 > LIMITE_NO2 || prediccion.pm25 > LIMITE_PM25) {
            printf("  ALERTA: Se preve que se excederan los limites OMS\n");
        }
    }
}

void generarAlertas() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    num_alertas = 0; 
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].num_registros == 0) continue;

        for (int dia = 0; dia < zonas[i].num_registros; dia++) {
            RegistroDiario *actual = &zonas[i].registros[dia];

            if (actual->niveles.co2 > LIMITE_CO2) {
                Alerta alerta;
                snprintf(alerta.mensaje, sizeof(alerta.mensaje), 
                        "ALERTA: Zona %s - CO2 actual (%.2f ppm) excede limite OMS (%.1f ppm)", 
                        zonas[i].nombre, actual->niveles.co2, LIMITE_CO2);
                alerta.fecha = actual->fecha;
                alerta.prioridad = 2;
                alertas[num_alertas++] = alerta;
            }
            
            if (actual->niveles.so2 > LIMITE_SO2) {
                Alerta alerta;
                snprintf(alerta.mensaje, sizeof(alerta.mensaje), 
                        "ALERTA: Zona %s - SO2 actual (%.3f ppm) excede limite OMS (%.3f ppm)", 
                        zonas[i].nombre, actual->niveles.so2, LIMITE_SO2);
                alerta.fecha = actual->fecha;
                alerta.prioridad = 2;
                alertas[num_alertas++] = alerta;
            }
            
            if (actual->niveles.no2 > LIMITE_NO2) {
                Alerta alerta;
                snprintf(alerta.mensaje, sizeof(alerta.mensaje), 
                        "ALERTA: Zona %s - NO2 actual (%.3f ppm) excede limite OMS (%.3f ppm)", 
                        zonas[i].nombre, actual->niveles.no2, LIMITE_NO2);
                alerta.fecha = actual->fecha;
                alerta.prioridad = 2;
                alertas[num_alertas++] = alerta;
            }
            
            if (actual->niveles.pm25 > LIMITE_PM25) {
                Alerta alerta;
                snprintf(alerta.mensaje, sizeof(alerta.mensaje), 
                        "ALERTA: Zona %s - PM2.5 actual (%.2f ug/m3) excede limite OMS (%.1f ug/m3)", 
                        zonas[i].nombre, actual->niveles.pm25, LIMITE_PM25);
                alerta.fecha = actual->fecha;
                alerta.prioridad = 2;
                alertas[num_alertas++] = alerta;
            }

            if (dia >= 2) {
                RegistroDiario *ultimos[3];
                for (int j = 0; j < 3; j++) {
                    ultimos[j] = &zonas[i].registros[dia - j];
                }
                
                Contaminantes prediccion;
                prediccion.co2 = (ultimos[0]->niveles.co2 * 0.5 + ultimos[1]->niveles.co2 * 0.3 + ultimos[2]->niveles.co2 * 0.2);
                prediccion.so2 = (ultimos[0]->niveles.so2 * 0.5 + ultimos[1]->niveles.so2 * 0.3 + ultimos[2]->niveles.so2 * 0.2);
                prediccion.no2 = (ultimos[0]->niveles.no2 * 0.5 + ultimos[1]->niveles.no2 * 0.3 + ultimos[2]->niveles.no2 * 0.2);
                prediccion.pm25 = (ultimos[0]->niveles.pm25 * 0.5 + ultimos[1]->niveles.pm25 * 0.3 + ultimos[2]->niveles.pm25 * 0.2);
                
                float factor_clima = 1.0;
                if (actual->clima.velocidad_viento < 5.0) factor_clima *= 1.2;
                else if (actual->clima.velocidad_viento > 15.0) factor_clima *= 0.8;
                if (actual->clima.humedad < 30.0) factor_clima *= 1.1;
                
                prediccion.co2 *= factor_clima;
                prediccion.so2 *= factor_clima;
                prediccion.no2 *= factor_clima;
                prediccion.pm25 *= factor_clima;
                
                if (prediccion.co2 > LIMITE_CO2) {
                    Alerta alerta;
                    snprintf(alerta.mensaje, sizeof(alerta.mensaje), 
                            "PREDICCION: Zona %s - CO2 previsto (%.2f ppm) excedera limite OMS (%.1f ppm)", 
                            zonas[i].nombre, prediccion.co2, LIMITE_CO2);
                    alerta.fecha = actual->fecha;
                    alerta.prioridad = 1;
                    alertas[num_alertas++] = alerta;
                }
                
                if (prediccion.so2 > LIMITE_SO2) {
                    Alerta alerta;
                    snprintf(alerta.mensaje, sizeof(alerta.mensaje), 
                            "PREDICCION: Zona %s - SO2 previsto (%.3f ppm) excedera limite OMS (%.3f ppm)", 
                            zonas[i].nombre, prediccion.so2, LIMITE_SO2);
                    alerta.fecha = actual->fecha;
                    alerta.prioridad = 1;
                    alertas[num_alertas++] = alerta;
                }
                
                if (prediccion.no2 > LIMITE_NO2) {
                    Alerta alerta;
                    snprintf(alerta.mensaje, sizeof(alerta.mensaje), 
                            "PREDICCION: Zona %s - NO2 previsto (%.3f ppm) excedera limite OMS (%.3f ppm)", 
                            zonas[i].nombre, prediccion.no2, LIMITE_NO2);
                    alerta.fecha = actual->fecha;
                    alerta.prioridad = 1;
                    alertas[num_alertas++] = alerta;
                }
                
                if (prediccion.pm25 > LIMITE_PM25) {
                    Alerta alerta;
                    snprintf(alerta.mensaje, sizeof(alerta.mensaje), 
                            "PREDICCION: Zona %s - PM2.5 previsto (%.2f ug/m3) excedera limite OMS (%.1f ug/m3)", 
                            zonas[i].nombre, prediccion.pm25, LIMITE_PM25);
                    alerta.fecha = actual->fecha;
                    alerta.prioridad = 1;
                    alertas[num_alertas++] = alerta;
                }
            }
        }
    }
    
    printf("Se generaron %d alertas historicas.\n", num_alertas);
    guardarDatos();
}

void listarAlertas() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    if (num_alertas == 0) {
        printf("No hay alertas registradas.\n");
        printf("Genere alertas primero usando la opcion 1.\n");
        return;
    }
    
    listarZonas();
    int id = leerEntero("Ingrese el ID de la zona para ver alertas: ");

    char nombreZona[MAX_NOMBRE];
    int zonaEncontrada = 0;
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].id == id) {
            strcpy(nombreZona, zonas[i].nombre);
            zonaEncontrada = 1;
            break;
        }
    }
    
    if (!zonaEncontrada) {
        printf("No se encontro una zona con ID %d\n", id);
        return;
    }
    
    printf("\n=== ALERTAS %s ===\n", nombreZona);

    Alerta alertasZona[MAX_DIAS * 4];
    int numAlertasZona = 0;
    
    // Buscar alertas que contengan el nombre de la zona
    for (int i = 0; i < num_alertas; i++) {
        // Crear un patrón de búsqueda más específico
        char patron[100];
        snprintf(patron, sizeof(patron), "Zona %s", nombreZona);
        
        if (strstr(alertas[i].mensaje, patron) != NULL) {
            alertasZona[numAlertasZona++] = alertas[i];
        }
    }
    
    if (numAlertasZona == 0) {
        printf("No hay alertas para la zona %s\n", nombreZona);
        printf("Total de alertas en el sistema: %d\n", num_alertas);
        
        // Debug: mostrar algunas alertas para verificar el formato
        printf("\nEjemplos de alertas en el sistema:\n");
        for (int i = 0; i < (num_alertas < 3 ? num_alertas : 3); i++) {
            printf("Alerta %d: %s\n", i+1, alertas[i].mensaje);
        }
        return;
    }

    // Ordenar alertas por fecha (más recientes primero)
    for (int i = 0; i < numAlertasZona - 1; i++) {
        for (int j = i + 1; j < numAlertasZona; j++) {
            if (alertasZona[i].fecha < alertasZona[j].fecha) {
                Alerta temp = alertasZona[i];
                alertasZona[i] = alertasZona[j];
                alertasZona[j] = temp;
            }
        }
    }

    time_t fechaActual = 0;
    for (int i = 0; i < numAlertasZona; i++) {
        if (alertasZona[i].fecha != fechaActual) {
            fechaActual = alertasZona[i].fecha;
            char fechaStr[20];
            formatearFecha(fechaActual, fechaStr);
            printf("\nFecha: %s\n", fechaStr);
        }
        
        char fechaStr[20];
        formatearFecha(alertasZona[i].fecha, fechaStr);
        printf("[%s] ", fechaStr);
        
        switch (alertasZona[i].prioridad) {
            case 1: printf("(BAJA) "); break;
            case 2: printf("(MEDIA) "); break;
            case 3: printf("(ALTA) "); break;
        }
        printf("%s\n", alertasZona[i].mensaje);
    }
    
    printf("\nTotal de alertas para %s: %d\n", nombreZona, numAlertasZona);
}

void calcularPromediosHistoricos() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== PROMEDIOS HISTORICOS (ULTIMOS 30 DIAS) ===\n");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].num_registros == 0) {
            printf("Zona %s: No hay datos registrados.\n", zonas[i].nombre);
            continue;
        }
        
        int dias = zonas[i].num_registros > 30 ? 30 : zonas[i].num_registros;
        Contaminantes promedio = {0};
        
        for (int j = zonas[i].num_registros - 1; j >= zonas[i].num_registros - dias; j--) {
            promedio.co2 += zonas[i].registros[j].niveles.co2;
            promedio.so2 += zonas[i].registros[j].niveles.so2;
            promedio.no2 += zonas[i].registros[j].niveles.no2;
            promedio.pm25 += zonas[i].registros[j].niveles.pm25;
        }
        
        promedio.co2 /= dias;
        promedio.so2 /= dias;
        promedio.no2 /= dias;
        promedio.pm25 /= dias;
        
        printf("\nZona: %s (Ultimos %d dias)\n", zonas[i].nombre, dias);
        printf("Promedios:\n");
        printf("  CO2: %.2f ppm\n", promedio.co2);
        printf("  SO2: %.3f ppm\n", promedio.so2);
        printf("  NO2: %.3f ppm\n", promedio.no2);
        printf("  PM2.5: %.2f ug/m3\n", promedio.pm25);
        
        printf("Comparacion con limites OMS:\n");
        printf("  CO2: %.2f ppm vs %.1f ppm\n", promedio.co2, LIMITE_CO2);
        printf("  SO2: %.3f ppm vs %.3f ppm\n", promedio.so2, LIMITE_SO2);
        printf("  NO2: %.3f ppm vs %.3f ppm\n", promedio.no2, LIMITE_NO2);
        printf("  PM2.5: %.2f ug/m3 vs %.1f ug/m3\n", promedio.pm25, LIMITE_PM25);
        
        if (promedio.co2 > LIMITE_CO2 || promedio.so2 > LIMITE_SO2 || 
            promedio.no2 > LIMITE_NO2 || promedio.pm25 > LIMITE_PM25) {
            printf("  ADVERTENCIA: Promedios historicos exceden limites OMS\n");
        }
    }
}

void mostrarTendencias() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== ANALISIS DE TENDENCIAS ===\n");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].num_registros < 3) {
            printf("\nZona: %s\n", zonas[i].nombre);
            printf("Datos insuficientes para analizar tendencias (minimo 3 registros).\n");
            continue;
        }
        
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("Registros disponibles: %d\n", zonas[i].num_registros);
        
        float co2_inicial = zonas[i].registros[0].niveles.co2;
        float co2_final = zonas[i].registros[zonas[i].num_registros - 1].niveles.co2;
        float tendencia_co2 = ((co2_final - co2_inicial) / co2_inicial) * 100;
        
        printf("\nTendencias de contaminantes:\n");
        printf("  CO2: %.2f ppm -> %.2f ppm (", co2_inicial, co2_final);
        if (tendencia_co2 > 5) {
            printf("AUMENTO del %.1f%%)\n", tendencia_co2);
        } else if (tendencia_co2 < -5) {
            printf("DISMINUCION del %.1f%%)\n", -tendencia_co2);
        } else {
            printf("ESTABLE %.1f%%)\n", tendencia_co2);
        }
        
        float so2_inicial = zonas[i].registros[0].niveles.so2;
        float so2_final = zonas[i].registros[zonas[i].num_registros - 1].niveles.so2;
        float tendencia_so2 = ((so2_final - so2_inicial) / so2_inicial) * 100;
        
        printf("  SO2: %.3f ppm -> %.3f ppm (", so2_inicial, so2_final);
        if (tendencia_so2 > 5) {
            printf("AUMENTO del %.1f%%)\n", tendencia_so2);
        } else if (tendencia_so2 < -5) {
            printf("DISMINUCION del %.1f%%)\n", -tendencia_so2);
        } else {
            printf("ESTABLE %.1f%%)\n", tendencia_so2);
        }
        
        float no2_inicial = zonas[i].registros[0].niveles.no2;
        float no2_final = zonas[i].registros[zonas[i].num_registros - 1].niveles.no2;
        float tendencia_no2 = ((no2_final - no2_inicial) / no2_inicial) * 100;
        
        printf("  NO2: %.3f ppm -> %.3f ppm (", no2_inicial, no2_final);
        if (tendencia_no2 > 5) {
            printf("AUMENTO del %.1f%%)\n", tendencia_no2);
        } else if (tendencia_no2 < -5) {
            printf("DISMINUCION del %.1f%%)\n", -tendencia_no2);
        } else {
            printf("ESTABLE %.1f%%)\n", tendencia_no2);
        }
        
        float pm25_inicial = zonas[i].registros[0].niveles.pm25;
        float pm25_final = zonas[i].registros[zonas[i].num_registros - 1].niveles.pm25;
        float tendencia_pm25 = ((pm25_final - pm25_inicial) / pm25_inicial) * 100;
        
        printf("  PM2.5: %.2f ug/m3 -> %.2f ug/m3 (", pm25_inicial, pm25_final);
        if (tendencia_pm25 > 5) {
            printf("AUMENTO del %.1f%%)\n", tendencia_pm25);
        } else if (tendencia_pm25 < -5) {
            printf("DISMINUCION del %.1f%%)\n", -tendencia_pm25);
        } else {
            printf("ESTABLE %.1f%%)\n", tendencia_pm25);
        }
        
        printf("\nEvaluacion general:\n");
        int contaminantes_aumentando = 0;
        if (tendencia_co2 > 5) contaminantes_aumentando++;
        if (tendencia_so2 > 5) contaminantes_aumentando++;
        if (tendencia_no2 > 5) contaminantes_aumentando++;
        if (tendencia_pm25 > 5) contaminantes_aumentando++;
        
        if (contaminantes_aumentando >= 3) {
            printf("  ALERTA: Tendencia general de EMPEORAMIENTO\n");
        } else if (contaminantes_aumentando >= 2) {
            printf("  CUIDADO: Tendencia mixta con empeoramiento parcial\n");
        } else {
            printf("  BUENO: Tendencia general estable o de mejoramiento\n");
        }
        
        printf("----------------------------------------\n");
    }
}

void generarRecomendaciones() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    num_recomendaciones = 0;
    time_t ahora = time(NULL);
    
    printf("\n=== GENERANDO RECOMENDACIONES ===\n");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].num_registros == 0) {
            printf("Zona %s: No hay datos para generar recomendaciones.\n", zonas[i].nombre);
            continue;
        }
        
        RegistroDiario *actual = &zonas[i].registros[zonas[i].num_registros - 1];
        
        if (actual->niveles.co2 > LIMITE_CO2 || actual->niveles.so2 > LIMITE_SO2 || 
            actual->niveles.no2 > LIMITE_NO2 || actual->niveles.pm25 > LIMITE_PM25) {
            
            if (num_recomendaciones < MAX_ZONAS * 3) {
                strcpy(recomendaciones[num_recomendaciones].zona, zonas[i].nombre);
                strcpy(recomendaciones[num_recomendaciones].recomendacion, 
                       "Contaminacion elevada: Reducir trafico vehicular, limitar actividades industriales, evitar ejercicio al aire libre");
                recomendaciones[num_recomendaciones].fecha = ahora;
                recomendaciones[num_recomendaciones].tipo = 2;
                num_recomendaciones++;
            }
            
            if (actual->niveles.pm25 > LIMITE_PM25 && num_recomendaciones < MAX_ZONAS * 3) {
                strcpy(recomendaciones[num_recomendaciones].zona, zonas[i].nombre);
                strcpy(recomendaciones[num_recomendaciones].recomendacion, 
                       "Particulas PM2.5 elevadas: Usar mascarillas de proteccion contra particulas finas");
                recomendaciones[num_recomendaciones].fecha = ahora;
                recomendaciones[num_recomendaciones].tipo = 3;
                num_recomendaciones++;
            }
            
            if (actual->clima.velocidad_viento < 5.0 && num_recomendaciones < MAX_ZONAS * 3) {
                strcpy(recomendaciones[num_recomendaciones].zona, zonas[i].nombre);
                strcpy(recomendaciones[num_recomendaciones].recomendacion, 
                       "Viento bajo: Esperar condiciones de mayor ventilacion para actividades contaminantes");
                recomendaciones[num_recomendaciones].fecha = ahora;
                recomendaciones[num_recomendaciones].tipo = 1;
                num_recomendaciones++;
            }
        }
    }
    
    printf("Se generaron %d recomendaciones.\n", num_recomendaciones);
    guardarDatos();
}

void mostrarRecomendaciones() {
    if (num_recomendaciones == 0) {
        printf("No hay recomendaciones disponibles.\n");
        printf("Genere recomendaciones primero usando la opcion 1.\n");
        return;
    }
    
    printf("\n=== RECOMENDACIONES ACTUALES ===\n");
    
    for (int i = 0; i < num_recomendaciones - 1; i++) {
        for (int j = i + 1; j < num_recomendaciones; j++) {
            if (recomendaciones[i].tipo < recomendaciones[j].tipo) {
                Recomendacion temp = recomendaciones[i];
                recomendaciones[i] = recomendaciones[j];
                recomendaciones[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < num_recomendaciones; i++) {
        char fechaStr[20];
        formatearFecha(recomendaciones[i].fecha, fechaStr);
        
        printf("\n[%s] Zona: %s\n", fechaStr, recomendaciones[i].zona);
        
        switch (recomendaciones[i].tipo) {
            case 1:
                printf("Tipo: PREVENTIVA\n");
                break;
            case 2:
                printf("Tipo: CORRECTIVA\n");
                break;
            case 3:
                printf("Tipo: URGENTE\n");
                break;
            default:
                printf("Tipo: GENERAL\n");
        }
        
        printf("Recomendacion: %s\n", recomendaciones[i].recomendacion);
        printf("----------------------------------------\n");
    }
    
    printf("\nTotal de recomendaciones: %d\n", num_recomendaciones);
}

void mostrarPredicciones() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== PREDICCIONES GUARDADAS ===\n");
    printf("Esta funcion mostraria las predicciones calculadas previamente.\n");
    printf("Use la opcion 1 para generar nuevas predicciones.\n");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].num_registros >= 3) {
            printf("\nZona: %s\n", zonas[i].nombre);
            printf("Datos disponibles para prediccion: %d registros\n", zonas[i].num_registros);
            printf("Estado: Listo para generar predicciones\n");
        } else {
            printf("\nZona: %s\n", zonas[i].nombre);
            printf("Datos insuficientes para prediccion (necesita minimo 3 registros)\n");
        }
    }
}

void generarInformeDiario() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    char nombreArchivo[50];
    time_t ahora = time(NULL);
    struct tm *tm_info = localtime(&ahora);
    strftime(nombreArchivo, 50, "informe_%Y%m%d.txt", tm_info);
    
    FILE *archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("Error al crear el archivo de informe.\n");
        return;
    }
    
    fprintf(archivo, "=== INFORME DIARIO DE CONTAMINACION ===\n");
    char fechaActual[20];
    strftime(fechaActual, 20, "%d/%m/%Y", tm_info);
    fprintf(archivo, "Fecha: %s\n\n", fechaActual);
    
    for (int i = 0; i < num_zonas; i++) {
        fprintf(archivo, "Zona: %s\n", zonas[i].nombre);
        if (zonas[i].num_registros == 0) {
            fprintf(archivo, "  No hay datos registrados.\n\n");
            continue;
        }
        
        RegistroDiario ultimo = zonas[i].registros[zonas[i].num_registros - 1];
        char fechaStr[11];
        formatearFecha(ultimo.fecha, fechaStr);
        
        fprintf(archivo, "  Ultimo registro: %s\n", fechaStr);
        fprintf(archivo, "  CO2: %.2f ppm\n", ultimo.niveles.co2);
        fprintf(archivo, "  SO2: %.3f ppm\n", ultimo.niveles.so2);
        fprintf(archivo, "  NO2: %.3f ppm\n", ultimo.niveles.no2);
        fprintf(archivo, "  PM2.5: %.2f ug/m3\n", ultimo.niveles.pm25);
        fprintf(archivo, "  Temperatura: %.1f grados C\n", ultimo.clima.temperatura);
        fprintf(archivo, "  Humedad: %.1f%%\n", ultimo.clima.humedad);
        fprintf(archivo, "  Viento: %.1f km/h\n\n", ultimo.clima.velocidad_viento);
    }
    
    fclose(archivo);
    printf("Informe diario generado en %s\n", nombreArchivo);
}

void generarInformeHistorico() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    char nombreArchivo[50];
    time_t ahora = time(NULL);
    struct tm *tm_info = localtime(&ahora);
    strftime(nombreArchivo, 50, "historico_%Y%m%d.txt", tm_info);
    
    FILE *archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("Error al crear el archivo de informe.\n");
        return;
    }
    
    fprintf(archivo, "=== INFORME HISTORICO DE CONTAMINACION ===\n");
    char fechaActual[20];
    strftime(fechaActual, 20, "%d/%m/%Y", tm_info);
    fprintf(archivo, "Generado: %s\n\n", fechaActual);
    
    for (int i = 0; i < num_zonas; i++) {
        fprintf(archivo, "Zona: %s\n", zonas[i].nombre);
        fprintf(archivo, "Total de registros: %d\n", zonas[i].num_registros);
        
        if (zonas[i].num_registros == 0) {
            fprintf(archivo, "  No hay datos historicos.\n\n");
            continue;
        }
        
        for (int j = 0; j < zonas[i].num_registros; j++) {
            char fechaStr[11];
            formatearFecha(zonas[i].registros[j].fecha, fechaStr);
            
            fprintf(archivo, "\n  Registro %d (%s):\n", j + 1, fechaStr);
            fprintf(archivo, "    CO2: %.2f ppm\n", zonas[i].registros[j].niveles.co2);
            fprintf(archivo, "    SO2: %.3f ppm\n", zonas[i].registros[j].niveles.so2);
            fprintf(archivo, "    NO2: %.3f ppm\n", zonas[i].registros[j].niveles.no2);
            fprintf(archivo, "    PM2.5: %.2f ug/m3\n", zonas[i].registros[j].niveles.pm25);
            fprintf(archivo, "    Temperatura: %.1f grados C\n", zonas[i].registros[j].clima.temperatura);
            fprintf(archivo, "    Humedad: %.1f%%\n", zonas[i].registros[j].clima.humedad);
            fprintf(archivo, "    Viento: %.1f km/h\n", zonas[i].registros[j].clima.velocidad_viento);
        }
        fprintf(archivo, "\n");
    }
    
    fclose(archivo);
    printf("Informe historico generado en %s\n", nombreArchivo);
}

void compararDiasIngresados() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    listarZonas();
    int id = leerEntero("Ingrese el ID de la zona para comparar: ");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].id == id) {
            if (zonas[i].num_registros < 2) {
                printf("La zona debe tener al menos 2 registros para comparar.\n");
                return;
            }
            
            printf("\n=== COMPARACION DE DIAS INGRESADOS ===\n");
            printf("Zona: %s\n", zonas[i].nombre);
            printf("Total de registros: %d\n\n", zonas[i].num_registros);
            
            for (int j = 0; j < zonas[i].num_registros; j++) {
                char fechaStr[11];
                formatearFecha(zonas[i].registros[j].fecha, fechaStr);
                
                printf("Dia %d (%s):\n", j + 1, fechaStr);
                printf("  CO2: %.2f ppm\n", zonas[i].registros[j].niveles.co2);
                printf("  SO2: %.3f ppm\n", zonas[i].registros[j].niveles.so2);
                printf("  NO2: %.3f ppm\n", zonas[i].registros[j].niveles.no2);
                printf("  PM2.5: %.2f ug/m3\n", zonas[i].registros[j].niveles.pm25);
                printf("  Temperatura: %.1f grados C\n", zonas[i].registros[j].clima.temperatura);
                printf("  Humedad: %.1f%%\n", zonas[i].registros[j].clima.humedad);
                printf("  Viento: %.1f km/h\n", zonas[i].registros[j].clima.velocidad_viento);
                printf("\n");
            }
            
            printf("=== ANALISIS COMPARATIVO ===\n");
            if (zonas[i].num_registros >= 2) {
                RegistroDiario primer = zonas[i].registros[0];
                RegistroDiario ultimo = zonas[i].registros[zonas[i].num_registros - 1];
                
                printf("Cambio del primer al ultimo dia:\n");
                printf("  CO2: %.2f -> %.2f (%.2f%%)\n", primer.niveles.co2, ultimo.niveles.co2, 
                       ((ultimo.niveles.co2 - primer.niveles.co2) / primer.niveles.co2) * 100);
                printf("  SO2: %.3f -> %.3f (%.2f%%)\n", primer.niveles.so2, ultimo.niveles.so2,
                       ((ultimo.niveles.so2 - primer.niveles.so2) / primer.niveles.so2) * 100);
                printf("  NO2: %.3f -> %.3f (%.2f%%)\n", primer.niveles.no2, ultimo.niveles.no2,
                       ((ultimo.niveles.no2 - primer.niveles.no2) / primer.niveles.no2) * 100);
                printf("  PM2.5: %.2f -> %.2f (%.2f%%)\n", primer.niveles.pm25, ultimo.niveles.pm25,
                       ((ultimo.niveles.pm25 - primer.niveles.pm25) / primer.niveles.pm25) * 100);
                
                printf("\nCambios climaticos:\n");
                printf("  Temperatura: %.1f -> %.1f (%.2f%%)\n", primer.clima.temperatura, ultimo.clima.temperatura,
                       ((ultimo.clima.temperatura - primer.clima.temperatura) / primer.clima.temperatura) * 100);
                printf("  Humedad: %.1f -> %.1f (%.2f%%)\n", primer.clima.humedad, ultimo.clima.humedad,
                       ((ultimo.clima.humedad - primer.clima.humedad) / primer.clima.humedad) * 100);
                printf("  Viento: %.1f -> %.1f (%.2f%%)\n", primer.clima.velocidad_viento, ultimo.clima.velocidad_viento,
                       ((ultimo.clima.velocidad_viento - primer.clima.velocidad_viento) / primer.clima.velocidad_viento) * 100);
            }
            return;
        }
    }
    
    printf("No se encontro una zona con ID %d\n", id);
}

void exportarZonasTexto() {
    char nombreArchivo[50];
    time_t ahora = time(NULL);
    struct tm *tm_info = localtime(&ahora);
    strftime(nombreArchivo, 50, "zonas_%Y%m%d.txt", tm_info);
    
    FILE *archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("Error al crear el archivo de zonas.\n");
        return;
    }
    
    fprintf(archivo, "=== INFORME DE ZONAS REGISTRADAS ===\n");
    fprintf(archivo, "Fecha de generacion: ");
    char fechaGen[50];
    strftime(fechaGen, 50, "%d/%m/%Y %H:%M:%S", tm_info);
    fprintf(archivo, "%s\n\n", fechaGen);
    
    fprintf(archivo, "Total de zonas: %d\n\n", num_zonas);
    
    for (int i = 0; i < num_zonas; i++) {
        fprintf(archivo, "Zona ID: %d\n", zonas[i].id);
        fprintf(archivo, "Nombre: %s\n", zonas[i].nombre);
        fprintf(archivo, "Registros de contaminacion: %d\n", zonas[i].num_registros);
        
        if (zonas[i].num_registros > 0) {
            RegistroDiario ultimo = zonas[i].registros[zonas[i].num_registros - 1];
            char fechaStr[11];
            formatearFecha(ultimo.fecha, fechaStr);
            
            fprintf(archivo, "Ultimo registro (%s):\n", fechaStr);
            fprintf(archivo, "  CO2: %.2f ppm\n", ultimo.niveles.co2);
            fprintf(archivo, "  SO2: %.3f ppm\n", ultimo.niveles.so2);
            fprintf(archivo, "  NO2: %.3f ppm\n", ultimo.niveles.no2);
            fprintf(archivo, "  PM2.5: %.2f ug/m3\n", ultimo.niveles.pm25);
            fprintf(archivo, "  Temperatura: %.1f grados C\n", ultimo.clima.temperatura);
            fprintf(archivo, "  Humedad: %.1f%%\n", ultimo.clima.humedad);
            fprintf(archivo, "  Viento: %.1f km/h\n", ultimo.clima.velocidad_viento);
        }
        
        fprintf(archivo, "\n");
    }
    
    fclose(archivo);
    printf("Informe de zonas guardado en %s\n", nombreArchivo);
}

void exportarAlertasTexto() {
    char nombreArchivo[50];
    time_t ahora = time(NULL);
    struct tm *tm_info = localtime(&ahora);
    strftime(nombreArchivo, 50, "alertas_%Y%m%d.txt", tm_info);
    
    FILE *archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("Error al crear el archivo de alertas.\n");
        return;
    }
    
    fprintf(archivo, "=== INFORME DE ALERTAS ACTIVAS ===\n");
    fprintf(archivo, "Fecha de generacion: ");
    char fechaGen[50];
    strftime(fechaGen, 50, "%d/%m/%Y %H:%M:%S", tm_info);
    fprintf(archivo, "%s\n\n", fechaGen);
    
    fprintf(archivo, "Total de alertas: %d\n\n", num_alertas);
    
    if (num_alertas == 0) {
        fprintf(archivo, "No hay alertas registradas.\n");
    } else {
        for (int i = 0; i < num_alertas - 1; i++) {
            for (int j = i + 1; j < num_alertas; j++) {
                if (alertas[i].prioridad < alertas[j].prioridad) {
                    Alerta temp = alertas[i];
                    alertas[i] = alertas[j];
                    alertas[j] = temp;
                }
            }
        }
        
        for (int i = 0; i < num_alertas; i++) {
            char fechaStr[20];
            formatearFecha(alertas[i].fecha, fechaStr);
            
            fprintf(archivo, "Alerta #%d\n", i + 1);
            fprintf(archivo, "Fecha: %s\n", fechaStr);
            
            switch (alertas[i].prioridad) {
                case 1:
                    fprintf(archivo, "Prioridad: BAJA\n");
                    break;
                case 2:
                    fprintf(archivo, "Prioridad: MEDIA\n");
                    break;
                case 3:
                    fprintf(archivo, "Prioridad: ALTA\n");
                    break;
                default:
                    fprintf(archivo, "Prioridad: DESCONOCIDA\n");
            }
            
            fprintf(archivo, "Mensaje: %s\n", alertas[i].mensaje);
            fprintf(archivo, "\n");
        }
    }
    
    fclose(archivo);
    printf("Informe de alertas guardado en %s\n", nombreArchivo);
}

void exportarRecomendacionesTexto() {
    char nombreArchivo[50];
    time_t ahora = time(NULL);
    struct tm *tm_info = localtime(&ahora);
    strftime(nombreArchivo, 50, "recomendaciones_%Y%m%d.txt", tm_info);
    
    FILE *archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("Error al crear el archivo de recomendaciones.\n");
        return;
    }
    
    fprintf(archivo, "=== INFORME DE RECOMENDACIONES ===\n");
    fprintf(archivo, "Fecha de generacion: ");
    char fechaGen[50];
    strftime(fechaGen, 50, "%d/%m/%Y %H:%M:%S", tm_info);
    fprintf(archivo, "%s\n\n", fechaGen);
    
    fprintf(archivo, "Total de recomendaciones: %d\n\n", num_recomendaciones);
    
    if (num_recomendaciones == 0) {
        fprintf(archivo, "No hay recomendaciones disponibles.\n");
        fprintf(archivo, "Genere recomendaciones usando el menu correspondiente.\n");
    } else {
        for (int i = 0; i < num_recomendaciones - 1; i++) {
            for (int j = i + 1; j < num_recomendaciones; j++) {
                if (recomendaciones[i].tipo < recomendaciones[j].tipo) {
                    Recomendacion temp = recomendaciones[i];
                    recomendaciones[i] = recomendaciones[j];
                    recomendaciones[j] = temp;
                }
            }
        }
        
        for (int i = 0; i < num_recomendaciones; i++) {
            char fechaStr[20];
            formatearFecha(recomendaciones[i].fecha, fechaStr);
            
            fprintf(archivo, "Recomendacion #%d\n", i + 1);
            fprintf(archivo, "Fecha: %s\n", fechaStr);
            fprintf(archivo, "Zona: %s\n", recomendaciones[i].zona);
            
            switch (recomendaciones[i].tipo) {
                case 1:
                    fprintf(archivo, "Tipo: PREVENTIVA\n");
                    break;
                case 2:
                    fprintf(archivo, "Tipo: CORRECTIVA\n");
                    break;
                case 3:
                    fprintf(archivo, "Tipo: URGENTE\n");
                    break;
                default:
                    fprintf(archivo, "Tipo: GENERAL\n");
            }
            
            fprintf(archivo, "Descripcion: %s\n", recomendaciones[i].recomendacion);
            fprintf(archivo, "\n");
        }
    }
    
    fclose(archivo);
    printf("Informe de recomendaciones guardado en %s\n", nombreArchivo);
}

void compararDiasConOMS() {
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    listarZonas();
    int id = leerEntero("Ingrese el ID de la zona para comparar con OMS: ");
    
    for (int i = 0; i < num_zonas; i++) {
        if (zonas[i].id == id) {
            if (zonas[i].num_registros == 0) {
                printf("La zona no tiene registros para comparar.\n");
                return;
            }
            
            printf("\n=== COMPARACION CON ESTANDARES OMS 2021 ===\n");
            printf("Zona: %s\n", zonas[i].nombre);
            printf("Total de registros: %d\n\n", zonas[i].num_registros);
            
            int excesos_contaminantes = 0;
            int excesos_climaticos = 0;
            
            for (int j = 0; j < zonas[i].num_registros; j++) {
                char fechaStr[11];
                formatearFecha(zonas[i].registros[j].fecha, fechaStr);
                
                printf("=== DIA %d (%s) ===\n", j + 1, fechaStr);
                printf("CONTAMINANTES:\n");
                printf("  CO2: %.2f ppm ", zonas[i].registros[j].niveles.co2);
                if (zonas[i].registros[j].niveles.co2 > LIMITE_CO2) {
                    printf("(EXCEDE limite OMS: %.1f ppm)\n", LIMITE_CO2);
                    excesos_contaminantes++;
                } else {
                    printf("(OK - Dentro del limite OMS)\n");
                }
                
                printf("  SO2: %.3f ppm ", zonas[i].registros[j].niveles.so2);
                if (zonas[i].registros[j].niveles.so2 > LIMITE_SO2) {
                    printf("(EXCEDE limite OMS: %.3f ppm)\n", LIMITE_SO2);
                    excesos_contaminantes++;
                } else {
                    printf("(OK - Dentro del limite OMS)\n");
                }
                
                printf("  NO2: %.3f ppm ", zonas[i].registros[j].niveles.no2);
                if (zonas[i].registros[j].niveles.no2 > LIMITE_NO2) {
                    printf("(EXCEDE limite OMS: %.3f ppm)\n", LIMITE_NO2);
                    excesos_contaminantes++;
                } else {
                    printf("(OK - Dentro del limite OMS)\n");
                }
                
                printf("  PM2.5: %.2f ug/m3 ", zonas[i].registros[j].niveles.pm25);
                if (zonas[i].registros[j].niveles.pm25 > LIMITE_PM25) {
                    printf("(EXCEDE limite OMS: %.1f ug/m3)\n", LIMITE_PM25);
                    excesos_contaminantes++;
                } else {
                    printf("(OK - Dentro del limite OMS)\n");
                }
                printf("\nCONDICIONES CLIMATICAS:\n");
                printf("  Temperatura: %.1f grados C ", zonas[i].registros[j].clima.temperatura);
                if (zonas[i].registros[j].clima.temperatura < TEMP_MIN_CONFORT) {
                    printf("(BAJO limite minimo OMS: %.1f°C)\n", TEMP_MIN_CONFORT);
                    excesos_climaticos++;
                } else if (zonas[i].registros[j].clima.temperatura > TEMP_MAX_CONFORT) {
                    printf("(EXCEDE limite maximo OMS: %.1f°C)\n", TEMP_MAX_CONFORT);
                    excesos_climaticos++;
                } else {
                    printf("(OK - Rango de confort OMS: %.1f-%.1f°C)\n", TEMP_MIN_CONFORT, TEMP_MAX_CONFORT);
                }
                
                printf("  Humedad: %.1f%% ", zonas[i].registros[j].clima.humedad);
                float diferencia_humedad = fabs(zonas[i].registros[j].clima.humedad - HUMEDAD_OPTIMA);
                if (diferencia_humedad > 20.0) {
                    printf("(ALEJADO del optimo OMS: %.1f%%)\n", HUMEDAD_OPTIMA);
                    excesos_climaticos++;
                } else if (diferencia_humedad > 10.0) {
                    printf("(MODERADAMENTE alejado del optimo OMS: %.1f%%)\n", HUMEDAD_OPTIMA);
                } else {
                    printf("(OK - Cerca del optimo OMS: %.1f%%)\n", HUMEDAD_OPTIMA);
                }
                
                printf("  Velocidad viento: %.1f km/h ", zonas[i].registros[j].clima.velocidad_viento);
                if (zonas[i].registros[j].clima.velocidad_viento > VIENTO_MAX_CONFORT) {
                    printf("(EXCEDE limite confort OMS: %.2f km/h)\n", VIENTO_MAX_CONFORT);
                    excesos_climaticos++;
                } else {
                    printf("(OK - Dentro del limite de confort OMS)\n");
                }
                
                printf("\n");
            }

            printf("=== RESUMEN DE EVALUACION OMS ===\n");
            printf("Total de excesos en contaminantes: %d\n", excesos_contaminantes);
            printf("Total de problemas climaticos: %d\n", excesos_climaticos);
            
            if (excesos_contaminantes == 0 && excesos_climaticos == 0) {
                printf("EVALUACION: EXCELENTE - Todos los parametros cumplen con estandares OMS\n");
            } else if (excesos_contaminantes <= 2 && excesos_climaticos <= 2) {
                printf("EVALUACION: BUENA - Pocos excesos, condiciones generalmente aceptables\n");
            } else if (excesos_contaminantes <= 5 && excesos_climaticos <= 5) {
                printf("EVALUACION: REGULAR - Varios excesos, requiere atencion\n");
            } else {
                printf("EVALUACION: MALA - Muchos excesos, requiere accion inmediata\n");
            }
            
            return;
        }
    }
    printf("No se encontro una zona con ID %d\n", id);
}

void agregarZonasAutomaticas() {
    if (strlen(nombre_ciudad) == 0) {
        printf("No se ha ingresado el nombre de la ciudad.\n");
        return;
    }

    char zonasQuito[10][MAX_NOMBRE] = {
        "Centro Historico", "Cumbaya", "Inaquito", "La Floresta", "La Carolina",
        "La Mariscal", "Tumbaco", "Los Chillos", "El Panecillo", "Mitad del Mundo"
    };
    
    char zonasGuayaquil[10][MAX_NOMBRE] = {
        "Centro", "Malecon 2000", "Las Penas", "Puerto Santa Ana", "Urdesa",
        "Samborondon", "Kennedy", "Alborada", "Parque Historico", "Cerro Blanco"
    };
    
    char zonasCuenca[10][MAX_NOMBRE] = {
        "Centro Historico", "El Barranco", "Turi", "Yanuncay", "Puente Roto",
        "Barrio de San Sebastian", "Barrio del Vado", "Parque Nacional Cajas", "Miraflores", "Capulispamba"
    };
    
    char zonasSantoDomingo[10][MAX_NOMBRE] = {
        "Centro", "Bomboli", "La Concordia", "El Colorado", "Abraham Calazacon",
        "Zaracay", "Rio Verde", "Las Palmeras", "Los Rosales", "Plan de Vivienda"
    };
    
    char zonasMachala[10][MAX_NOMBRE] = {
        "Centro", "Puerto Bolivar", "La Aurora", "El Cambio", "Barrio Cuba",
        "Jambeli (Archipielago)", "San Gregorio", "Brisas del Mar", "La Providencia", "Miraflores"
    };
    
    char zonasManta[10][MAX_NOMBRE] = {
        "Centro", "Tarqui", "Malecon", "Barbasquillo", "Murcielago",
        "San Mateo", "Santa Marianita", "San Lorenzo", "El Palmar", "Las Cumbres"
    };
    
    char zonasPortoviejo[10][MAX_NOMBRE] = {
        "Centro", "El Floron", "Picoaza", "Colon", "Andres de Vera",
        "San Pablo", "Ciudadela Universitaria", "Los Cerezos", "La California", "Crucita (Playa)"
    };
    
    char zonasLoja[10][MAX_NOMBRE] = {
        "Centro Historico", "Villonaco", "Pucara", "Jipiro", "Epoca",
        "Daniel Alvarez", "La Argelia", "San Sebastian", "El Valle", "La Tebaida"
    };
    
    char zonasAmbato[10][MAX_NOMBRE] = {
        "Centro", "Ficoa", "Atocha", "La Peninsula", "Huachi Chico",
        "Huachi Grande", "Ingahurco", "Miraflores", "Izamba", "Juan Benigno Vela"
    };
    
    char zonasBanos[10][MAX_NOMBRE] = {
        "Centro", "El Salado", "Runtun", "Las Cascadas (Ruta de las Cascadas)", "Agoyan",
        "Rio Blanco", "Ulba", "Puyo (Ruta hacia)", "Caserio El Tungurahua", "El Pisque"
    };

    char (*zonasSeleccionadas)[MAX_NOMBRE] = NULL;
    char ciudadLower[MAX_NOMBRE];
    strcpy(ciudadLower, nombre_ciudad);

    for (int i = 0; ciudadLower[i]; i++) {
        ciudadLower[i] = tolower(ciudadLower[i]);
    }
    
    if (strcmp(ciudadLower, "quito") == 0) {
        zonasSeleccionadas = zonasQuito;
    } else if (strcmp(ciudadLower, "guayaquil") == 0) {
        zonasSeleccionadas = zonasGuayaquil;
    } else if (strcmp(ciudadLower, "cuenca") == 0) {
        zonasSeleccionadas = zonasCuenca;
    } else if (strcmp(ciudadLower, "santo domingo") == 0) {
        zonasSeleccionadas = zonasSantoDomingo;
    } else if (strcmp(ciudadLower, "machala") == 0) {
        zonasSeleccionadas = zonasMachala;
    } else if (strcmp(ciudadLower, "manta") == 0) {
        zonasSeleccionadas = zonasManta;
    } else if (strcmp(ciudadLower, "portoviejo") == 0) {
        zonasSeleccionadas = zonasPortoviejo;
    } else if (strcmp(ciudadLower, "loja") == 0) {
        zonasSeleccionadas = zonasLoja;
    } else if (strcmp(ciudadLower, "ambato") == 0) {
        zonasSeleccionadas = zonasAmbato;
    } else if (strcmp(ciudadLower, "banos") == 0) {
        zonasSeleccionadas = zonasBanos;
    } else {
        printf("No hay zonas predefinidas para la ciudad '%s'.\n", nombre_ciudad);
        printf("Use la opcion 1 para agregar zonas manualmente.\n");
        return;
    }
    int zonasYaAgregadas = 0;
    for (int i = 0; i < num_zonas; i++) {
        for (int j = 0; j < 10; j++) {
            if (strcmp(zonas[i].nombre, zonasSeleccionadas[j]) == 0) {
                zonasYaAgregadas++;
                break;
            }
        }
    }
    
    int zonasDisponiblesCiudad = 10 - zonasYaAgregadas;
    if (zonasDisponiblesCiudad <= 0) {
        printf("Ya se han agregado todas las zonas disponibles para %s.\n", nombre_ciudad);
        return;
    }
    
    int cantidad;
    char mensaje[100];
    do {
        if (zonasYaAgregadas == 0) {
            sprintf(mensaje, "Cuantas zonas desea agregar automaticamente (maximo 10)? ");
        } else {
            sprintf(mensaje, "Cuantas zonas desea agregar automaticamente (maximo %d)? ", zonasDisponiblesCiudad);
        }
        cantidad = leerEntero(mensaje);
        if (cantidad <= 0 || cantidad > zonasDisponiblesCiudad) {
            printf("Debe ingresar un numero entre 1 y %d.\n", zonasDisponiblesCiudad);
        }
    } while (cantidad <= 0 || cantidad > zonasDisponiblesCiudad);
    
    printf("Son un total de 10 zonas.\n");
    printf("Se le asignara %d zona(s) de manera automatica dependiendo de la ciudad agregada.\n", cantidad);
    printf("Zonas agregadas:\n");
    
    int agregadas = 0;
    for (int i = 0; i < 10 && agregadas < cantidad; i++) {
        // Verificar si esta zona ya existe
        int yaExiste = 0;
        for (int j = 0; j < num_zonas; j++) {
            if (strcmp(zonas[j].nombre, zonasSeleccionadas[i]) == 0) {
                yaExiste = 1;
                break;
            }
        }
        
        if (!yaExiste) {
            Zona nueva;
            nueva.id = encontrarSiguienteID();
            
            if (nueva.id == -1) {
                printf("Error: No hay IDs disponibles.\n");
                break;
            }
            
            nueva.num_registros = 0;
            strcpy(nueva.nombre, zonasSeleccionadas[i]);
            
            zonas[num_zonas++] = nueva;
            printf("%d %s\n", nueva.id, nueva.nombre);
            agregadas++;
        }
    }
    
    printf("Zonas agregadas exitosamente.\n");
    guardarDatos();
}