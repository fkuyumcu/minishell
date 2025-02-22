#include <unistd.h>

void	pwd()
{
	char *pwd;

	pwd = getcwd(NULL, 0);
	printf("%s", pwd);
}