#include "FichierClient.h"
/*Pour open*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*POur close | write | read*/
#include <unistd.h>
#include <string.h>//pour read et write
#include <stdio.h>
#include <errno.h>



//const char* filename = FICHIER_CLIENTS;

int estPresent(const char* nom)
{
  // TO DO
  CLIENT client;
  int fd;
  //int offset;
  int cpt=0;//compteur de position
  fd=open(FICHIER_CLIENTS ,O_RDONLY);
  printf("\n\nfd=%d\n\n\n",fd);
  if (fd==-1)
  {
    perror("\nErreur de open(): ");
    return -1;
  }
  else
  {
    while(read(fd,&client,sizeof(CLIENT))==sizeof(CLIENT))
    {
      cpt++;
      
      if(strcmp(client.nom, nom)==0)
      {
        //offset= (lseek(fd,0, SEEK_CUR))- sizeof(CLIENT);
        printf("\n\nCLIENT TROUVE");
        close(fd);
        return cpt;// quand client trouve
      }
      

    }

  }

  close(fd);
  return 0;// si  client pas trouvé 
    
}

////////////////////////////////////////////////////////////////////////////////////
int hash(const char* motDePasse)
{
  // TO DO
  int i;
  int h=0;
  for(i=0; i<strlen(motDePasse); i++)
  { 
    h= (i+1)*motDePasse[i] + h;
  }
  
  return (h % 97);
}

////////////////////////////////////////////////////////////////////////////////////
void ajouteClient(const int socket,const char* nom, const char* motDePasse)
{
  // TO DO
  int fd;
  CLIENT client;

  
  strcpy(client.nom,nom);
  client.hash=hash(motDePasse);

  fd=open(FICHIER_CLIENTS,O_WRONLY|O_CREAT|/*O_APPEND|*/O_EXCL,0644);//ouvre fichier fichier

  if(fd==-1)
  {
    fd=open(FICHIER_CLIENTS,O_WRONLY|O_APPEND);//ouvre fichier fichier
  }

  
  if(write(fd,&client,sizeof(CLIENT))==sizeof(CLIENT))
  {

    printf("\n\nnom:%s   &&   mdp:%d ",client.nom,client.hash);
    printf("CLIENT AJOUTE AVEC SUCCE\n");
  }
  else
  {
    perror("Erreur du write(): ");
   
  }

  close(fd);

}

////////////////////////////////////////////////////////////////////////////////////
int verifieMotDePasse(int pos, const char* motDePasse)
{
  // TO DO
  CLIENT client;
  int fd;
  int hashFind=0;
  fd=open(FICHIER_CLIENTS,O_RDONLY );
  if (fd==-1)
  {
    perror("Erreur de open(): ");
    return -1;
  }
  else
  {
    lseek(fd,(pos-1)*sizeof(CLIENT),SEEK_SET);

    if(read(fd,&client,sizeof(CLIENT))==-1)
    {
      perror("Erreur de read ");
    }
    else
    {
      hashFind=hash(motDePasse);
      printf("\n\nnomClient:%s\nHashClient:%d\n\n",client.nom,client.hash);
      if(hashFind==client.hash)
      {
        close(fd);
        return 1; // mdp trouvé
      }
      /*else
      {
        return 0;//mdp non trouvé
      }*/

    }
  }
  printf("\n\nnomClient:%s\nHashClient:%d\n\n",client.nom,client.hash);
  close(fd);
  return 0;//mdp non trouvé

}

////////////////////////////////////////////////////////////////////////////////////
int listeClients(CLIENT *vecteur) // le vecteur doit etre suffisamment grand
{
  // TO DO
  CLIENT client;
  int fd, i=0;
  if((fd = open(FICHIER_CLIENTS ,O_RDONLY)) == -1)
  {
    return -1;// si fichier existe pas 
  }
  while(read(fd,&client,sizeof(client))==sizeof(CLIENT))
  {
    strcpy(vecteur->nom,client.nom);
    vecteur->hash=client.hash;
    vecteur++;
    i++;
  }
  close(fd);
  return i;//return nbr de client present in file
}
