#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct command{
  int argc;
  char**argv;
  char* cmdline;
  int flag_background;
};

/* create a command structure from a command line  */
struct command* extract_command(char* cmdline) {
  struct command* c = malloc(sizeof(struct command));
  c->argc = 0;
  c->argv = NULL;
  c->cmdline = malloc(sizeof(char)*(strlen(cmdline)+1));
  strcpy(c->cmdline, cmdline);

  /* first, let's count the number of parameters */
  char* token = strtok(cmdline, " ");
  while(token) {
    c->argc++;
    token = strtok(NULL, " ");
  }
  /* strtok modified cmdline, so let's restore it */
  strcpy(cmdline, c->cmdline);

  /* now, extract the parameters */
  c->argv = malloc(sizeof(char*) * (c->argc+1));
  c->argv[0] = strtok(cmdline, " ");
  int i;
  for(i=1; i<c->argc; i++) {
    c->argv[i] = strtok(NULL, " ");
  }

  if(c->argc && strcmp("&", c->argv[c->argc-1]) == 0) {
    c->argc--;
    c->flag_background = 1;
  }
  else {
    c->flag_background = 0;
  }
  c->argv[c->argc] = NULL;

  return c;
}

/* execute a command */
void execute_command(struct command* c) {
  if(c->argc == 0) {
    return;
  }
  else {
    int res_val;
    res_val = fork();

    if(res_val != 0 && strcmp(c->argv[0], "exit") != 0 && !c->flag_background) {
      waitpid(res_val, 0, 0);
    }
    else if (res_val == 0 && strcmp(c->argv[0], "exit") != 0) {
      execvp(c->argv[0], c->argv);
    }
  }
}

int main(int argc, char** argv){

  do {
    char *cmdline = NULL;
    /* print a prompt, and return a buffer that contains the user command */
    cmdline = readline("mysh $ ");

    /* extract the command, and execute it */
    struct command* cmd = extract_command(cmdline);
    execute_command(cmd);

    if(cmd->argc > 0 && strcmp(cmd->argv[0] , "exit") == 0) {
      free(cmd->argv);
      free(cmd->cmdline);
      free(cmd);
      free(cmdline);
      return EXIT_SUCCESS;
    }


    /* cleanup */
    free(cmd->argv);
    free(cmd->cmdline);
    free(cmd);
    free(cmdline);
    cmdline = NULL;
  } while(1);

  return EXIT_SUCCESS;
}
