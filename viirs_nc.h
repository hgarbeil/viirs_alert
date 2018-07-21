/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   viirs_nc.h
 * Author: harold
 *
 * Created on July 13, 2018, 8:48 AM
 */

#ifndef VIIRS_NC_H
#define VIIRS_NC_H


#include "H5Cpp.h"
using namespace H5 ;

class viirs_nc {
public:
    viirs_nc();
    viirs_nc(const viirs_nc& orig);
    virtual ~viirs_nc();
    void set_hdf_file  (char *tfile, char *gfile) ;
    void get_info_thermal () ;
    void read_geom  () ;
    void read_thermal() ;
    void resamp_grid() ;
    H5File *h5f_therm, *h5f_geom  ;
    int nx_grid, ny_grid ;
    float *lat, *lon, *solzen, *solaz ;
    float startlat, startlon, endlat, endlon, gridspace ;
    float *band10, *band13, *band16 ;
    float b13_scale, b13_off, b16_scale, b16_off ;
    bool nightFlag ;
    
private:
    int nx, ny ;
};

#endif /* VIIRS_NC_H */

