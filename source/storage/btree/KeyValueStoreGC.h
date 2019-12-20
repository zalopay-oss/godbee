#ifndef _MY_PACKAGE_BTREESTORE_H_
#define _MY_PACKAGE_BTREESTORE_H_

#ifdef __cplusplus
extern "C" {
#endif
    typedef void* KeyValueStoreDiskGC;
    KeyValueStoreDiskGC BTreeInit(void);
    void BTreeFree(KeyValueStoreDiskGC);
    void BTreeSet(KeyValueStoreDiskGC,char *, char *);
    const char* BTreeGet(KeyValueStoreDiskGC,char *);
    int BTreeRemove(KeyValueStoreDiskGC,char *);
    int BTreeExist(KeyValueStoreDiskGC,char *);
    void BTreeTraverse(KeyValueStoreDiskGC btree);
#ifdef __cplusplus
}
#endif

#endif