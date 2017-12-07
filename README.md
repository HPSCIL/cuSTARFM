cuSTARFM
========
STARFM is a remote sensing image fusion algorithm conceived by Doctor Feng Gao to blend high-frequency temporal information from MODIS and high-resolution spatial information from Landsat to acqiure high resolution in both time and space. STARFM blends one or more pairs of Landsat and MODIS maps that acquired on the same date and one MODIS map from the prediction date to predict daily maps at Landsat spatial resolution.cuSTARFM is the GPU parallel implementation of STARFM which accelerates STARFM with CUDA. cuSTARFM could achieve high speedup ratios compared to cpu-based ESTARFM without calculation error.

How to use
----------
Parameter setting：

tifFile1——The path of High resolution Landsat image data at reference time T1,type char.

modFile1——The path of High resolution MODIS image data at reference time T1,type char.

modFile0——The path of High resolution MODIS image data at prediction time T0,type char.

out——The path of the image fusion result,Type char.

w——The width of searching window,type int.

classnum——Assumed number of classifications，type int.

A——The relative importance of space distance relative to temporal difference and Spectral differences,type int.

pf——The pf band of Landsat for calculating,type int.

pc——The pc band of MODIS for calculating,type int.

L_err——Landsat sensor error,type float.

M_err——MODIS sensor error,type float.

Sample
----------
```c
const char* modFile1="D:\\data\\newdata\\MODO9A1.A2009249.dat";
const char* tifFile1="D:\\data\\newdata\\2009-249-flaash.dat";
const char* modFile0="D:\\data\\newdata\\MODO9A1.A2009329-0.dat";
const char* out="D:\\data\\newdata\\teststrafm.tif";
int w=31;
int classnum=6;
int A=25;
int pf=3;
int pc=2;
float L_err=0.2/100
float M_err=0.5/100;
Re_fusion(tifFile1, modFile1,modFile0,out,w,classnum,A,pf,pc,L_err,M_err);//c
```
## References

F. Gao, J. Masek, M. Schwaller, and F. Hall, “On the blending of the Landsat and MODIS surface reflectance: Predicting daily Landsat surface reflectance,” IEEE Trans. Geosci. Remote Sens., vol. 44, pp. 2207–2218, 2006.
