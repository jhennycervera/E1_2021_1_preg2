/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FuncionesEx01_20211_Preg02.h
 * Author: jhenn
 *
 * Created on 18 de octubre de 2021, 03:48 PM
 */

#ifndef FUNCIONESEX01_20211_PREG02_H
#define FUNCIONESEX01_20211_PREG02_H

#include <iostream>        
#include <fstream>
using namespace std;

void imprimirLinea(ofstream &, char, int);
ifstream abrirArchL(const char *nombre);
ofstream abrirArchE(const char *nombre);

void cargadatos(void* &conductor, void*&falta);
void cargarConductores(void* &conductorGen);
void * leerRegConductor(ifstream &arch);
void cargarFaltas(void* &faltaGen);
void* leerRegFaltas(ifstream &arch);


void  registrafaltas(void *&consolidadoGen, void *conductor, void *falta);

int validarConductorEnConsolidado(int rcodC, int rmm, int raa, void**buffCons, int numCons);

void  actualizarMonto(void* conductorGen, int rInfr, void* falta);

double obtenerMontoFalta(int rInfr, void* faltaGen);

char* obtenerNombreConductor(int rcodC, void *conductorGen);
void * agregarRegistroConsolidado(int rcodC, void* conductorGen, int raa,int rmm,int rInfr,void*falta);



void imprime(void* consolidadoGen);

void ordenarConsolidado(void**consolidado);
void quickSortLicenciaFecha(void **consolidado, int ini, int fin);

void intercambiar(void **consolidado, int i, int j);

int compararLicFecha(void*consIGen, void* consJGen);
void imprimirReporte(void **consolidado);
void imprimirUnConsolidado(ofstream &arch, void* consoliGen) ;
#endif /* FUNCIONESEX01_20211_PREG02_H */
