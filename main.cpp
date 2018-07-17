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
#include "viirs_hdf.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    viirs_hdf *vhdf = new viirs_hdf() ;
    vhdf->set_hdf_file (
    "/lhome/hg/data/viirs/hawaii/GMODO-SVM10-SVM13-SVM16_npp_d20140711_t1241137_e1246541_b14005_c20180713043924959650_noaa_ops.h5") ;
    cout << "EXITING" << endl ;
    return 0;
}

