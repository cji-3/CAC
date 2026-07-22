/**
 * @file _main.c
 * @brief 選單模式 V1.*.*
 * @date 2026-07-22
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include<stdbool.h>
#include<windows.h>

bool cmdExist(const char* s){
	char ss[128];

	snprintf(ss,128,"where %s > nul 2>&1",s);
	return !system(ss);
}

//將字串轉成一int整數
int strToInt(const char *s){
	int i,len=strlen(s),out=0;
	bool sig=true;	//正整數?

	if(s[0]=='-') sig=false;

	for(i=0;i<len;i++){
		if(s[i]>='0' && s[i]<='9') out+=pow(10,len-i-1)*(s[i]-'0');
	}

	return sig?out:-1*out;
}

bool __systemEnd=false;	//用於與_system()溝通，讓_system()知道__system()結束了沒
/*
待會要被_system()創建的線程函式
接收要執行的命令字串，執行命令後將結果寫入%TMP%\r.ctx
以__systemEnd變數與_system()溝通
*/
DWORD WINAPI __system(LPVOID cmd){
	char *_cmd=(char*)malloc(512*sizeof(char));
	sprintf(_cmd,"%s > %%TMP%%\\r.ctx 2>&1",(char*)cmd);
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
		free(tmp);
}

int main(int van,char* vas[]){
	//set cmd for utf8
	system("chcp 65001 > nul 2>&1");

	if(!cmdExist("arduino-cli")){
		printf("你的系統上找不到arduino-cli，請至\"https://docs.arduino.cc/arduino-cli/installation/\"安裝\n");
		system("PAUSE");
		exit(-1);
	}

	if(van==1) goto noInput;
	else goto yesInput;

	bool is3=false;

	//指令沒有傳參
	noInput:{
		printf("0.列出COM口\t");
		printf("1.編譯(驗證)\t");
		printf("2.編譯並上傳\t");
		if(!is3) printf("3.其他指令\t");
		else{
			printf("\n5.串口監視器\t");
			printf("6.初始化arduind-cli\t");
			is3=false;
		}
		printf("\n歡迎使用CAC，依據指令列表輸入指令索引 >> ");

		int index;
		scanf("%d",&index);

		bool isCmd=true;
		char* cmd=NULL;
		switch(index){
			case 0:
				cmd="arduino-cli board list";
				break;
			case 1:
				cmd="arduino-cli compile --fqbn arduino:avr:uno";
				break;
			case 2:
				{
					printf("輸入要上傳的COM埠 >> ");
					int com;
					scanf("%d",&com);
					cmd=(char*)malloc(64*sizeof(char));
		 			snprintf(cmd,64,"arduino-cli compile --fqbn arduino:avr:uno -p COM%d -u",com);
				}
				break;
			case 3:
				is3=true;
				goto noInput;
				break;
			case 5:{
				int com;
				printf("輸入要監視的COM埠 >> ");
				scanf("%d",&com);
				cmd=(char*)malloc(128*sizeof(char));
				sprintf(cmd,"arduino-cli monitor -p COM%d -c baudrate=%d",com,9600);
				system(cmd);
				isCmd=false;
				break;
			}
			case 6:
				system("file\\.bat");
				isCmd=false;
				break;
			default:
				printf("目前沒有其他指令可用 :(");
				isCmd=false;
				break;
		}
		if(isCmd) _system(cmd);

		goto end;
	}

	//指令有傳參
	yesInput:{
		int index[van-1];
		int i;
		for(i=0;i<van-1;i++) index[i]=strToInt(vas[i+1]);

		bool isCmd=true;
		char* cmd=NULL;
		switch(index[0]){
			case 0:
				cmd="arduino-cli board list";
				break;
			case 1:
				cmd="arduino-cli compile --fqbn arduino:avr:uno";
				break;
			case 2:
				{
					if(van<3){
						printf("輸入要上傳的COM埠 >> ");
						int com;
						scanf("%d",&com);
						cmd=(char*)malloc(64*sizeof(char));
		 				snprintf(cmd,64,"arduino-cli compile --fqbn arduino:avr:uno -p COM%d -u",com);
					}
					else{
						int com=index[1];
						cmd=(char*)malloc(64*sizeof(char));
		 				snprintf(cmd,64,"arduino-cli compile --fqbn arduino:avr:uno -p COM%d -u",com);
					}
				}
				break;
			case 3:
				printf("0.列出COM口\t");
				printf("1.編譯(驗證)\t");
				printf("2.編譯並上傳\t");
				printf("3.其他指令\t");
				printf("\n5.串口監視器\t");
				printf("6.初始化arduind-cli\t");
				isCmd=false;
				break;
			case 5:{
				int com;
				if(van<3){
					printf("輸入要監視的COM埠 >> ");
					scanf("%d",&com);
				}
				else com=strToInt(vas[2]);
				cmd=(char*)malloc(128*sizeof(char));
				sprintf(cmd,"arduino-cli monitor -p COM%d -c baudrate=%d",com,9600);
				system(cmd);
				isCmd=false;
				break;
			}
			case 6:
				system("file\\.bat");
				isCmd=false;
				break;
			default:
				printf("目前沒有其他指令可用 :(");
				isCmd=false;
				break;
		}
		if(isCmd) _system(cmd);

		goto end;
	}

	end:
		return 0;
}