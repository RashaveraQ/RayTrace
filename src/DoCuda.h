class DevNode;

bool DoCuda_Init();
bool DoCuda_OnSize(void** dst, const int imageW, const int imageH);
bool DoCuda_OnDraw(unsigned long* out, void* d_dst, DevNode* root, const int imageW, const int imageH, const struct fsize* pView, const struct matrix* pMatrix);
bool DoCuda_Free(void* dst);
DevNode* mallocDevicePointer();
bool freeDevicePointer(DevNode* devNode);