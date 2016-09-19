/*
 This is a simple kernel for testing.
*/



__kernel void VectorAdd(__global float* In, __global float* out, const int size){
	int localID = get_local_id(0);
	int globalID = get_global_id(0);
	int groupID = get_group_id(0);
	int localsize = get_local_size(0);
	float a = exp2(In[globalID]);
	float b = acosh(In[globalID]);
	float c = hypot(a, b);
	float d = sin(a + b + c);

	//printf("%f,%f,%f,%f\n",a,b,c,d);
#ifdef COMINTENSIVE
	for (int i = 0; i<10; i++) {
		a = (float)1 / (1+In[globalID]);
		b = acosh(b + 2*In[globalID-1]);
		c = hypot(a, b)+In[localID];
		d = sin(a + b + c + d);
	}
#else
	for (int i = 0; i<10; i++) {
		b = In[globalID] + c;
		a = In[globalID + i%1000];
		d = In[globalID] + hypot(a, b);
		c = sin(a + b + c + d);
	}
#endif
	//printf("%f\n",d);
	out[globalID] = d;
	if (globalID < size) {
		out[globalID] = In[globalID]+1;
	}
}

__kernel void VectorFun(__global float* In,__global float* out, const int size){
	int localID = get_local_id(0);
	int globalID = get_global_id(0);
	int groupID = get_group_id(0);
	int localsize = get_local_size(0);
	float a = (float)1 / (1 + In[globalID]);
	float b = acosh(In[globalID]);
	float c = hypot(a, b);
	float d = sin(a + b + c);

	//printf("%f,%f,%f,%f\n",a,b,c,d);

	for (int i = 0; i<10; i++) {
		b = In[localID]+c;
		a = In[localID+1];
		d = In[globalID]+ hypot(a, b);
		c = sin(a + b + c + d);	
	}
	out[globalID] = d;
	if (globalID < size) {
		out[globalID] = In[globalID]+1;
	}
}
