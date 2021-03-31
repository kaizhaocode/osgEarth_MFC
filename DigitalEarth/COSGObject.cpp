#include "pch.h"
#include "COSGObject.h"
#include "DigitalEarth.h"

COSGObject::COSGObject(HWND hWnd)
{
	m_hWnd = hWnd;
	labelEvent = 0;
}

COSGObject::~COSGObject(void)
{}


void COSGObject::InitOSG()
{
	InitSceneGraph();
	InitCameraConfig();
	InitOsgEarth();
}

//初始化场景图
void COSGObject::InitSceneGraph()
{
	mRoot = new osg::Group;
	osg::ref_ptr<osg::Node> earth = osgDB::readNodeFile("clear.earth");
	mRoot->addChild(earth);
	mapNode = dynamic_cast<osgEarth::MapNode*>(earth.get());

	//地标初始化
	earthLabel = new osg::Group;
	mRoot->addChild(earthLabel);

	//解决飞机模型被遮挡问题
	mRoot->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	//解决模型变成黑色的问题
	mRoot->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

}

//初始化相机
void COSGObject::InitCameraConfig()
{
	RECT rect;
	mViewer = new osgViewer::Viewer;
	::GetWindowRect(m_hWnd, &rect);

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	//traits->width = 1000;
	traits->height = rect.bottom - rect.top;
	//traits->height = 800;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;
	osg::GraphicsContext * gc = osg::GraphicsContext::createGraphicsContext(traits);

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(gc);
	//前两个参数设置为0可以解决非全屏问题
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0, 1000.0);

	mViewer->setCamera(camera);
	//mViewer->setCameraManipulator(new osgGA::TrackballManipulator);

	mViewer->setSceneData(mRoot);
	mViewer->realize();
	mViewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
	mViewer->getCamera()->setNearFarRatio(0.000003f);
}

void COSGObject::PreFrameUpdate()
{
	//while (theApp.bNeedModify) Sleep(1);
	//theApp.bCanModify = FALSE;
}

void COSGObject::PostFrameUpdate()
{
	//if (theApp.bNeedModify) theApp.bCanModify = TRUE;

}

//渲染的线程
void COSGObject::Render(void *ptr)
{
	//启动渲染线程
	COSGObject*osg = (COSGObject*)ptr;
	osgViewer::Viewer *viewer = osg->getViewer();
	while (!viewer->done())
	{
		osg->PreFrameUpdate();
		viewer->frame();
		osg->PostFrameUpdate();
	}
	_endthread();
}

osgViewer::Viewer *COSGObject::getViewer()
{
	return mViewer;
}

osgEarth::Util::EarthManipulator *COSGObject::getEM()
{
	return em;
}

void COSGObject::InitOsgEarth()
{
	//初始化操作器
	em = new osgEarth::Util::EarthManipulator;
	if (mapNode.valid())
	{
		em->setNode(mapNode);
	}
	em->getSettings()->setArcViewpointTransitions(true);
	mViewer->setCameraManipulator(em);
	//设置初始视点
	em->setViewpoint(osgEarth::Viewpoint("",116, 40, 0.0, 0.0, -90.0, 3e7),5);

	//添加状态事件，可以相应键盘和鼠标事件，响应L
	mViewer->addEventHandler(new osgGA::StateSetManipulator(mViewer->getCamera()->getOrCreateStateSet()));
	//窗口大小变化，响应F
	mViewer->addEventHandler(new osgViewer::WindowSizeHandler);
	//添加路径记录 Z
	mViewer->addEventHandler(new osgViewer::RecordCameraPathHandler);
	//帮助文档显示H
	mViewer->addEventHandler(new osgViewer::HelpHandler);
	//截屏 C
	mViewer->addEventHandler(new osgViewer::ScreenCaptureHandler);
	//视角追踪
	mViewer->addEventHandler(new osgViewer::ThreadingHandler);
	//添加一些常用状态设置，响应W,S
	mViewer->addEventHandler(new osgViewer::StatsHandler);


	//初始化天空
	osgEarth::Config skyConf;
	osgEarth::DateTime dateTime(2019, 5, 8, 9);
	osg::ref_ptr<osgEarth::Util::SkyNode> sky_node = osgEarth::Util::SkyNode::create(mapNode);;
	sky_node->setDateTime(dateTime);
	sky_node->attach(mViewer,2);
	mRoot->addChild(sky_node);

	//新增地标
	//addLabel();

	//新增显示视点信息的控件
	addViewPointLabel();

	//增加飞机
	addPlane();

}

