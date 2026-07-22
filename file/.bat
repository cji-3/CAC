@echo off

echo 我們只會安裝最基礎的必要東西，開發板只安裝UNO的，
echo 其他東西的安裝請參考Arduino官方文檔，或是自Arduino IDE安裝，因為兩者基本是相通的。
echo ------------------------------

echo 建立初始設定檔
arduino-cli config init

echo 更新開發板索引目錄
arduino-cli core update-index

echo 安裝開發板的核心架構
arduino-cli core install arduino:avr