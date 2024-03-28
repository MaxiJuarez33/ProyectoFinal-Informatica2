#include "sofa.h"
#include <stdio.h>

int main()
{
    /* Definir el Tiempo Terrestre (TT) */
    double tt1 = 2453750.5;   /* Parte entera del día juliano */
    double tt2 = 0.892482639; /* Fracción del día juliano */

    /* Variables para almacenar el Tiempo Atómico Internacional (TAI) y el Tiempo Universal Coordinado (UTC) */
    double tai1, tai2, utc1, utc2;

    /* Variable para almacenar el estado de la operación */
    int status;

    /* Convertir TT a TAI */
    status = iauTttai(tt1, tt2, &tai1, &tai2);

    if (status != 0)
    {
        printf("Error al convertir TT a TAI\n");
        return 1;
    }

    /* Convertir TAI a UTC */
    status = iauTaiutc(tai1, tai2, &utc1, &utc2);

    if (status == 0)
    {
        printf("UTC: %.6f %.6f\n", utc1, utc2);
    }
    else
    {
        printf("Error al convertir TAI a UTC\n");
    }

    return 0;
}
