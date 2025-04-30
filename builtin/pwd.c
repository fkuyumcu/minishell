#include <unistd.h>
#include "../src/minishell.h"
void	pwd()
{
	char *pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
}