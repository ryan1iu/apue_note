# 目录和文件

获取文件属性
stat

文件访问权限
st_mode 是一个16位的位图

umask: 防止产生权限过松的文件

文件权限的修改
chmod
fchmod

粘住位
t位，现在已经不再常用，目的是让某一个二进制文件在执行完成后留在内存中方便下一次加载

文件系统：FAT和UFS

硬链接、符号链接
link
unlink

rename
remove

remove() deletes a name from the filesystem.  It calls unlink(2) for files, and rmdir(2) for directories.

硬链接与目录项是同义词，硬链接建立有限制：不能够跨分区建立，不能够给目录建立；符号链接可以

utime: 更改文件最后读的时间和最后修改的时间

目录的创建和销毁
mkdir
rmdir

更改当前工作路径
chdir
fchdir
getcwd

分析目录/读取目录内容
glob
opendir
readdir
rewinddir
seekdir
closedir
