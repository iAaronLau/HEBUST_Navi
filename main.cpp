#include "stdafx.h"
#include "Navi.h"
#include <QtWidgets/QApplication>
#include<QPushButton>
#include <QtWidgets>

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	Navi w;//初始化主窗体
	w.show();//显示主窗体
	return a.exec();//执行app
}
