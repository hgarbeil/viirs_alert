/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   viirs_hdf.h
 * Author: harold
 *
 * Created on July 13, 2018, 8:48 AM
 */

#ifndef VIIRS_HDF_H
#define VIIRS_HDF_H


#include "H5Cpp.h"
using namespace H5 ;

class viirs_hdf {
public:
    viirs_hdf();
    viirs_hdf(const viirs_hdf& orig);
    virtual ~viirs_hdf();
    void set_hdf_file  (char *) ;
    void get_info () ;
    void read_geom  () ;
    void read_thermal() ;
    H5File *h5f ;
    float *lat, *lon, *solzen, *solaz ;
    float *band10, *band13, *band16 ;
    bool nightFlag ;
    
private:
    int nx, ny ;
};

#endif /* VIIRS_HDF_H */

