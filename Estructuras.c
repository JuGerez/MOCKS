#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "Estructuras.h"

//    int id;
//    int idCliente;
//    int tipoCuenta; 1. Caja de Ahorro en Pesos, 2. Caja de Ahorro en Dólares, 3. Cta Cte en $.
//    int nroCuenta;
//    float costoMensual; empleado sin costo, jubilado sin costo, dolares extra.
//    float saldo;
//    int eliminado;

stCuenta cargaUnaCuenta (char nombreArchivo[])
{
    stCuenta Cuenta;
    printf("Conoce el Id de Cliente o lo quiere buscar por n%c de Dni? \n", 167);
    printf("1. Lo conozco. \n");
    printf("2. Lo tengo que buscar. \n");
    scanf("%d", &Cuenta.idCliente);

    while(Cuenta.idCliente > 2 || Cuenta.idCliente < 1)
    {
        printf("Seleccione una opcion valida: \n");
        printf("1. Conozco el Id del Cliente. \n");
        printf("2. Lo tengo que buscar. \n");
        scanf("%d", &Cuenta.idCliente);
    }

    while(Cuenta.idCliente == 2){
        stCliente Cliente;
        char dni;
        printf("Ingrese el Dni del Cliente que quiere buscar: \n");
        fflush(stdin);
        gets(&dni);
        while( muestraUnClienteXDni(nombreArchivo, dni, Cliente) < 0){
            printf("Ingrese el Dni del Cliente que quiere buscar: \n");
            fflush(stdin);
            gets(&dni);
        }
    }

    while (Cuenta.idCliente >= cuentaRegistroGral(nombreArchivo, sizeof(stCliente)))
    {
        printf("Id de Cliente no existente.\n");
        printf("Ingrese un Id de Cliente: \n");
        scanf("%d", &Cuenta.idCliente);
    }

     printf("Seleccione el tipo de cuenta que quiere abrir: \n");
    printf("1. Caja de ahorro en pesos. \n");
    printf("2. Caja de ahorro en dolares. \n");
    printf("3. Cuenta corriente en pesos. \n");
    scanf("%d", &Cuenta.tipoCuenta);

    while(Cuenta.tipoCuenta > 3 || Cuenta.tipoCuenta < 1)
    {
        printf("Seleccione un tipo de cuenta valido: \n");
        printf("1. Caja de ahorro en pesos. \n");
        printf("2. Caja de ahorro en dolares. \n");
        printf("3. Cuenta corriente en pesos. \n");
        scanf("%d", &Cuenta.tipoCuenta);
    }

    int nroCuenta;
    printf("Cree un n%c de Cuenta: \n", 167);
    scanf("%d", &nroCuenta);

    while (buscaIdCuenta(nombreArchivo, nroCuenta) == 1)//1 existe y 0 no.
    {
        printf("N%c de Cuenta existente.\n", 167);
        printf("Por favor cree otro n%c de Cuenta: \n", 167);
        scanf("%d", &nroCuenta);
    }

    Cuenta.nroCuenta = nroCuenta;

    if(Cuenta.tipoCuenta == 1)
    {
        Cuenta.costoMensual = 10;
    }

    if(Cuenta.tipoCuenta == 2)
    {
        Cuenta.costoMensual = 20;
    }

    if(Cuenta.tipoCuenta == 3)
    {
        Cuenta.costoMensual = 30;
    }

    Cuenta.saldo = 0;

    printf("Ingrese estado de la cuenta: \n");
    printf("0. si esta activo.\n ");
    printf("1. si no esta activo.\n ");
    scanf("%d", &Cuenta.eliminado);

    while(Cuenta.eliminado != 1 || Cuenta.eliminado !=0)
    {
        printf("Ingrese un numero valido: \n");
        printf("0. si esta activo.\n ");
        printf("1. si no esta activo.\n ");
        scanf("%d", &Cuenta.eliminado);
    }

    return Cuenta;
}

void muestraUnaCuenta (stCuenta Cuenta)
{
        printf("Los datos de la cuenta cargada son: \n");
        printf("El Id: %d", Cuenta.id);
        printf("El Id del Cliente: %d", Cuenta.idCliente);
        printf("El n%c de Cuenta: %d", Cuenta.nroCuenta);
        printf("El tipo de Cuenta: %d", Cuenta.tipoCuenta);
        printf("El costo mensual de la Cuenta es: %d", Cuenta.costoMensual);
        printf("El saldo actual de la Cuenta es: %d", Cuenta.saldo);
        if(Cuenta.eliminado == 1)
        {
            printf("La cuenta no esta activa.");
        } else {
            printf("La cuenta esta activa.");
        }
}
void muestraArchivoCuenta(char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo,"rb");

    if(archivo)
    {
        stCuenta aux;

        printf("\nContenido del archivo:\n\n");

        while( (fread(&aux, sizeof(stCuenta),1,archivo) ) >0)
        {
            muestraUnaCuenta(aux);
        }
        fclose(archivo);
    }
}

void cargaCuentaArch (char nombreArchivo[], stCuenta Cuenta)
{
    FILE* archivo = fopen(nombreArchivo, "ab");

    if(archivo)
    {
        Cuenta.id = cuentaRegistroGral(nombreArchivo, sizeof(stCuenta));
        fwrite(&Cuenta, sizeof(stCuenta), 1, archivo);

        fclose(archivo);
    }
}

