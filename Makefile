cxx = g++
cc = gcc
oclLib = -lOpenCL


all: SimpleCL SimpleCL_CUTWG_1D SimpleCL_CUTWG_1D_measured_time 

SimpleCL:SimpleCL.cpp
	${cxx} -o SimpleCL SimpleCL.cpp ${oclLib} 
	 
SimpleCL_CUTWG_1D:SimpleCL_CUTWG_1D.cpp
	${cxx} -o SimpleCL_CUTWG_1D SimpleCL_CUTWG_1D.cpp ${oclLib}  

SimpleCL_CUTWG_1D_measured_time:SimpleCL_CUTWG_1D_measured_time.cpp
	${cxx} -o SimpleCL_CUTWG_1D_measured_time SimpleCL_CUTWG_1D_measured_time.cpp ${oclLib} 





clean:
	rm -f *.o
	rm -f SimpleCL
	rm -f SimpleCL_CUTWG_1D
	rm -f SimpleCL_CUTWG_1D_measured_time
