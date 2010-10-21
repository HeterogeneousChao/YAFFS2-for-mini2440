/*yaffs_tester.c*/

#include "yaffs_tester.h"



int random_seed;
int simulate_power_failure = 0;


buffer message_buffer;	/*create  message_buffer */



int main(){	
	char yaffs_test_dir[]="/yaffs2/test_dir\0";	/*the path to the directory where all of the testing will take place*/
	char yaffs_mount_dir[]="/yaffs2/\0";		/*the path to the mount point which yaffs will mount*/
	
	init(yaffs_test_dir,yaffs_mount_dir);
	test(yaffs_test_dir);
	yaffs_unmount(yaffs_mount_dir);
	return 0;
}


void init(char yaffs_test_dir[],char yaffs_mount_dir[]){
	/*these variables are already set to zero, but it is better not to take chances*/
	message_buffer.head=0;				 
	message_buffer.tail=0;
	
	add_to_buffer(&message_buffer,"welcome to the yaffs tester\0",MESSAGE_LEVEL_BASIC_TASKS);/* print boot up message*/	
	yaffs_start_up();
    	yaffs_mount(yaffs_mount_dir);

	if (yaffs_access(yaffs_test_dir,0))
	{
		yaffs_mkdir(yaffs_test_dir,S_IREAD | S_IWRITE);
	}
	
}
void join_paths(char path1[],char path2[],char *new_path ){
	unsigned int i=0;
	unsigned int x=0;
	printf("strlen path1:%d\n",strlen(path1));
	printf("strlen path2:%d\n",strlen(path2));
	printf("path1; %s\n",path1);

	//add_to_buffer(&message_buffer, "joining paths\0",MESSAGE_LEVEL_BASIC_TASKS);
	char cat[10]="cat\0";
	strcat(cat,"dog\0");
	add_to_buffer(&message_buffer,path1,MESSAGE_LEVEL_BASIC_TASKS);
	add_to_buffer(&message_buffer, path2,MESSAGE_LEVEL_BASIC_TASKS);
	if ( (path1[(sizeof(path1)/sizeof(char))-2]=='/') && path2[0]!='/') {
		/*paths are compatiable. concatanate them. note -2 is because of \0*/  
		//char new_path[(sizeof(path1)/sizeof(char))+(sizeof(path2)/sizeof(char))];
		strcpy(new_path,strcat(path1,path2)); 
		//return new_path;
	}	
	else if ((path1[(sizeof(path1)/sizeof(char))-2]!='/') && path2[0]=='/') {
		/*paths are compatiable. concatanate them*/  
		//char new_path[(sizeof(path1)/sizeof(char))+(sizeof(path2)/sizeof(char))];
		strcpy(new_path,strcat(path1,path2)); 
		//return new_path;
	}
	else if ((path1[(sizeof(path1)/sizeof(char))-2]!='/') && path2[0]!='/') {
			/*need to add a "/". */  
		strcat(new_path,path1);
		strcat(new_path,"/");
		strcat(new_path,path2);
		//strcpy(new_path,strcat(path1,strcat("/\0",path2)));

/*		copy_array(path1,new_path,0,0);
		copy_array('\0',new_path,0,(sizeof(path1)/sizeof(char)));
		copy_array(path2,new_path,0,(sizeof(path1)/sizeof(char))+1);
 old method now trying to use copy_array
		//char new_path[(sizeof(path1)/sizeof(char))+(sizeof(path2)/sizeof(char))+1];
		for (x=0;x<=(sizeof(path1)/sizeof(char))-1;x++){ 
			new_path[x]=path1[x];
		}
		new_path[x+1]='/';
		for (x=(sizeof(path1)/sizeof(char)) ,i=0 ;i<=(sizeof(path2)/sizeof(char));x++,i++){ 
			new_path[x]=path2[i]; 
		}
*/
		//return new_path;
	}
	else if ((path1[(sizeof(path1)/sizeof(char))-2]=='/') && path2[0]=='/') {
		/*need to remove a "/". */  
		//char new_path[(sizeof(path1)/sizeof(char))+(sizeof(path2)/sizeof(char))-1];
		strcpy(new_path,strcat(path1,strncat("",path2,(sizeof(path1)/sizeof(char))-1))); 
		//return new_path;
	} 
	else{
		//error 
		//return -1;
	}
}
/*
void copy_array(char from[],char *to, unsigned int from_offset,unsigned int to_offset){	
	unsigned int x=0;
	for (x=0+from_offset; x<(sizeof(from)/sizeof(char));x++){
		//add_to_buffer(&message_buffer, x,MESSAGE_LEVEL_BASIC_TASKS);
		//add_to_buffer(&message_buffer,from[x],MESSAGE_LEVEL_BASIC_TASKS);
		printf("x=%d\n",x);
		printf("char in from: %c\n\n",from[x]);
		
		to[x+to_offset]=from[x];
	}
}
*/
void test(char yaffs_test_dir[]){
	char output=0;
	char name[MAX_FILE_NAME_SIZE+3 ]="apple\0";
	char path[MAX_FILE_NAME_SIZE+strlen(yaffs_test_dir)];
	join_paths(yaffs_test_dir,name,path);
	while(1)
	{
		path[0]='\0';// this should clear the path
		generate_random_string(name);
		join_paths(yaffs_test_dir,name,path);
		add_to_buffer(&message_buffer,"trying to open file",MESSAGE_LEVEL_BASIC_TASKS);
		add_to_buffer(&message_buffer,path,MESSAGE_LEVEL_BASIC_TASKS);
		output=yaffs_open(path,O_CREAT | O_TRUNC| O_RDWR, S_IREAD | S_IWRITE);
		yaffs_check_for_errors(output, &message_buffer,"failed to open file","opened file");
	}
}
void  generate_random_string(char *ptr){
	unsigned int x;
	unsigned int length=((rand() %MAX_FILE_NAME_SIZE)+1);	/*creates a int with the number of charecters been between 1 and 51*/ 		
	//printf("generating string\n");
	//printf("string length is %d\n",length);
	for (x=0; x <= (length-2) &&length>2 ; x++)
	{
		//printf("x=%d\n",x);	
		ptr[x]=(rand() % 126-32)+32;	/*generate a number between 32 and 126 and uses it as a charecter (letter) */
		//printf("charecter generated is %c\n",ptr[x]);
	}
	ptr[x+1]='\0';	/*adds NULL charecter to turn it into a string*/
	
}