void COSGObject::addViewPointLabel()
{
	//mRoot->addChild(osgEarth::Util::Controls::ControlCanvas::get(mViewer));
	//画布

	osgEarth::Util::Controls::ControlCanvas*canvas = osgEarth::Util::Controls::ControlCanvas::get(mViewer);
	//添加控件显示视点信息
	osgEarth::Util::Controls::LabelControl*viewCoords = new osgEarth::Util::Controls::LabelControl("lijieLLLLLLLLLLL",osg::Vec4f(1.0,1.0,1.0,1.0));
	viewCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	viewCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_CENTER);
	viewCoords->setBackColor(0,0,0,0.5);
	viewCoords->setSize(800,50);
	viewCoords->setMargin(10);
	canvas->addChild(viewCoords);
	mRoot->addChild(canvas);
	//mViewer->setSceneData(mRoot);
	//mViewer->realize();

	//osgEarth::Util::Controls::ControlCanvas* canvas =osgEarth::Util::Controls::ControlCanvas::get(mViewer);
	//mRoot->addChild(canvas);
	//osgEarth::Util::Controls::Grid* grid = new osgEarth::Util::Controls::Grid();
	//canvas->addControl(grid);
	//grid->setControl(0, 0, new osgEarth::Util::Controls::LabelControl("lijie"));
	//osgEarth::Util::Controls::LabelControl* label = new osgEarth::Util::Controls::LabelControl();
	//label->setFont(osgText::readFontFile("simhei.ttf"));
	//label->setFontSize(24.0f);
	//label->setHorizAlign(osgEarth::Util::Control::ALIGN_CENTER);
	//label->setText("hello");
	//grid->setControl(1, 0, label);

	//添加控件显示鼠标交点信息
	osgEarth::Util::Controls::LabelControl*mouseCoords = new osgEarth::Util::Controls::LabelControl("lijieLLLLLLLLLLL", osg::Vec4f(1.0, 1.0, 1.0, 1.0));
	mouseCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	mouseCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_CENTER);
	mouseCoords->setBackColor(0, 0, 0, 0.5);
	mouseCoords->setSize(400, 50);
	mouseCoords->setMargin(10);
	canvas->addChild(mouseCoords);

	if (labelEvent == 0)
	{
		labelEvent = new CLabelControlEventHandler(viewCoords,mouseCoords,mapNode);

	}
	mViewer->addEventHandler(labelEvent);
}

void COSGObject::addLabel()
{
	osgEarth::Style style;
	osgEarth::Symbology::TextSymbol*textStyle = style.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	//设置颜色
	textStyle->fill()->color() = osg::Vec4f(1.0,1.0,1.0,1.0);
	//设置边框
	textStyle->halo()->color() = osg::Vec4f(1.0,0.0,0.0,1.0);
	textStyle->font() = "simsun.ttc";
	textStyle->size() = 40;
	textStyle->pixelOffset() = osg::Vec2s(10,10);

	osg::Image*china = osgDB::readImageFile("1.png");
	const osgEarth::SpatialReference* geoSRS = mapNode->getMapSRS()->getGeographicSRS();
	osgEarth::Annotation::PlaceNode *pn = new osgEarth::Annotation::PlaceNode(mapNode, osgEarth::GeoPoint(geoSRS, 110.0, 40.0),china,"China",style);
	earthLabel->addChild(pn);
}


void COSGObject::addPlane()
{
	csn = new osg::CoordinateSystemNode;
	csn->setEllipsoidModel(new osg::EllipsoidModel());
	plane = osgDB::readNodeFile("cessna.osg");
	mtplane = new osg::MatrixTransform;
	mtplane->addChild(plane);
	mRoot->addChild(mtplane);
	osg::Matrix matrix;
	csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(40.0), osg::DegreesToRadians(116.0), 300000.0, matrix);
	matrix.preMult(osg::Matrix::scale(30000, 30000, 30000));
	mtplane->setMatrix(matrix);

	flyPlane = osgDB::readNodeFile("tank.FLT");
	mtFlySelf = new osg::MatrixTransform;
	mtFlySelf->addChild(flyPlane);

	mtfly = new osg::MatrixTransform;
	mtfly->addChild(mtFlySelf);
	mRoot->addChild(mtfly);

	csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.3834),osg::DegreesToRadians(109.1347),0,matrix);
	matrix.preMult(osg::Matrix::scale(30, 30, 30));
	mtfly->setMatrix(matrix);
 
}


void COSGObject::FlyTo(double flylog, double flylat, double flyhei,double flypitch)
{
	em->setViewpoint(osgEarth::Viewpoint("",flylog,flylat,0,0,flypitch,flyhei),5);
}