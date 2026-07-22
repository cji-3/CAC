/**
 * @file main.c
 * @brief 標準POSIX_CLI程序 V2.*.*
 * @date 2026-07-22
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>
#include<windows.h>
#include<unistd.h>

#define VERSION "2.0.0"

bool isp=false;	//是否啟用"-p"模式

bool __systemEnd=false;	//用於與_system()溝通，讓_system()知道__system()結束了沒
/*
待會要被_system()創建的線程函式
接收要執行的命令字串，執行命令後將結果寫入%TMP%\r.ctx
以__systemEnd變數與_system()溝通
*/
DWORD WINAPI __system(LPVOID cmd){
	char *_cmd=(char*)malloc(512*sizeof(char));
	sprintf(_cmd,"%s > %%TMP%%\\r.ctx 2>&1",(char*)cmd);

	if(isp) printf("-p:%s\n",(char*)cmd);
	system(_cmd);

	free(_cmd);
	__systemEnd=true;
	return 0;
}

/*
會以"..."來表示命令正在執行的特殊system()函式
接收一個命令字串
原理：
創建一個線程來執行命令(__system())
以__systemEnd變數與__system()溝通
以"..."來表示命令正在執行直到命令執行完畢
__system()會將命令執行結果放到%TMP%\r.ctx
打開該文件並打印(不讓__system()直接打印的原因是因為要先把"..."刪乾淨)
*/
void _system(char* cmd){
	CreateThread(NULL,0,__system,cmd,0,NULL);
	Sleep(100);
	while(true){
		int i;
		for(i=0;i<3;i++){
			putchar('.');
			uint64_t t=GetTickCount()+300;
			while(t>GetTickCount()){
				if(__systemEnd) goto end;
				Sleep(0);
			}
		}
		printf("\b\b\b   \b\b\b");
		Sleep(300);
	}

	end:
		__systemEnd=false;
		printf("\b\b\b   \b\b\b");

		char *tmp=getenv("TMP");	//取得環境變數
		char *path=(char*)malloc(1024*sizeof(char));
		sprintf(path,"%s\\r.ctx",tmp);
		FILE *f=fopen(path,"r");

		char *s=(char*)malloc(1024*sizeof(char));
		while(fgets(s,1024,f)){
			printf("%s",s);
		}

		free(s);
		fclose(f);
		remove(path);	//刪除文件
		free(path);
}

//用於紀錄執行目錄路徑
char _runPath[1024];

//到EXE本身之目錄
void cdExePath(){
	_getcwd(_runPath,sizeof(_runPath));

	char path[1024];
	char *lastSlash;

	GetModuleFileName(NULL,path,sizeof(path));

	lastSlash=strrchr(path,'\\');
	if(lastSlash!=NULL){
		*lastSlash='\0';
	}

	chdir(path);
}

//回到執行的目錄
void cdRunPath(){
	chdir(_runPath);
}

void displayHelp(){
	cdExePath();
	FILE *f=fopen("../file/.help","r");

	if(!f) perror("說明檔遺失");

	char *s=(char*)malloc(256*sizeof(char));
	while(fgets(s,256,f)){
		printf("%s",s);
	}
	cdRunPath();
	fclose(f);
	free(s);
}

/**********************************main***************************************/

int main(int van,char* vas[]){
	//取得目前CMD的編碼以便最後恢復，並將當前編碼設為UTF8
	UINT cp=GetConsoleOutputCP();
	SetConsoleOutputCP(CP_UTF8);

	if(!strcmp(vas[van-1],"-p")) isp=true;
	if(van>1){
		if(!strcmp(vas[1],"-l")){
			_system("arduino-cli board list");
		}
		else if(!strcmp(vas[1],"-c")){
			_system("arduino-cli compile --fqbn arduino:avr:uno");
		}
		else if(!strncmp(vas[1],"-u",2)){
			int com=atoi(vas[2]);
			char cmd[128];
		 	snprintf(cmd,sizeof(cmd),"arduino-cli compile --fqbn arduino:avr:uno -p COM%d -u",com);
			_system(cmd);
		}
		else if(!strcmp(vas[1],"-k")){
			int com=atoi(vas[2]);
			int b=9600;
			char cmd[256];
			bool t=false;

			if(van>3){
				int i=3;
				while(i<van){
					if(!strcmp(vas[i],"-b")){
						b=atoi(vas[i+1]);
						i++;
					}
					else if(!strcmp(vas[i],"-t")) t=true;
					else break;
					i++;
				}
			}

			sprintf(cmd,"arduino-cli monitor -p com%d -c baudrate=%d --quiet",com,b);
			if(t) strncat(cmd," --timestamp",sizeof(cmd)-strlen(cmd));

			printf("監看的COM埠:%d\n按下<ctrl><c>來結束監看\n",com);
			system(cmd);
		}
		else if(!strcmp(vas[1],"-i")){
				system("file\\.bat");
		}
		else if(!strcmp(vas[1],"-v")){
			printf("V"VERSION"\n保證支援的arduino-cli版本：1.5.0");
		}
		else{	//-h
			displayHelp();
		}
	}
	else displayHelp();

	//恢復原本的編碼
	SetConsoleOutputCP(cp);
}