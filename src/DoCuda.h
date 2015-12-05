bool DoCuda_Init();
bool DoCuda_OnSize(void** dst, const int imageW, const int imageH);
bool DoCuda_OnDraw(unsigned long* out, void* d_dst, class Node** root, const int imageW, const int imageH, const struct fsize* pView, const struct matrix* pMatrix);
bool DoCuda_Free(void* dst);
bool mallocDevicePointer(class Node*** devPtr);
bool freeDevicePointer(class Node** devPtr);