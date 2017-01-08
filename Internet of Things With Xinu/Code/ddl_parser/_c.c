#include<xinu.h>

devcall buttonread(struct dentry	*devptr, char	*buff,  int32	count)
{
write_pin(1, buff);
return OK;}