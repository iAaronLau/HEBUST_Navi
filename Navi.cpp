#pragma execution_character_set("utf-8")
#include "stdafx.h"
#include "Navi.h"
#include<iostream>
#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <cmath>
#include <QVector>
#include <QtWidgets>
#include <string>
#include <fstream>

using namespace std;

//构造函数
Navi::Navi(QWidget *parent)
	: QMainWindow(parent){
	ui.setupUi(this);
	setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
	setWindowTitle("河北科技大学校内导航");
	setWindowIcon(QIcon("./Navi.ico"));
	createComboBox();
	Creatmap();
	//绑定信号和槽
	connect(ui.searchBtn, SIGNAL(clicked(bool)), this, SLOT(Action()));
}

//创建下拉菜单
void Navi::createComboBox() {
	view = ui.mapView;
	scene = new QGraphicsScene;
	QGraphicsPixmapItem *item = scene->addPixmap(QPixmap("keda.jpg"));
	view->setScene(scene);
	view->show();

	ui.cmbStart->addItem(tr("♥经管楼."));
	ui.cmbStart->addItem(tr("风雨操场."));
	ui.cmbStart->addItem(tr("主体育场."));
	ui.cmbStart->addItem(tr("♥公教楼."));
	ui.cmbStart->addItem(tr("♥机关楼."));
	ui.cmbStart->addItem(tr("♥讲堂群."));
	ui.cmbStart->addItem(tr("♥信息电气楼."));
	ui.cmbStart->addItem(tr("♥生工楼."));
	ui.cmbStart->addItem(tr("♥机械楼."));
	ui.cmbStart->addItem(tr("♥图书馆.")); 
	ui.cmbStart->addItem(tr("♥校医院."));
	ui.cmbStart->addItem(tr("西北操场."));
	ui.cmbStart->addItem(tr("1/2号♥公寓."));
	ui.cmbStart->addItem(tr("3号♥公寓."));
	ui.cmbStart->addItem(tr("4号♥公寓."));
	ui.cmbStart->addItem(tr("5/6号♥公寓."));
	ui.cmbStart->addItem(tr("7/8号♥公寓."));
	ui.cmbStart->addItem(tr("9号♥公寓/第二餐厅."));
	ui.cmbStart->addItem(tr("10/11号♥公寓."));
	ui.cmbStart->addItem(tr("12/13号♥公寓."));
	ui.cmbStart->addItem(tr("14/15号♥公寓."));
	ui.cmbStart->addItem(tr("16号♥公寓."));
	ui.cmbStart->addItem(tr("一餐A."));
	ui.cmbStart->addItem(tr("一餐B."));
	ui.cmbStart->addItem(tr("第三餐厅."));
	ui.cmbStart->addItem(tr("金工工厂."));
	ui.cmbStart->addItem(tr("南门."));
	ui.cmbStart->addItem(tr("东门."));
	ui.cmbStart->addItem(tr("西门."));
	ui.cmbStart->addItem(tr("南公教、材料楼."));

	ui.cmbEnd->addItem(tr("♥经管楼."));
	ui.cmbEnd->addItem(tr("风雨操场."));
	ui.cmbEnd->addItem(tr("主体育场."));
	ui.cmbEnd->addItem(tr("♥公教楼."));
	ui.cmbEnd->addItem(tr("♥机关楼."));
	ui.cmbEnd->addItem(tr("♥讲堂群."));
	ui.cmbEnd->addItem(tr("♥信息电气楼."));
	ui.cmbEnd->addItem(tr("♥生工楼."));
	ui.cmbEnd->addItem(tr("♥机械楼."));
	ui.cmbEnd->addItem(tr("♥图书馆."));
	ui.cmbEnd->addItem(tr("♥校医院."));
	ui.cmbEnd->addItem(tr("西北操场."));
	ui.cmbEnd->addItem(tr("1/2号♥公寓."));
	ui.cmbEnd->addItem(tr("3号♥公寓."));
	ui.cmbEnd->addItem(tr("4号♥公寓."));
	ui.cmbEnd->addItem(tr("5/6号♥公寓."));
	ui.cmbEnd->addItem(tr("7/8号♥公寓."));
	ui.cmbEnd->addItem(tr("9号♥公寓/第二餐厅."));
	ui.cmbEnd->addItem(tr("10/11号♥公寓."));
	ui.cmbEnd->addItem(tr("12/13号♥公寓."));
	ui.cmbEnd->addItem(tr("14/15号♥公寓."));
	ui.cmbEnd->addItem(tr("16号♥公寓."));
	ui.cmbEnd->addItem(tr("一餐A."));
	ui.cmbEnd->addItem(tr("一餐B."));
	ui.cmbEnd->addItem(tr("第三餐厅."));
	ui.cmbEnd->addItem(tr("金工工厂."));
	ui.cmbEnd->addItem(tr("南门."));
	ui.cmbEnd->addItem(tr("东门."));
	ui.cmbEnd->addItem(tr("西门."));
	ui.cmbEnd->addItem(tr("南公教、材料楼."));
}

//画出路径
void Navi::DrawPath(QVector<int> v) {
	QGraphicsPathItem *ite = new QGraphicsPathItem();
	QPen pen;
	pen.setWidth(4);
	pen.setColor(Qt::red);
	ite->setPen(pen);
	ite->setFlag(QGraphicsItem::ItemIsPanel);
	scene->addItem(ite);
	QPainterPath pa;
	pa.moveTo(Navi::Map.mpn[v.at(0)].x, Navi::Map.mpn[v.at(0)].y);
	for (int i = 0; i < v.size();i++) {
		pa.lineTo(Navi::Map.mpn[v.at(i)].x, Navi::Map.mpn[v.at(i)].y);
	}
	ite->setPath(pa);
}

