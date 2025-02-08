#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>

#include <stdlib.h>
int main(int argc, char **argv)
{
	struct passwd *pwd;
	pwd = getpwuid(atoi(argv[1]));
	printf("%s : %s\n", pwd->pw_name, pwd->pw_passwd);

	// free(pwd); // The return value may point to a static area

	return 0;
}
