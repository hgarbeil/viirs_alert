/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   viirs_nc.cpp
 * Author: harold
 * 
 * Created on July 13, 2018, 8:48 AM
 */

#include "viirs_nc.h"
#include <iostream>
#include <string>
#include <math.h>
using namespace std ;

viirs_nc::viirs_nc() {
    h5f_geom = 0L ;
    h5f_therm = 0L ;
    band13 = 0L ;
    band16 = 0L ;
    lat = 0L ;
    lon = 0L ;
    b13_scale = b16_scale = 1. ;
    b13_off = b16_off = 0. ;
    startlat = 20.33 ;
    startlon = -156.2 ;
    gridspace = .0068 ;
    nx_grid = int((startlat - endlat) / gridspace) + 1 ;
    ny_grid = int((endlon - startlon) / gridspace) + 1 ;
    
}

void viirs_nc::set_hdf_file (char *tfile, char *gfile) {
    hid_t hid ;
    h5f_therm = new H5File (tfile, H5F_ACC_RDONLY ) ;
    hid = h5f_therm->getId () ;
    h5f_geom= new H5File (gfile, H5F_ACC_RDONLY ) ;
    hid = h5f_geom->getId () ;
    
    cout << "file id is : " << hid << endl ;
    read_thermal () ;
    read_geom() ;
    /*
    read_geom() ;
    read_thermal() ;
    nightFlag = true ;
    band10 = 0L ;
    band13 = 0L ;
    band16 = 0L ;
    b13_scale = 1 ;
     
     
    
    nx_grid = int((startlat - endlat) / gridspace) + 1 ;
    ny_grid = int((endlon - startlon) / gridspace) + 1 ;
     */
}

void viirs_nc::read_thermal () {
    int i, j, ndims ;
    float sfac ;
    
    string dset_name ("/observation_data/M13") ;
    DataSet tband = h5f_therm->openDataSet (dset_name) ;
    string attname ("scale_factor") ;
    Attribute val = tband.openAttribute (attname) ;
    val.read (PredType::NATIVE_FLOAT, &b13_scale) ;
    attname.clear () ;
    attname.append ("add_offset") ;
    val = tband.openAttribute (attname) ;
    val.read (PredType::NATIVE_FLOAT, &b13_off) ;
    hsize_t dims[3] ;
    DataSpace dspace = tband.getSpace() ;
    ndims = dspace.getSimpleExtentDims (dims, NULL) ;
    DataSpace mspace (ndims, dims) ;
    int hg = 0 ;
    ny = dims [0] ;
    nx = dims [1] ;
    
    band13 = new float [nx * ny] ;
    band16 = new float [nx * ny] ;
    unsigned short int * temparr = new unsigned short int [nx * ny] ;
    tband.read (temparr, PredType::NATIVE_UINT16, mspace, dspace) ;
    cout << "Tband_13 " << temparr[3200*1500] * b13_scale + b13_off << endl ;
    for (i=0; i<ny * nx; i++) {
        band13[i] = temparr[i] * b13_scale + b13_off ;
    }
    tband.close() ;
    
    dset_name.clear() ;
    dset_name.append ("/observation_data/M16") ;
    tband = h5f_therm->openDataSet (dset_name) ;
    attname.clear () ;
    attname.append ("scale_factor") ;
    val = tband.openAttribute (attname) ;
    val.read (PredType::NATIVE_FLOAT, &b16_scale) ;
    attname.clear () ;
    attname.append ("add_offset") ;
    val = tband.openAttribute (attname) ;
    val.read (PredType::NATIVE_FLOAT, &b16_off) ;
    tband.read (temparr, PredType::NATIVE_UINT16, mspace, dspace) ;
    cout << "Tband_16 " << temparr[3200*1500] * b16_scale + b16_off<< endl ;
    
    
 }

void viirs_nc::read_geom () {
    DataSpace dspace ;
    DataSet dset ;
    int ndims ;
    hsize_t dims[3] ;
    string dset_name ("/geolocation_data/latitude") ;
    dset =  h5f_geom->openDataSet (dset_name) ;
    dspace = dset.getSpace() ;
    dspace.getSimpleExtentDims(dims, NULL) ;
    ndims = dspace.getSimpleExtentDims(dims, NULL) ;
    DataSpace mspace1 (ndims, dims) ;
    lat = new float [nx * ny] ;
    lon = new float [nx * ny] ;
    solzen = new float [nx * ny] ;
    solaz = new float [nx * ny] ;
    dset.read (lat, PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    dset_name.clear() ;
    dset_name.append ("/geolocation_data/longitude") ;
    dset =  h5f_geom->openDataSet (dset_name) ;
    dset.read (lon, PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    dset_name.clear() ;
    dset_name.append ("/geolocation_data/solar_zenith") ;
    dset =  h5f_geom->openDataSet (dset_name) ;
    dset.read (solzen, PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    dset_name.clear() ;
    dset_name.append ("/geolocation_data/solar_azimuth") ;
    dset =  h5f_geom->openDataSet (dset_name) ;
    dset.read (solaz, PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    dset_name.clear() ;
    for (int i=0; i<nx * ny; i++){
        solzen[i] /= 100. ;
        solaz[i] /= 100. ;
        
    }
    /*
    dset_name.append ("/All_Data/VIIRS-MOD-GEO_All/SolarAzimuthAngle") ;
    dset =  h5f_geom->openDataSet (dset_name) ;
    dset.read (solaz,  PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    */
    cout << "Ave lat is " << lat [1500 * 3200 + 1600] << endl ; 
    cout << "Ave lon is " << lon [1500 * 3200 + 1600] << endl ; 
    cout << "Ave zen  is " << solzen [1500 * 3200 + 1600] << endl ; 
    cout << "Ave azim is " << solaz [1500 * 3200 + 1600] << endl ; 
    if (solzen[1500 * 3200 + 1600]<90.) 
        nightFlag = false ;
    
}


void viirs_nc::resamp_grid () {
    int i, ixloc, iyloc ;
    float latval, lonval, xloc, yloc, xdist, ydist, dist ;
    float *outgrid = new float [nx_grid * ny_grid * 2] ;
    float *distgrid = new float [nx_grid * ny_grid] ;
    for (i=0; i< nx * ny; i++) {
        latval = lat[i] ;
        lonval = lon [i] ;
        xloc = (lonval - startlon) / gridspace ;
        yloc = (startlat - latval) / gridspace ;
        ixloc = int(xloc + 0.5) ;
        iyloc = int(yloc + 0.5) ;
         
        if (xloc < 0 || xloc > nx_grid) continue ;
        if (yloc < 0 || yloc > ny_grid) continue ;
        xdist = xloc - ixloc ;
        ydist = yloc - iyloc ;
        dist = sqrt (xdist * xdist + ydist * ydist) ;
        //xdist =  
    }
}

viirs_nc::viirs_nc(const viirs_nc& orig) {
}

viirs_nc::~viirs_nc() {
    delete [] lat ;
    delete [] lon ;
    delete [] solzen ;
    delete [] solaz ;
    h5f_geom->close() ;
    h5f_therm->close() ;
}

