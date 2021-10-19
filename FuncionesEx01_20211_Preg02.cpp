/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



/* 
 * File:   FuncionesEx01_20211_Preg02.cpp
 * Author: jhenn
 * Codigo: 20182841
 * Created on 18 de octubre de 2021, 03:48 PM
 */

#include "FuncionesEx01_20211_Preg02.h"
#include "Imprime_Examen01_2021_1.h"
#include <iostream>        
#include <iomanip>
#include <fstream>
#include <cstring>        
using namespace std;

enum E_Conducor {
    COD_C, NOMBRE
};

enum E_FALTA {
    COD, DESC, MULTA
};

enum E_CONS {
    C0D_CONS, NOMBRE_CONS, AA, MM, MONTO
};

void imprimirLinea(ofstream &Rep, char c, int cant) {
    Rep.fill(c);
    Rep << setw(cant) << c << endl;
    Rep.fill(' ');
}

ifstream abrirArchL(const char *nombre) {
    ifstream arch;
    arch.open(nombre, ios::in);
    if (!arch) {
        cout << "ERROR: no se pudo abrir el archivo " << nombre << endl;
        exit(1);
    }
    return arch;
}

ofstream abrirArchE(const char *nombre) {
    ofstream arch;
    arch.open(nombre, ios::out);
    if (!arch) {
        cout << "ERROR: no se pudo abrir el archivo " << nombre << endl;
        exit(1);
    }
    return arch;
}

void cargadatos(void* &conductor, void*&falta) {

    cargarConductores(conductor);
    cargarFaltas(falta);

    imprimeconductor(conductor);
    imprimefalta(falta);

}

void cargarConductores(void* &conductorGen) {

    ifstream arch("Conductores.csv", ios::in);

    void ** conductor, *buffCond[300], *regCond;
    int numCond = 0;

    while (1) {
        regCond = leerRegConductor(arch);
        if (regCond == nullptr) break;
        buffCond[numCond] = regCond;
        numCond++;
    }

    conductor = new void*[numCond + 1];
    for (int i = 0; i < numCond; i++) {
        conductor[i] = buffCond[i];
    }
    conductor[numCond] = nullptr; //terminador
    conductorGen = conductor;
}

void * leerRegConductor(ifstream &arch) {

    void** regCond;
    int rcod;
    char rnombr[200];

    arch>>rcod;
    if (arch.eof()) return nullptr;
    arch.get(); //coma
    arch.getline(rnombr, 200);

    regCond = new void*[2];

    char *ptrNombre = new char[strlen(rnombr) + 1];
    strcpy(ptrNombre, rnombr);
    regCond[COD_C] = new int(rcod);
    regCond[NOMBRE] = ptrNombre;

    return regCond;
}

void cargarFaltas(void* &faltaGen) {
    ifstream arch("Infracciones.csv", ios::in);

    void ** falta, *buffFalt[300], *regFalt;
    int numFalt = 0;

    while (1) {
        regFalt = leerRegFaltas(arch);
        if (regFalt == nullptr) break;
        buffFalt[numFalt] = regFalt;
        numFalt++;
    }

    falta = new void*[numFalt + 1];
    for (int i = 0; i < numFalt; i++) {
        falta[i] = buffFalt[i];
    }
    falta[numFalt] = nullptr; //terminador
    faltaGen = falta;

}

void* leerRegFaltas(ifstream &arch) {

    void** regFalta;

    int rcod;
    char rdes[500], raux[20];
    double rmonto;

    arch>>rcod;
    if (arch.eof()) return nullptr;
    arch.get();
    arch.getline(rdes, 500, ',');
    arch.getline(raux, 20, ',');
    arch>>rmonto;

    char *ptrDescr = new char[strlen(rdes) + 1];
    strcpy(ptrDescr, rdes);
    regFalta = new void*[3];
    regFalta[COD] = new int(rcod);
    regFalta[DESC] = ptrDescr;
    regFalta[MULTA] = new double(rmonto);

    return regFalta;
}

void registrafaltas(void *&consolidadoGen, void *conductor, void *falta) {


    ifstream arch("RegistroDeFaltas.csv", ios::in);
    void** consolidado, *buffCons[1000], *regCons;

    int rcodC, rInfr, rdd, rmm, raa, numCons = 0;
    char c, rplaca[10];

    while (1) {
        arch>>rcodC;
        if (arch.eof())break;
        arch.get(); //coma
        arch.getline(rplaca, 10, ',');
        arch >> rdd >> c >> rmm >> c >> raa >> c>>rInfr;

        //condiciones para agregar al consolidado  --> busco en el buffer donde estoy llenado
        int pos = validarConductorEnConsolidado(rcodC, rmm, raa, buffCons, numCons); //si ya se encuentra el conductor y es la mismo mes y año
        if (pos != -1) { //se encuentra 
            actualizarMonto(buffCons[pos], rInfr, falta);
        } else {
            buffCons[numCons] = agregarRegistroConsolidado(rcodC, conductor, raa, rmm, rInfr, falta);
            numCons++;
        }
    }

    consolidado = new void*[numCons];
    for (int i = 0; i < numCons; i++) {
        consolidado[i] = buffCons[i];
    }
    consolidado[numCons] = nullptr;
    consolidadoGen = consolidado;
}

