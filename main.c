#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>

#ifndef _WIN32
#include <sys/wait.h>
#endif

char str[10][128];

//載入str
void loadStr(){
	FILE* strfile=fopen("str/zh","r");
	int i=0;
	while(fgets(str[i],128,strfile)){
		str[i][strlen(str[i])-1]='\0';	//去除換行符
		i++;
	}
}

//指令在平台上存在?
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

int main(){
	setCmdUTF8();
	loadStr();

	//檢查環境是否有arduino-cli
	if(!cmdExist("arduino-cli")){
		printf(str[1]);	//你的系統上找不到arduino-cli，請至"https://docs.arduino.cc/arduino-cli/installation/"安裝
		return 1;
	}

	int i;

	//指令列表
	for(i=0;i<5;i++){
		printf("%s\n",str[i+2]);
	}

	//接收輸入
	char *cmd=NULL;
	printf("\n%s",str[7]);
	int index=getchar()-'0';
	switch(index){
		case 1:
			cmd="arduino-cli board list";
			break;
		default:
			printf(str[8]);
			break;
	}
	system(cmd);
}