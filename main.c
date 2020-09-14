#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <pthread.h>
#define max_id_per_process 15

int  fd, rc = 0, mode=1;


const char *symbolname = "watchdog_open"; //enter the sybom name you want to open 


//dumpmodes for different set of process
typedef enum dump_modes
{
    PARENT_DUMP,
    CHILD_DUMP,
    GENERAL_DUMP
}dump_mode_t;

//checker struct to maintain store ids called by the process
struct check_s{
  int check[max_id_per_process];
     int check_count;

};

/*udate function to maintain the dicitonry of id's*/
void update(struct check_s *c,int id){

	c->check_count++;
	c->check[c->check_count]=id;
	

}

void *checker(void *args)
{

	int id;
	struct check_s *s=(struct check_s*)args;
	id=syscall(359,symbolname,GENERAL_DUMP);//calling the insdump syscall 
	update(s,id);//updating the hashtable of keys

	id=syscall(359,symbolname,GENERAL_DUMP);//calling the insdump syscall 
	update(s,id);//updating the hashtable of keys
	id=syscall(359,symbolname,GENERAL_DUMP);//calling the insdump syscall 
	update(s,id);//updating the hashtable of keys
		
    return NULL;
}


//child process1 
void child_process(struct check_s *c)
{
	int id;
	   pthread_t checkid,checkid2;
		id=syscall(359,symbolname,GENERAL_DUMP);//calling the insdump syscall 
	update(c,id);//updating the hashtable of keys
	/* calling to threads in the same process*/
	 pthread_create(&checkid, NULL, checker, (void *)c);
	pthread_create(&checkid2, NULL, checker, (void *)c);
	
         pthread_join(checkid, NULL);
	 pthread_join(checkid2, NULL);
	
	


}

//child process2
void child_process2(struct check_s *c)
{
	int id;
	 pthread_t checkid;
	printf("in child2");
	id=syscall(359,symbolname,GENERAL_DUMP); //calling the insdump syscall 
	update(c,id);//updating the hashtable of keys
	pthread_create(&checkid, NULL, checker, (void *)c);
	pthread_join(checkid, NULL);
	

}



/*Function to remove all id's at the end of the process*/
void remove_all_id(struct check_s *c){

int count;
count=c->check_count;
while(count>0){
  		syscall(360,c->check[count]);
count--;

}
}



int main()
{



pid_t cpid,cpid2;




   
   mode=0;
   
    static struct check_s c;
    static struct check_s s;

	
    cpid=fork();

	if(cpid==0){    

		child_process(&c); //forking a child process
		fd = open("/dev/watchdog", O_RDWR);  //This will display all dumpstacks inserted at this location by process1 before removing 
		remove_all_id(&c);
		
		
	}
	else if(cpid>0){
		cpid2=fork();

	
	if(cpid2==0){    
		child_process2(&s); //creating second process 
		remove_all_id(&s);

		sleep(4);
		printf("\n \n");
		printf("calling kernel symbol after unregistering all probes\n");
		sleep(2);
		fd = open("/dev/watchdog", O_RDWR); //At this point all the probes are unregistered thus there are no dumpstack operations at this point . 
			
	}
	}
		



    return 0;
}
