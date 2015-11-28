bool DoCuda_Init();
bool DoCuda_OnSize(void** dst, const int imageW, const int imageH);
void DoCuda_OnDraw(unsigned long* out, void* d_dst, class Node* root, const int imagew, const int imageh);
bool DoCuda_Free(void* dst);
