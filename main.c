#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<windows.h>

bool cmdExist(const char* s){
	char ss[128];

	snprintf(ss,128,"where %s > nul 2>&1",s);
	return !system(ss);
}

//將子串轉成一int整數
int strToInt(const char *s){
	int i,len=strlen(s),out=0;
	bool sig=true;	//正整數?

	if(s[0]=='-') sig=false;

	for(i=0;i<len;i++){
		if(s[i]>='0' && s[i]<='9') out+=pow(10,len-i-1)*(s[i]-'0');
	}

	return sig?out:-1*out;
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

	//指令沒有傳參
	noInput:{
		printf("0.列出COM口\t");
		printf("1.編譯(驗證)\t");
		printf("2.編譯並上傳\t");
		printf("3.其他指令\t");
		printf("\n歡迎使用CAC，依據指令列表輸入指令索引 >> ");

		int index;
		scanf("%d",&index);
		printf("\n");

		char* cmd;
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
			default:
				printf("目前沒有其他指令可用 :(");
				break;
		}
		system(cmd);

		exit(0);
	}

	//指令有傳參
	yesInput:{
		int index[van-1];
		int i;
		for(i=0;i<van-1;i++) index[i]=strToInt(vas[i+1]);

		char* cmd;
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
			default:
				printf("目前沒有其他指令可用 :(");
				break;
		}
		system(cmd);

		exit(0);
	}

	return 0;
}