//创建图结构
void Navi::Creatmap() {
	//初始化
	Navi::Map.vexnum = sitenum;
	Navi::Map.arcnum = sitenum * sitenum;
	for (int i = 0; i < sitenum;i++) {
		for (int j = 0; j < sitenum; j++) {
			Navi::Map.arcs[i][j] = maxv;
		}
	}
	////连接数据库
	//QSqlDatabase database;
	//database = QSqlDatabase::addDatabase("QSQLITE");
	//database.setDatabaseName("./map.db");
	//if (!database.open()){
	//	qDebug() << "Error: Failed to connect database." << database.lastError();
	//}else{
	//	qDebug() << "Succeed to connect database.";
	//}
	//QSqlQuery sql_query(database);
	////读入点坐标信息
	//if (!sql_query.exec("SELECT * FROM mappoint")){
	//	qDebug() << sql_query.lastError();
	//}
	//else{
	//	for (int i = 0; sql_query.next();i++) {
	//		int id = sql_query.value(0).toInt();
	//		int x = sql_query.value(1).toInt();
	//		int y = sql_query.value(2).toInt();
	//		Navi::Map.mpn[i].id = id;
	//		Navi::Map.mpn[i].x = x;
	//		Navi::Map.mpn[i].y = y;
	//	}
	//}
	////读入弧的联通信息
	//if (!sql_query.exec("SELECT * FROM arcs")) {
	//	qDebug() << sql_query.lastError();
	//}
	//else {
	//	for (int i = 0; sql_query.next(); i++) {
	//		int from = sql_query.value(0).toInt();
	//		int to = sql_query.value(1).toInt();
	//		//通过坐标换算弧长
	//		int diatance = sqrt(pow((Navi::Map.mpn[from].x - Navi::Map.mpn[to].x), 2) + pow((Navi::Map.mpn[from].y - Navi::Map.mpn[to].y), 2));
	//		Navi::Map.arcs[from][to] = diatance;
	//		Navi::Map.arcs[to][from] = diatance;
	//	}
	//}

	//文本存储
	//读入点坐标数据
	ifstream fin;
	fin.open("mappoint.txt", ios::in);
	for (int i = 0; i<sitenum;i++) {
		int x, y, id;
		fin >>id>>x>>y;
		Navi::Map.mpn[i].id = id;
		Navi::Map.mpn[i].x = x;
		Navi::Map.mpn[i].y = y;
	}
	fin.close();
	fin.clear();

	//读入弧连接信息数据
	fin.open("arcs.txt", ios::in);
	for (int i = 0;!fin.eof(); i++) {
		int from, to;
		fin >>from>>to;
		//通过坐标计算弧长
		int diatance = sqrt(pow((Navi::Map.mpn[from].x - Navi::Map.mpn[to].x), 2) + pow((Navi::Map.mpn[from].y - Navi::Map.mpn[to].y), 2));
		Navi::Map.arcs[from][to] = diatance;
		Navi::Map.arcs[to][from] = diatance;
	}
	fin.close();
	fin.clear();
}

//迪杰斯特拉算法，求从起点startPos出发到各个顶点的最短距离
//传入出发点结点编号
void Navi::Dijkstra(int startSite){
	//初始化辅助变量
	for (int i = 0; i < Navi::Map.vexnum; i++) {
		d[i] = maxv; //边e = (u,v)的权值(不存在时为maxv,自己d[i][i]=0)
		used[i] = false;  //已经访问过的结点，相当于s集合
		prev[i] = -1; //最短路上的前驱顶点
	}
	d[startSite] = 0;
	while (true) {
		int v = -1;
		for (int u = 0; u < Navi::Map.vexnum; u++) {
			if (!used[u] && (v == -1 || d[u] < d[v])) {
				v = u;
			}
		}
		if (v == -1) {
			break;
		}
		used[v] = true;
		for (int u = 0; u < Navi::Map.vexnum; u++) {
			if (d[u] > d[v] + Navi::Map.arcs[v][u]) {
				d[u] = d[v] + Navi::Map.arcs[v][u];
				prev[u] = v;
			}
		}
	 }
}

//找到由迪杰斯特拉算法创建好的单源最短路径图找到到endPos点的路径
//即 到顶点endPos的最短路
QVector<int> Navi::GetDijkstraPath(int endSite){
	QVector<int> path;
	for (; endSite != -1; endSite = prev[endSite]) {
		path.push_back(endSite);
	}
	std::reverse(path.begin(), path.end());
	return path;
}

//点击开始的操作
void Navi::Action() {
	Navi::Clear();
	Navi::Start = ui.cmbStart->currentIndex();
	Navi::End = ui.cmbEnd->currentIndex();
	Navi::Dijkstra(Navi::Start);
	Navi::DrawPath(Navi::GetDijkstraPath(ui.cmbEnd->currentIndex()));
}

//清除已绘制的路
void Navi::Clear() {
	QList<QGraphicsItem*> listItem = scene->items();
	while (!listItem.empty()){
		scene->removeItem(listItem.at(0));
		listItem.removeAt(0);
	}
	QGraphicsPixmapItem *item = scene->addPixmap(QPixmap("keda.jpg"));
}