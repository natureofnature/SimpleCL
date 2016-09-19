/*
 This is a simple kernel for testing.
*/
__kernel void VectorAdd(__global float* In, __global float* out, const int size){
    int thread_id = get_global_id(0);
    if(thread_id>=size)return;
    out[thread_id]=In[thread_id]+1;
}
