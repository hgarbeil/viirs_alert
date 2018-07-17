/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   viirs_hdf.cpp
 * Author: harold
 * 
 * Created on July 13, 2018, 8:48 AM
 */

#include "viirs_hdf.h"
#include <iostream>
#include <string>
using namespace std ;

viirs_hdf::viirs_hdf() {
    h5f = 0L ;
}

void viirs_hdf::set_hdf_file (char *infile) {
    hid_t hid ;
    h5f = new H5File (infile, H5F_ACC_RDONLY ) ;
    hid = h5f->getId () ;
    
    cout << "file id is : " << hid << endl ;
    get_info () ;
    read_geom() ;
    read_thermal() ;
    nightFlag = true ;
    band10 = 0L ;
    band13 = 0L ;
    band16 = 0L ;
}

void viirs_hdf::get_info () {
    int ndims ;
    string dset_name ("/All_Data/VIIRS-MOD-GEO_All/Latitude") ;
    DataSet set_lat = h5f->openDataSet (dset_name) ;
    
    IntType it = set_lat.getIntType ()  ;
    cout << "Size is " << it.getSize() << endl ;
    hsize_t dims[3] ;
    DataSpace dspace = set_lat.getSpace() ;
    
    ndims = dspace.getSimpleExtentDims (dims, NULL) ;
    int hg = 0 ;
    nx = dims [0] ;
    ny = dims [1] ;
    
}

void viirs_hdf::read_geom () {
    DataSpace dspace ;
    DataSet dset ;
    int ndims ;
    hsize_t dims[3] ;
    string dset_name ("/All_Data/VIIRS-MOD-GEO_All/Latitude") ;
    dset =  h5f->openDataSet (dset_name) ;
    dspace = dset.getSpace() ;
    ndims = dspace.getSimpleExtentDims(dims, NULL) ;
    DataSpace mspace1 (ndims, dims) ;
    lat = new float [nx * ny] ;
    lon = new float [nx * ny] ;
    solzen = new float [nx * ny] ;
    solaz = new float [nx * ny] ;
    dset.read (lat, PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    dset_name.clear() ;
    dset_name.append ("/All_Data/VIIRS-MOD-GEO_All/Longitude") ;
    dset =  h5f->openDataSet (dset_name) ;
    dset.read (lon, PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    dset_name.clear() ;
    dset_name.append ("/All_Data/VIIRS-MOD-GEO_All/Longitude") ;
    dset =  h5f->openDataSet (dset_name) ;
    dset.read (lon, PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    dset_name.clear() ;
    dset_name.append ("/All_Data/VIIRS-MOD-GEO_All/SolarZenithAngle") ;
    dset =  h5f->openDataSet (dset_name) ;
    dset.read (solzen, PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    dset_name.clear() ;
    dset_name.append ("/All_Data/VIIRS-MOD-GEO_All/SolarAzimuthAngle") ;
    dset =  h5f->openDataSet (dset_name) ;
    dset.read (solaz,  PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    cout << "Ave lat is " << lat [1500 * 3200 + 1600] << endl ; 
    cout << "Ave lon is " << lon [1500 * 3200 + 1600] << endl ; 
    cout << "Ave zen  is " << solzen [1500 * 3200 + 1600] << endl ; 
    cout << "Ave azim is " << solaz [1500 * 3200 + 1600] << endl ; 
    if (solzen[1500 * 3200 + 1600]<90.) 
        nightFlag = false ;
    
}

void viirs_hdf::read_thermal () {
    band13 = new float [nx * ny] ;
    band16 = new float [nx * ny] ;
    unsigned short int * temparr = new unsigned short int [nx * ny] ;
    float scalefacs [8] ;
    DataSpace dspace ;
    DataSet dset ;
    int ndims, dsize ;
    hsize_t dims[3], dimsrfac [3];
    string dset_name ("/All_Data/VIIRS-M13-SDR_All/Radiance") ;
    dset =  h5f->openDataSet (dset_name) ;
    IntType it = dset.getIntType ()  ;
    dsize = it.getSize() ;
    cout << "B13 Size is " << dsize << endl ;
    dspace = dset.getSpace() ;
    ndims = dspace.getSimpleExtentDims(dims, NULL) ;
    DataSpace mspace1 (ndims, dims) ;
    if (dsize==4) {
        dset.read (band13,  PredType::NATIVE_FLOAT, mspace1, dspace ) ;
    }
    else {
        dset.read (temparr, PredType::NATIVE_UINT16, mspace1, dspace) ;
        string dset_name ("/All_Data/VIIRS-M13-SDR_All/RadianceFactors") ;
        dset =  h5f->openDataSet (dset_name) ;
        IntType it = dset.getIntType ()  ;
        dsize = it.getSize() ;
        cout << "B16 Size is " << dsize << endl ;
        DataSpace dspacerfac = dset.getSpace() ;
        int ndimsrfac =  dspacerfac.getSimpleExtentDims(dimsrfac, NULL) ;
        DataSpace mspacerad (ndimsrfac, dimsrfac) ;
        dset.read (scalefacs, PredType::NATIVE_FLOAT, mspacerad, NULL) ;
        for (int i=0; i<nx * ny; i++ ){
            band13[i] = temparr[i] * scalefacs[0] + scalefacs[1] ;
        }
        
    }
   
    dset =  h5f->openDataSet ("/All_Data/VIIRS-M16-SDR_All/Radiance") ;
    it = dset.getIntType ()  ;
    dsize = it.getSize() ;
    cout << "B16 Size is " << dsize << endl ;
    dspace = dset.getSpace() ;
    ndims = dspace.getSimpleExtentDims(dims, NULL) ;
    DataSpace mspace2 (ndims, dims) ;
    if (dsize==4) {
        dset.read (band16,  PredType::NATIVE_FLOAT, mspace2, dspace ) ;
    }
    else {
        dset.read (temparr, PredType::NATIVE_UINT16, mspace2, dspace) ;
        string dset_name ("/All_Data/VIIRS-M16-SDR_All/RadianceFactors") ;
        dset =  h5f->openDataSet ("/All_Data/VIIRS-M16-SDR_All/RadianceFactors") ;
        IntType it = dset.getIntType ()  ;
        dsize = it.getSize() ;
        cout << "B16 Size is " << dsize << endl ;
        DataSpace dspacerfac = dset.getSpace() ;
        int ndimsrfac =  dspacerfac.getSimpleExtentDims(dimsrfac, NULL) ;
        DataSpace mspacerad (ndimsrfac, dimsrfac) ;
        dset.read (scalefacs, PredType::NATIVE_FLOAT, mspacerad, NULL) ;
        for (int i=0; i<nx * ny; i++ ){
            if (temparr[i] > 32767) {
                band16[i] = -999. ;
                continue ;
            }
            band16[i] = temparr[i] * scalefacs[0] + scalefacs[1] ;
        }
        
    }
    
}

viirs_hdf::viirs_hdf(const viirs_hdf& orig) {
}

viirs_hdf::~viirs_hdf() {
    delete [] lat ;
    delete [] lon ;
    delete [] solzen ;
    delete [] solaz ;
}

