/* 解析通配符 */
#include <glob.h>

#include <stdio.h>
#include <string.h>
int main()
{
	char *pat = "/home/ryan/learn_subject/unix_system_programing/*";
	// char pat[32] = "./*";
	glob_t globres;
	glob(pat, GLOB_PERIOD, NULL, &globres);

	int globc = globres.gl_pathc;
	for (int i = 0; i < globc; i++) {
		printf("%s\n", globres.gl_pathv[i]);
	}

	return 0;
}
