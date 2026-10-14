#include<iostream>
#include<unistd.h>
#include<sys/wait.h>

using namespace std;
int main() {
    pid_t pid=fork();
    if(pid < 0){
      cerr<< "No fue posible crear el proceso hijo " <<endl;
      return 1;
    } else if(pid ==0){
      cout<<"Hijo: ejecutando ls..."<<endl;
      execl("/bin/ls",ls,(char *)NULL);
      cerr<<"Error al ejecutar ls"<<endl;
      return 1;
    }else{
      cout<<"Padre : esperando al proceso hijo ..."<<endl;
      wait(NULL);
      cout<<"Padre:el proceso hijo ha terminado."<<endl;
      
    }
    
    return 0;
}
