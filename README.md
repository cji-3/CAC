# CAC V2.0.0

---

## 全稱：cji arduino-cli
這是一個改善arduino-cli使用體驗的工具。cji是我的綽號!
一切按照指示來就好!
需先裝有arduino-cli，並且已經設定好arduino-cli的環境。
在你的專案資料夾中使用CAC指令來編譯並上傳你的程式碼。
CAC會自動尋找你的程式碼，並且使用arduino-cli來編譯。
本程式之 CLI 命令列介面設計與參數解析，完全遵循 POSIX 1003.1 Utility Syntax Guidelines（POSIX 工具程式語法指南）標準。

---

## 注意
V2.0.0並不支援之前的語法，V2.0.0進行了全面的語法(POSIX)更新，V2.0.0之前的語法是用索引值來做為指令的參數，V2.0.0之後的語法是使用"-l"等來做規範式的CLI程序。

但我們仍然保留了V1.0.0，它是[_main.c](src/_main.c)，我們將它編譯成_CAC.exe。

保證支援的arduino-cli版本：`arduino-cli  版本：1.5.0 交付：dd407d42d 日期：2026-05-19T14:07:59Z`

---

## 用法
[檔案](file/.help) 這個檔案是用來說明CAC的使用方式。

---

## 安裝
將整個資料夾移到你想安裝的位置(如:`C:\Program Files`)並命名為`CAC`，然後將`CAC\bin`資料夾的路徑加入到系統PATH環境變數中。