int cuentaRegistroGral(char nombreArchivo[], int tamanio)
{
    int cantidad=0;
    FILE* archivo = fopen(nombreArchivo,"rb");
    if(archivo)
    {
    fseek(archivo,0,2);
    cantidad = ftell(archivo)/tamanio;
    fclose(archivo);
    }
    return cantidad;
}

int buscaIdCuenta(char nombreArchivo[], int nroCuenta)
{
    int flag = 0; // 0 NO EXISTE; 1 EXISTE.
    stCuenta cuenta;
    FILE *archivo = fopen(nombreArchivo, "rb");

    if (archivo)
    {
        return flag;
    }

    while (fread(&cuenta, sizeof(stCuenta), 1, archivo) > 0)
    {
        if (cuenta.nroCuenta == nroCuenta)
        {
            flag = 1;
            break;
        }
    }

    fclose(archivo);
    return flag;
}
int muestraUnClienteXDni (char nombreArchivo[], char dni, stCliente cliente)
{
    int id = -1; //Por si el dni a buscar no esta en cliente.
    FILE* archivo = fopen(nombreArchivo, "rb");

    if(archivo)
    {
        while (fread(&cliente, sizeof(stCliente), 1, archivo) > 0)
        {
            if (strcmp(dni,cliente.dni) == 0)
            {
                muestraUnCliente(cliente);
                id = cliente.id;
            }
        }
        fclose(archivo);
    }

    return id;
}
void muestraUnCliente(stCliente cliente)
{

}

void cambiarEstadoCuenta(char nombreArchivo[], int idCuenta, int estado)
{

    stCuenta Cuenta;
    FILE* archivo = fopen(nombreArchivo, "r+b");

    if(archivo)
    {
        while (fread(&Cuenta, sizeof(stCuenta), 1, archivo) > 0)
        {
            if (Cuenta.id == idCuenta)
            {
                Cuenta.eliminado = estado;
                fseek(archivo, -1*sizeof(stCuenta), 1);
                fwrite(&Cuenta, sizeof(stCuenta), 1, archivo);
            }
        }

        fclose(archivo);
    }
}

void cambiarTipoCuenta(char nombreArchivo[], int idCuenta, int tipoCuenta)
{
    stCuenta cuenta;
    FILE* archivo = fopen(nombreArchivo, "r+b");

    if(archivo)
    {
        while (fread(&cuenta, sizeof(stCuenta), 1, archivo) > 0)
        {
            if (cuenta.id == idCuenta)
            {
                cuenta.tipoCuenta = tipoCuenta;
                fseek(archivo, -1*sizeof(stCuenta), 1);
                fwrite(&cuenta, sizeof(stCuenta), 1, archivo);
            }
        }
        fclose(archivo);
    }
}
int tipoCuenta()
{
    int tipoCta;

    printf("Seleccione el tipo de cuenta por el que quiere cambiar: \n");
    printf("1. Caja de ahorro en pesos. \n");
    printf("2. Caja de ahorro en dolares. \n");
    printf("3. Cuenta corriente en pesos. \n");
    scanf("%d", &tipoCta);

    while(tipoCta > 3 || tipoCta < 1)
    {
        printf("Seleccione un tipo de cuenta valido: \n");
        printf("1. Caja de ahorro en pesos. \n");
        printf("2. Caja de ahorro en dolares. \n");
        printf("3. Cuenta corriente en pesos. \n");
        scanf("%d", &tipoCta);
    }

    return tipoCta;
}

void modificarCuenta(char nombreArchivo[], int idCuenta, stCuenta nuevacuenta)
{
    stCuenta cuenta;
    FILE* archivo = fopen(nombreArchivo, "r+b");

    if(archivo){
            while (fread(&cuenta, sizeof(cuenta), 1, archivo) > 0)
            {
                if (cuenta.id == idCuenta)
                {
                    fseek(archivo, -1*sizeof(stCuenta), 1);
                    fwrite(&nuevacuenta, sizeof(stCuenta), 1, archivo);
                    break;
                }
            }
            fclose(archivo);
    }
}

void cambiarEstadoCliente(char nombreArchivo[], int idCliente, int estado)
{
    stCliente cliente;
    FILE* archivo = fopen(nombreArchivo, "r+b");

    if(archivo)
    {
        while (fread(&cliente, sizeof(stCliente), 1, archivo) > 0)
        {
            if (cliente.id == idCliente)
            {
                cliente.id = estado;
                fseek(archivo, -1*sizeof(stCliente), 1);
                fwrite(&cliente, sizeof(stCliente), 1, archivo);
            }
        }
        fclose(archivo);
    }
}

void muestraPorIdCuenta(char nombreArchivo[], int idCuenta)
{
    stCuenta cuenta;
    FILE *archivo = fopen(nombreArchivo, "rb");

    if (archivo)
    {
        while (fread(&cuenta, sizeof(stCuenta), 1, archivo) > 0)
        {
            if (cuenta.id == idCuenta)
            {
                muestraUnaCuenta(cuenta);
            }
        }
        fclose(archivo);
    }
}