int validarConductorEnConsolidado(int rcodC, int rmm, int raa, void**buffCons, int numCons) {

    for (int i = 0; i < numCons; i++) {
        void** regCons = (void**) buffCons[i];

        int cod = *(int*) regCons[0];
        int aa = *(int*) regCons[2];
        int mm = *(int*) regCons[3];

        if (cod == rcodC && aa == raa && mm == rmm) return i;
    }

    return -1;

}

void actualizarMonto(void* conductorGen, int rInfr, void* falta) {

    void** regCons = (void**) conductorGen;

    double *ptrMonto = (double*) regCons[4];

    double aux = obtenerMontoFalta(rInfr, falta);
    if (aux != -1) (*ptrMonto) += aux;
}

double obtenerMontoFalta(int rInfr, void* faltaGen) {

    void** falta = (void**) faltaGen;

    for (int i = 0; falta[i] != nullptr; i++) {
        void** regFalt = (void**) falta[i];

        int cod = *(int*) regFalt[COD];
        double monto = *(double*) regFalt[MULTA];
        if (cod == rInfr) return monto;
    }
    return -1;
}

char* obtenerNombreConductor(int rcodC, void *conductorGen) {

    void** conductor = (void**) conductorGen;

    for (int i = 0; conductor[i] != nullptr; i++) {
        void** regCond = (void**) conductor[i];

        int cod = *(int *) regCond[COD_C];
        char* nombr = (char*) regCond[NOMBRE];
        if (cod == rcodC) return nombr;
    }
    return nullptr;
}

void * agregarRegistroConsolidado(int rcodC, void* conductorGen, int raa, int rmm, int rInfr, void*falta) {
    void** regCons;

    regCons = new void*[5];
    char *ptrNombre = obtenerNombreConductor(rcodC, conductorGen);
    double monto = obtenerMontoFalta(rInfr, falta);
    regCons[C0D_CONS] = new int(rcodC);
    regCons[NOMBRE_CONS] = ptrNombre; //Puede ser nullnullptr si no encuentra el nombre
    regCons[AA] = new int(raa);
    regCons[MM] = new int(rmm);
    regCons[MONTO] = new double(monto);

    return regCons;
}

void imprime(void* consolidadoGen) {
    void** consolidado = (void**) consolidadoGen;

    ordenarConsolidado(consolidado);

    imprimirReporte(consolidado);
}

void ordenarConsolidado(void**consolidado) {

    int i;
    for (i = 0; consolidado[i] != nullptr; i++);
    quickSortLicenciaFecha(consolidado, 0, i - 1);

}

void quickSortLicenciaFecha(void **consolidado, int ini, int fin) {

    if (ini >= fin) return;

    intercambiar(consolidado, ini, (ini + fin) / 2);

    int pivote = ini;
    for (int i = ini + 1; i <= fin; i++) {
        if (compararLicFecha(consolidado[i], consolidado[ini]) < 0) { //menor
            pivote++;
            intercambiar(consolidado, i, pivote);
        }
    }
    intercambiar(consolidado, ini, pivote);
    quickSortLicenciaFecha(consolidado, ini, pivote - 1);
    quickSortLicenciaFecha(consolidado, pivote + 1, fin);

}

void intercambiar(void **consolidado, int i, int j) {
    void* aux;

    aux = consolidado[i];
    consolidado[i] = consolidado[j];
    consolidado[j] = aux;

}

int compararLicFecha(void*consIGen, void* consJGen) {

    void**consI = (void**) consIGen;
    void**consJ = (void**) consJGen;


    int codI = *(int*) consI[C0D_CONS];
    int codJ = *(int*) consJ[C0D_CONS];
    int aaI = *(int *) consI[AA];
    int aaJ = *(int *) consJ[AA];
    int mmI = *(int *) consI[MM];
    int mmJ = *(int *) consJ[MM];

    int fechaI = mmI * 100 + aaI * 10000;
    int fechaJ = mmJ * 100 + aaJ * 10000;


    if (codI == codJ) { //ordenar por fecha
        return fechaI - fechaJ;
    } else return codI - codJ;
}

void imprimirReporte(void **consolidado) {

    ofstream arch("Reportepreg2.txt", ios:: out);
    int codAnt=0;
    
    for (int i = 0; consolidado[i] != nullptr; i++) {

        void** regCons = (void**) consolidado[i];
        int cod = *(int*) regCons[C0D_CONS];
        char* nombre = (char*) regCons[NOMBRE_CONS];
        
        if (codAnt != cod || (codAnt==0) ) {
            arch << setprecision(2) << fixed;
            arch << left << setw(10) << cod << nombre << endl;
        }
        if(i==496){
            cout<<i<<endl;
        }
        imprimirUnConsolidado(arch, consolidado[i]);
        codAnt = cod;

    }
}
void imprimirUnConsolidado(ofstream &arch, void* consoliGen) {
    void** regCons = (void**) consoliGen;

    int cod = *(int*) regCons[C0D_CONS];
    char* nombre = (char*) regCons[NOMBRE_CONS];
    int aa = *(int*) regCons[AA];
    int mm = *(int*) regCons[MM];
    double monto = *(double*) regCons[MONTO];

    arch << setw(10) << aa << setw(10) << mm << monto << endl;

}

