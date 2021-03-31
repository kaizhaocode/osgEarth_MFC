#pragma once
#include<osgViewer/Viewer>
#include<osgDB/ReadFile>
#include<osgViewer/api/win32/GraphicsWindowWin32>
#include<osgGA/TrackballManipulator>
#include<osgEarth/MapNode>
#include<osgEarthUtil/EarthManipulator>
#include<osgEarthUtil/Sky>
#include<osg/MatrixTransform>


#include<osg/AnimationPath>
#include<osg/CoordinateSystemNode>
#include <osg/PositionAttitudeTransform>
#include <osgEarthUtil/ExampleResources>

#include<osgEarthUtil/Controls>
#include<osgEarthAnnotation/PlaceNode>
//键盘事件
#include<osgViewer/ViewerEventHandlers>
//osgGA相关的库
#include<osgGA/StateSetManipulator>
#include<osgGA/TrackballManipulator>
#include<osgGA/FlightManipulator>
#include<osgGA/DriveManipulator>
#include<osgGA/KeySwitchMatrixManipulator>
#include<osgGA/AnimationPathManipulator>
#include<osgGA/TerrainManipulator>//地形

#include"DigitalEarth.h"
#include"CLabelControlEventHandler.h"


class COSGObject
{
public:
	COSGObject(HWND hWnd);
	~COSGObject(void);

	void InitOSG();
	void InitSceneGraph();
	//初始化相机
	void InitCameraConfig();
	void PreFrameUpdate();
	void PostFrameUpdate();
	//渲染的线程
	static void Render(void *ptr);
	//获取viewer
	osgViewer::Viewer *getViewer();

	void InitOsgEarth();
	//添加显示视点信息的控件
	void addViewPointLabel();
	CLabelControlEventHandler*labelEvent;

	//新增地标
	void addLabel();

	//飞往
	void FlyTo(double flylog,double flylat,double flyhei,double flypitch);

	//获取em
	osgEarth::Util::EarthManipulator *getEM();

public:
	HWND m_hWnd;
	osgViewer::Viewer *mViewer;
	osg::ref_ptr<osg::Group>mRoot;
	osg::ref_ptr<osgEarth::MapNode>mapNode;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> em;

	//地标
	osg::ref_ptr<osg::Group> earthLabel;

public:
	//地表矩阵操作
	osg::ref_ptr<osg::CoordinateSystemNode>csn;

public:
	osg::ref_ptr<osg::Node> plane;
	osg::ref_ptr<osg::MatrixTransform>mtplane;
	//设置机场
	void addPlane();

	osg::ref_ptr<osg::Node> flyPlane;
	//飞机角度变化
	osg::ref_ptr<osg::MatrixTransform> mtFlySelf;
	//飞机摆放位置
	osg::ref_ptr<osg::MatrixTransform> mtfly;


};
