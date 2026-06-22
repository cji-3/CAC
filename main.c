#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>

#ifdef _WIN32
	#include<windows.h>
	#include<direct.h>
	#define getcwd _getcwd
	#define chdir _chdir
#else
	#include<sys/wait.h>
#endif

char str[12][128];	//字串

void loadStr();	//載入str
bool cmdExist(char* s);	//指令在平台上存在?
void setCmdUTF8();	//設定cmd為TUF8編碼

int main(){
	setCmdUTF8();
	char* cwd=getcwd(NULL,0);
	loadStr();
	chdir(cwd);

	//檢查環境是否有arduino-cli
	if(!cmdExist("arduino-cli")){
		printf(str[1]);	//你的系統上找不到arduino-cli，請至"https://docs.arduino.cc/arduino-cli/installation/"安裝
		return 1;
	}

	int i;

	//打印指令列表
	printf("%s\n",str[10]);
	for(i=0;i<5;i++) printf("%s\n",str[i+2]);

	//接收輸入
	char *cmd=NULL;
	printf("\n%s",str[7]);
	int index=getchar()-'0';

	//解析輸入
	switch(index){
		case 1:
			cmd="arduino-cli board list";
			break;
		case 2:
			cmd="arduino-cli compile --fqbn arduino:avr:uno";
			break;
		case 3:

			break;
		case 4:

			break;
		case 5:{
			int com;
			printf(str[9]);
			scanf("%d",&com);
			cmd=(char*)malloc(64*sizeof(char));
		 	sprintf(cmd,"arduino-cli compile --fqbn arduino:avr:uno -p COM%d -u",com);
			break;
		}
		default:
			printf(str[8]);
			break;
	}
	system(cmd);
}

void loadStr(){
	//移動到EXE本身的路徑，這樣才能讀到str/的檔案
	#ifdef _WIN32
		char path[1024];
		GetModuleFileName(NULL,path,sizeof(path));
		char* _path=strrchr(path,'\\');
		*_path='\0';
		chdir(path);
	#else
		//稍後補上
	#endif

	FILE* strfile=fopen("str/zh","r");
	int i=0;
	while(fgets(str[i],128,strfile)){
		str[i][strlen(str[i])-1]='\0';	//去除換行符
		i++;
	}
	fclose(strfile);
}

bool cmdExist(char* s){
	char ss[128];

	#ifdef _WIN32
		snprintf(ss,128,"where %s > nul 2>&1",s);
		return !system(ss);
	#else
		snprintf(ss,128,"command -v %s > /dev/null 2>&1",s);
		int ret=system(ss);
		return (ret != -1 && WIFEXITED(ret) && WEXITSTATUS(ret) == 0);
	#endif
}

void setCmdUTF8(){
	#if defined(_WIN32) || defined(_WIN64)
		system("chcp 65001 > nul 2>&1");
	#else
		system("export LANG=en_US.UTF-8 > /dev/null 2>&1");
	#endif
}