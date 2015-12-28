class DevNode;
class DevGathering;

bool DoCuda_Init();
bool DoCuda_OnDraw(unsigned int* out, DevNode** root, const int imageW, const int imageH, const struct fsize* pView, const struct matrix* pMatrix);
bool mallocDev(DevNode*** out);
bool freeDevicePointer(DevNode** devNode);
bool DoCuda_updateMatrix(DevNode** devNode, const struct matrix* pMatrix);
bool DoCuda_SetRoot(DevNode** devNode, DevNode** devRoot);
bool DoCuda_AddNode(DevGathering** devGathering, DevNode** devNode);
bool DoCuda_DeleteNode(DevGathering** devGathering, DevNode** devNode);
bool DoCuda_updateColor(DevNode** devNode, float r, float g, float b);
bool DoCuda_updateMaterial(DevNode** devNode, float reflect, float refractive, float through);

