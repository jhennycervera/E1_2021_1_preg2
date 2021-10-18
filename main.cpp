/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Jhenny Cervera Vargas
 * Codigo: 20182841
 * Created on 18 de octubre de 2021, 02:14 PM
 */


#include "FuncionesEx01_20211_Preg02.h"

int main(int argc, char** argv) {
    void*conductor, *falta, *consolidado;
    
    cargadatos(conductor, falta);
    registrafaltas(consolidado, conductor, falta);
    imprime(consolidado);
    
    return 0;
}

