/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: harold
 *
 * Created on July 13, 2018, 8:44 AM
 */

#include <cstdlib>
#include <iostream>
#include "viirs_nc.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    viirs_nc *vhdf = new viirs_nc() ;
    vhdf->set_hdf_file ("/home/harold/vnp02.nc", "/home/harold/vnp03.nc") ;
    cout << "EXITING" << endl ;
    //vhdf->get_info_thermal() ;
    return 0;
}

