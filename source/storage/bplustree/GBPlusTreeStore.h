#ifndef _MY_PACKAGE_BPLUSTREESTORE_H_
#define _MY_PACKAGE_BPLUSTREESTORE_H_

#ifdef __cplusplus
extern "C" {
#endif
  typedef void* GBPlusTreeStore;
  GBPlusTreeStore GBPlusInit(void);
  void GBPlusFree(GBPlusTreeStore);
  void GBPlusSet(GBPlusTreeStore,char *, char *);
  char* GBPlusGet(GBPlusTreeStore,char *);
  int GBPlusRemove(GBPlusTreeStore,char *);
  int GBPlusExist(GBPlusTreeStore,char *);
  void GBPlusTraverse(GBPlusTreeStore btree);
#ifdef __cplusplus
}
#endif

#endif