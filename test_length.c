# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>

char *
resolve_path (
    char * path
    )
{
    char full_path[PATH_MAX];
    char *rpath, *home_env, *ret;
    size_t full_len, home_len = 0;

    full_len = strlen(path);

    if (*path == '~') {
        home_env = getenv("HOME");
        if (home_env != NULL)
            home_len = strlen(home_env);
    }

    if (full_len + home_len - 1 > PATH_MAX) {
        fprintf(stderr, "path too long\n");
        exit(EXIT_FAILURE);
    }

    if ((rpath = (char *) malloc(PATH_MAX)) == NULL) {
        fprintf(stderr, "malloc: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (home_env != NULL)
        sprintf((char *) full_path, "%s%s", home_env, path+1);
    else
        strcpy((char *) full_path, path);

    if ((ret = realpath(full_path, rpath)) == NULL) {
        fprintf(stderr, "realpath: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return ret;
}

int main ()
{
    char *rpath;

    printf("PATH_MAX: %i\n", PATH_MAX);
    //printf("MAXPATHLEN: %i\n", MAXPATHLEN);
    printf("NAME_MAX: %i\n", NAME_MAX);
    //printf("MAX_PATH_LEN: %i", MAX_PATH_LEN);
    
    rpath = resolve_path("~//../../usr/include/stdio.h");
    printf("rpath: %s\n", rpath);

    exit(EXIT_SUCCESS);
}
