#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
char lang='z';	//語言: z=中文, e=英文

void loadStr();	//載入str
bool cmdExist(char* s);	//指令在平台上存在?
void setCmdUTF8();	//設定cmd為TUF8編碼
void showLangMenu();	//顯示語言選擇菜單
bool isValidComPort(int port);	//驗證COM埠號碼

int main(){
	setCmdUTF8();
	char* cwd=getcwd(NULL,0);
	// showLangMenu();
	loadStr();
	chdir(cwd);

	//檢查環境是否有arduino-cli
	if(!cmdExist("arduino-cli")){
		printf(str[1]);	//你的系統上找不到arduino-cli，請至"https://docs.arduino.cc/arduino-cli/installation/"安裝
		return 1;
	}

	int i;

	//主菜單循環
	while(1){
		//打印指令列表
		printf("\n%s\n",str[10]);
		for(i=0;i<5;i++) printf("%s\n",str[i+2]);
		if(lang=='z'){
			printf("6.切換語言 (EN)\n");
		}
		else{
			printf("6. Switch Language (中文)\n");
		}

		//接收輸入
		char *cmd=NULL;
		bool need_free=false;
		printf("\n%s",str[7]);
		int index=getchar()-'0';
		getchar();	//清除換行符

		//解析輸入
		switch(index){
			case 1:
				cmd="arduino-cli board list";
				break;
			case 2:
				cmd="arduino-cli compile --fqbn arduino:avr:uno";
				break;
			case 3:
				cmd="arduino-cli upload --fqbn arduino:avr:uno";
				break;
			case 4:
				cmd="arduino-cli cache clean";
				break;
			case 5:{
				int com;
				printf(str[9]);
				scanf("%d",&com);
				getchar();	//清除換行符
				if(!isValidComPort(com)){
					if(lang=='z'){
						printf("COM埠號碼無效 (1-256)\n");
					}else {
						printf("Invalid COM port (1-256)\n");
					}
					continue;
				}
				cmd=(char*)malloc(64*sizeof(char));
				if(cmd==NULL){
					if(lang=='z'){
						printf("記憶體分配失敗\n");
					} else {
						printf("Memory allocation failed\n");
					}
					return 1;
				}
				need_free=true;
			 	sprintf(cmd,"arduino-cli compile --fqbn arduino:avr:uno -p COM%d -u",com);
				break;
			}
			case 6:{
				lang=(lang=='z')?'e':'z';
				loadStr();
				if(lang=='z'){
					printf("語言已切換為中文\n");
				}else {
					printf("Language switched to English\n");
				}
				continue;
			}
			default:
				printf(str[8]);
				continue;
		}
		if(cmd!=NULL){
			system(cmd);
		}
		if(need_free){
			free(cmd);
		}
	}
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

void showLangMenu(){
	printf("=== Language Selection ===\n");
	printf("1. 中文 (Chinese)\n");
	printf("2. English\n");
	printf("Select: ");
	int choice=getchar()-'0';
	getchar();
	lang=(choice==2)?'e':'z';
}

bool isValidComPort(int port){
	return (port > 0 && port <= 256);
}