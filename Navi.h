#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Navi.h"
#include <QVector>
#include <QtWidgets>

const int sitenum = 107;//地图上点的数量
const int maxv = 65535;//距离最大值，maxv即MaxValue

typedef int PathMatrix[sitenum][sitenum];   // 路径矩阵，二维数组
typedef int ShortPathTable[sitenum];               // 最短距离表，一维数组

//图结点结构体
typedef struct MapNode {
	int id;//地点编号
	int x;//地点x坐标
	int y;//地点y坐标
}MapNode;

//图结构体
typedef struct MapGraph {
	MapNode mpn[sitenum];//mapnodes，储存图结点信息
	int arcs[sitenum][sitenum];//储存路长信息
	int vexnum;//图结点数
	int arcnum;//图弧数
}MapGraph;

//主窗体类
class Navi : public QMainWindow{
	Q_OBJECT

	public:
		Navi(QWidget *parent = Q_NULLPTR);//构造函数
		void createComboBox();//创建下拉菜单
		void Clear();//清除已绘制的路
		void Creatmap();//创建图结构
		void Dijkstra(int startPos);//迪杰斯特拉算法
		void DrawPath(QVector<int> v);//画出路径
		//找到由迪杰斯特拉算法创建好的单源最短路径图找到到endPos点的路径，即 到顶点endPos的最短路
		QVector<int> GetDijkstraPath(int endPos);
		
	private:
		MapGraph Map;//图
		int Start;//起始点编号
		int End;//目的地编号
		int prev[sitenum];  //最短路上的前驱顶点
		int d[sitenum];     //表示边e = (u,v)的权值(不存在时为maxv,自己d[i][i]=0)
		bool used[sitenum]; //已经访问过的结点
		QGraphicsScene *scene;//图像场景
		QGraphicsView *view;//图像视图
		Ui::NaviClass ui;	//UI界面

	//槽
	public slots:
		void Action();//点击开始的操作
};
