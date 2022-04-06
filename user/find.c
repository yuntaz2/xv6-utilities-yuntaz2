// lab-1 activity-4
// based on ls.c and inspired by Prof.Klefstad's lecture on April 5th, 2022
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), 0, DIRSIZ - strlen(p));
    return buf;
}

// Don't recurse into "." and "..".
int checkRecurse(char *path)
{
    char *name = fmtname(path);
    if (name[0] == '.' && name[1] == 0)
    {
        return 0;
    }
    else if (name[0] == '.' && name[1] == '.' && name[2] == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strcmp(fmtname(path), name) == 0)
    {
        printf("%s\n", path);
    }

    switch (st.type)
    {
    case T_FILE:
        printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            /*
                        if (strcmp(fmtname(buf), name) == 0)
                        {
                            printf("%s\n", buf);
                        }
            */
            if (checkRecurse(buf))
            {
                find(buf, name);
            }

            printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    switch (argc)
    {
    case (1):
    {
        printf("Incorrect argc for find(): No arg but at least 1 arg is needed.");
        exit(-1);
    }
    case (2):
    {
        find(".", argv[1]);
        exit(0);
    }
    case (3):
    {
        find(argv[1], argv[2]);
        exit(0);
    }
    }
    exit(0);
}
