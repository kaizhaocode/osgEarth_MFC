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
//�����¼�
#include<osgViewer/ViewerEventHandlers>
//osgGA��صĿ�
#include<osgGA/StateSetManipulator>
#include<osgGA/TrackballManipulator>
#include<osgGA/FlightManipulator>
#include<osgGA/DriveManipulator>
#include<osgGA/KeySwitchMatrixManipulator>
#include<osgGA/AnimationPathManipulator>
#include<osgGA/TerrainManipulator>//����

#include"DigitalEarth.h"
#include"CLabelControlEventHandler.h"


class COSGObject
{
public:
	COSGObject(HWND hWnd);
	~COSGObject(void);

	void InitOSG();
	void InitSceneGraph();
	//��ʼ�����
	void InitCameraConfig();
	void PreFrameUpdate();
	void PostFrameUpdate();
	//��Ⱦ���߳�
	static void Render(void *ptr);
	//��ȡviewer
	osgViewer::Viewer *getViewer();

	void InitOsgEarth();
	//�����ʾ�ӵ���Ϣ�Ŀؼ�
	void addViewPointLabel();
	CLabelControlEventHandler*labelEvent;

	//�����ر�
	void addLabel();

	//����
	void FlyTo(double flylog,double flylat,double flyhei,double flypitch);

	//��ȡem
	osgEarth::Util::EarthManipulator *getEM();

public:
	HWND m_hWnd;
	osgViewer::Viewer *mViewer;
	osg::ref_ptr<osg::Group>mRoot;
	osg::ref_ptr<osgEarth::MapNode>mapNode;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> em;

	//�ر�
	osg::ref_ptr<osg::Group> earthLabel;

public:
	//�ر�������
	osg::ref_ptr<osg::CoordinateSystemNode>csn;

public:
	osg::ref_ptr<osg::Node> plane;
	osg::ref_ptr<osg::MatrixTransform>mtplane;
	//���û���
	void addPlane();

	osg::ref_ptr<osg::Node> flyPlane;
	//�ɻ��Ƕȱ仯
	osg::ref_ptr<osg::MatrixTransform> mtFlySelf;
	//�ɻ��ڷ�λ��
	osg::ref_ptr<osg::MatrixTransform> mtfly;


};
