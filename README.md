cuSTARFM
========
Version 1.0

Overview
========
MODIS and Landsat surface reflectance products have complementary characteristics in terms of spatial and temporal resolutions. To fully exploit these datasets, the Spatial and Temporal Adaptive Reflectance Fusion Model (STARFM) was developed by Gao et al. (2006). The STARFM approach blends the high-frequency temporal information from MODIS and the high-resolution spatial information from Landsat to generate synthetic surface reflectance products at 30m spatial resolution and daily temporal resolution. STARFM uses one or more pairs of Landsat-MODIS images collected on the same dates to predict the surface reflectance at Landsat resolution on other MODIS observation dates. However, the computational performance of STARFM has been a bottleneck for mass production, even with the parallel option in STARFM v1.2 (Geo, et al., 2015; Gao, et al., 2017). 

To overcome the computational barrier and support mass production of large-size images, we designed and implemented a GPU-enabled STARFM program based on the Compute Unified Device Architecture (CUDA), called cuSTARFM. By taking advantages of the large amount of concurrent computing threads of a GPU, cuSTARFM can greatly reduce the computing time and improve the computational performance. Experiments showed that cuSTARFM achieved a speedup of 342 using a Nvidia Tesla K40 GPU, compared with a sequential STARFM program running on an Intel Xeon E3-1226 CPU. 

Key features of cuSTARFM:
========
+ Supports a wide range of CUDA-enabled GPUs (https://developer.nvidia.com/cuda-gpus)  
  - Automatic setting of the numbers of threads and thread blocks according to the GPU’s available computing resources (e.g., memory, streaming multiprocessors, and warp)  
  - Adaptive cyclic task assignment to achieve better load balance
  - Optimized use of registers to improve the computational performance
  - Adaptive data decomposition when the size of images exceeds the GPU’s memory  
  -	All above are completely transparent to users
+	Intakes any number of pairs of Landsat-MODIS images as the input
+ Outputs any number of prediction images
+ Supports a wide range of image formats (see http://gdal.org/formats_list.html)
+ Includes a Windows version and a Linux/Unix version

References
========
+ Gao, F., Masek, J., Schwaller, M., and Hall, F. On the Blending of the Landsat and MODIS Surface Reflectance: Predict Daily Landsat Surface Reflectance, IEEE Transactions on Geoscience and Remote Sensing. 2006, 44(8):2207-2218.
+ Gao, F., Hilker, T., Zhu, X., Anderson, M. A., Masek, J., Wang, P. and Yang, Y. Fusing Landsat and MODIS data for vegetation monitoring, IEEE Geoscience and Remote Sensing Magazine. 2015, 3(3):47-60. 
+ Gao, F., Anderson, M., Zhang, X., Yang, Z., Alfieri, J., Kustas, W., Mueller, R., Johnson, D. and Prueger, J. Toward mapping crop progress at field scales through fusion of Landsat and MODIS imagery. Remote Sensing of Environment. 2017, 188:9-25

To Cite cuSTARFM in Publications
========
+ A paper describing cuSTARFM will be submitted to a scientific journal for publication soon
+	For now, you may just cite the URL of the source codes of cuSTARFM (https://github.com/HPSCIL/cuSTARFM) in your publications

Compilation
========
+ Requirements:
  -	A computer with a CUDA-enabled GPU (https://developer.nvidia.com/cuda-gpus)
  -	A C/C++ compiler (e.g., Microsoft Visual Studio for Windows, and gcc/g++ for Linux/Unix) installed and tested
  -	Nvidia CUDA Toolkit (https://developer.nvidia.com/cuda-downloads) installed and tested
  -	Geospatial Data Abstraction Library (GDAL, http://gdal.org) installed and tested
+ For the Windows version (using MS Visual Studio as an example)
  1. Open all the source codes in Visual Studio
  2. Click menu Project -> Properties -> VC++ Directories -> Include Directories, and add the “include” directory of GDAL (e.g., C:\GDAL\include\)
  3. Click menu Project -> Properties -> VC++ Directories -> Lib Directories, and add the “lib” directory of GDAL (e.g., C:\GDAL\lib\)
  4. Click menu Build -> Build Solution  
  Once successfully compiled, an executable file, cuSTARFM.exe, is created.
+ For the Linux/Unix version (using the CUDA compiler --- nvcc)  
In a Linux/Unix terminal, type in: 
  1. cd /the-directory-of-source-codes/
  2. nvcc -o cuSTARFM kernel.cu cuLayer.cpp cuSTARFM_util.cpp fusion.cpp -lgdal  
  Once successfully compiled, an executable file, cuSTARFM, is created.
  
Usage 
========
+ Before running the program, make sure that all Landsat and MODIS images have been pre-processed and co-registered. They must have:
  - the same spatial resolution (i.e., Landsat resolution --- 30m)
  - the same image size (i.e., numbers of rows and columns)
  - the same map projection
+ A text file must be manually created to specify the input and output images, and other parameters for the STARFM model.  
Example (# for comments):

> STARFM_PARAMETER_START
>
> #The number of input pairs of Landsat-MODIS images (>=1)  
>   NUM_IN_PAIRS = 1
>
> #The pf band of Landsat  
>   The_pf_band_of_Landsat_for_calculating = 3
>
> #The pc band of MODIS  
>   The_pc_band_of_MODIS_for_calculating = 1
>
> #The input MODIS images     
> #When NUM_IN_PAIRS > 1, multiple images must be given  
> #File names are separated by space  
>   IN_PAIR_MODIS_FNAME = D:\data\newdata\MODO9A1.A2009249.dat
>
> #The input Landsat images    
> #When NUM_IN_PAIRS > 1, multiple images must be given    
> #File names are separated by space    
>   IN_PAIR_LANDSAT_FNAME = D:\data\newdata\2009-249-flaash.dat
>
> #The MODIS images for the prediction dates  
> #Multiple images can be given  
> #File names are separated by space  
>   IN_PDAY_MODIS_FNAME = D:\data\newdata\MODO9A1.A2009329-0.dat D:\data\newdata\MODO9A1.A2009329-0.dat
>
> #The output synthetic prediction images  
> #Multiple images can be given  
> #File names are separated by space  
>   OUT_PDAY_LANDSAT_FNAME = D:\data\newdata\2009-329new-flaash-test2.tif D:\data\newdata\2009-329new-flaash-test3.tif
>
> #The_width of searching_window  
>   The_width_of_searching_window = 31
>
> #Assumed_number of classifications  
>   Assumed_number_of_classifications = 4
>
> #The relative importance of space distance   
>   The_relative_importance_of_space_distance = 25
>
> #Landsat sensor error   
>   Landsat_sensor_error = 20
>
> #MODIS sensor error  
>  MODIS_sensor_error = 50
>
> #Output image format (optional)    
> #Will be used when the extension of the output files    
> #is not given    
>   G_Type = GTIff
>
> STARFM_PARAMETER_END

Note: MODIS and Landsat images use different band number sequence. You can relate them using the following table:  

Landsat(pf) | 	MODIS(pc)  
----------|----------
   1      |   	         3  
   2      |            	 4  
   3      |  	           1  
   4      | 	           2  
   5      |              6  
   7      |              7  

+ The program runs as a command line. You may use the Command (i.e., cmd) in Windows, or a terminal in Linux/Unix. 
   - For the Windows version:    
   cuSTARFM.exe input.txt 
   - For the Linux/Unix version:   
   ./cuSTARFM input.txt  
  Note: The computational performance of cuSTARFM largely depends on the GPU. The more powerful is the GPU, the better performance. 

