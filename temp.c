struct vinode {
  int inum;                  // inode number of the disk inode
  int ref;                   // reference count
  int size;                  // size of the file (in bytes)
  int type;                  // one of FILE_I, DIR_I
  int nlinks;                // number of hard links to this file
  int blocks;                // number of blocks
  int addrs[DIRECT_BLKNUM];  // direct blocks
  void *i_fs_info;           // filesystem-specific info (see s_fs_info)
  struct super_block *sb;          // super block of the vfs inode
  const struct vinode_ops *i_ops;  // vfs inode operations
};

struct dentry {
  char name[MAX_DENTRY_NAME_LEN];
  int d_ref;
  struct vinode *dentry_inode;
  struct dentry *parent;
  struct super_block *sb;
};