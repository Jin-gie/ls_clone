#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

// const sinon le programme peut retrier et retrier et planter
// ici permet de s'assurer que les pointeurs ne changent pas
int compare(const void *a, const void *b);

void fullInfo(char *absolutePath, char *fileName, int i);

int main(int argc, char **argv)
{   
    DIR* rep;
    struct dirent *file;
    struct dirent **list;
    int opt, inode, all, ldesc; /*size*/
    int i = 0;
    char *absolutePath;

    while ((opt = getopt(argc, argv, "ial?")) != -1) {
        switch (opt)
        {
        case 'i':
            inode = 1; break;
        case 'a':
            all = 1; break; 
        case 'l':
            ldesc = 1; break;
        case '?':
            printf("Clone of the very well known ls command on unix system.\n");
            printf("Usage: -ls [document path] [-l] [-i] [-a]\n");
            printf("Options :\n");
            printf("-l: will shown all information of the directory\n");
            printf("-i: will display inode of files\n");
            printf("-a: will display also hidden files");
            printf("\n\nAuthor : Erin Bernardoni");
        default: break;
        }
    }

    /* Open and read directory */
    rep = (argv[optind] == NULL) ? opendir("./") : opendir(argv[optind]);
    if (rep == NULL)
        exit(1);
    for (; (file = readdir(rep)); i++);
    list = malloc(sizeof(*list) * i);
    for (i = 0, rewinddir(rep); (file = readdir(rep)); i++)
        list[i] = file;
    
    qsort(list, i-1, sizeof(*list), compare); // TODO make an argument for this


    /* Formate absolute path to finish with "/" if one is given as argument */
    if (argv[optind] != NULL) {
        absolutePath = malloc(strlen(argv[optind]) + 2);
        strcpy(absolutePath, argv[optind]);
        if (argv[optind][strlen(argv[optind])-1] != '/')
            strcat(absolutePath, "/");
    }


    /* Display */
    for (int tmp = 0; tmp < i; tmp++)
        if (all == 1 || list[tmp]->d_name[0] != '.') {
            if (ldesc == 1) {
                fullInfo(absolutePath, list[tmp]->d_name, inode);
            }
            else {
                if (inode == 1)
                    printf("%ld %s ", list[tmp]->d_ino, list[tmp]->d_name);
                else
                    printf("%s ", list[tmp]->d_name);
            }
        }
    printf("\n");

    /* free the space */
    free(list);

    if (closedir(rep) == -1)
        exit(-1);

    return 0;
}


void fullInfo(char *absolutePath, char *fileName, int i) {
    char *path = malloc(strlen(absolutePath) + strlen(fileName) + 1);
    struct stat s;
    struct tm *t;
    char timebuf[80];
    strcpy(path, absolutePath);
    strcat(path, fileName);
    stat(path, &s);
    
    printf( (i == 1) ? "%ld\t" : "", s.st_ino);
    printf( (S_ISDIR(s.st_mode)) ? "d" : "-");
    printf( (s.st_mode & S_IRUSR) ? "r" : "-");
    printf( (s.st_mode & S_IWUSR) ? "w" : "-");
    printf( (s.st_mode & S_IXUSR) ? "x" : "-");
    printf( (s.st_mode & S_IRGRP) ? "r" : "-");
    printf( (s.st_mode & S_IWGRP) ? "w" : "-");
    printf( (s.st_mode & S_IXGRP) ? "x" : "-");
    printf( (s.st_mode & S_IROTH) ? "r" : "-");
    printf( (s.st_mode & S_IWOTH) ? "w" : "-");
    printf( (s.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
    printf("%ld", s.st_size);

    t = localtime(&(s.st_mtime));
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", t); /* mois jour heure:min */
    printf("%s ", timebuf);
    
    printf("%s\n", fileName);
    free(path);
}

int compare(const void *a, const void *b) { 
    return strcmp((*(struct dirent **) a)->d_name, (*(struct dirent **) b)->d_name);
}