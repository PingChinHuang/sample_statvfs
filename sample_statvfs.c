#include <sys/statvfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <mntent.h>

int main(int argc, char *argv[])
{
	struct statvfs stat;
	struct mntent ent;
	FILE *pFile = setmntent("/proc/mounts", "r");
	int ret;
	char buf[1024];

	if (pFile == NULL)
	{
		 perror("setmntent: ");
		 return 1;
	}

	while (NULL != getmntent_r(pFile, &ent, buf, sizeof(buf)))
	{
		printf("%s %s %s\n", ent.mnt_fsname, ent.mnt_dir, buf);
		ret = statvfs(ent.mnt_dir, &stat);
		if (ret < 0)
		{
			perror("statvfs: ");
			return 1;
		}
		printf("bsize %llu, blocks %llu, free %llu, avail %llu\n", stat.f_bsize, stat.f_blocks, stat.f_bfree, stat.f_bavail);
		printf("Free Size %lluKB\n", stat.f_bsize * stat.f_bfree / 1024);
		printf("Total Size %lluKB\n", stat.f_bsize * stat.f_blocks / 1024);
	}

	fclose(pFile);

	return 0; 
}

