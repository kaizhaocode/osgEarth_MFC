#include "pch.h"
#include "COSGObject.h"

COSGObject::COSGObject(HWND hWnd)
{
	m_hWnd = hWnd;
}

COSGObject::~COSGObject(void)
{}


void COSGObject::InitOSG()
{
	InitSceneGraph();
	InitCameraConfig();
	InitOsgEarth();
}

//��ʼ������ͼ
void COSGObject::InitSceneGraph()
{
	mRoot = new osg::Group;
	osg::ref_ptr<osg::Node> earth = osgDB::readNodeFile("clear.earth");
	mRoot->addChild(earth);
	mapNode = dynamic_cast<osgEarth::MapNode*>(earth.get());

	//�ر��ʼ��
	earthLabel = new osg::Group;
	mRoot->addChild(earthLabel);
}

//��ʼ�����
void COSGObject::InitCameraConfig()
{
	//�������Ⱦ�����ھ��֮��

	//����һ������
	RECT rect;
	mViewer = new osgViewer::Viewer;
	//��ȡ��ǰ�ͻ����ĳ���
	::GetWindowRect(m_hWnd, &rect);
	osg::ref_ptr<osg::GraphicsContext::Traits>traits = new osg::GraphicsContext::Traits;
	osg::ref_ptr<osg::Referenced>windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);

	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;
	osg::GraphicsContext*gc = osg::GraphicsContext::createGraphicsContext(traits);

	osg::ref_ptr<osg::Camera>camera = new osg::Camera;
	camera->setGraphicsContext(gc);
	camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0, 1000.0);

	mViewer->setCamera(camera);
	//mViewer->setCameraManipulator(new osgGA::TrackballManipulator);
	mViewer->setSceneData(mRoot);
	mViewer->realize();
	mViewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
	mViewer->getCamera()->setNearFarRatio(0.000003f);
}

void COSGObject::PreFrameUpdate()
{}

void COSGObject::PostFrameUpdate()
{}

//��Ⱦ���߳�
void COSGObject::Render(void *ptr)
{
	//������Ⱦ�߳�
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


void COSGObject::InitOsgEarth()
{
	//��ʼ��������
	em = new osgEarth::Util::EarthManipulator;
	if (mapNode.valid())
	{
		em->setNode(mapNode);
	}
	em->getSettings()->setArcViewpointTransitions(true);
	mViewer->setCameraManipulator(em);
	//���ó�ʼ�ӵ�
	em->setViewpoint(osgEarth::Viewpoint("",116, 40, 0.0, 0.0, -90.0, 3e7),4);


	//��ʼ�����
	osgEarth::Config skyConf;
	osgEarth::DateTime dateTime(2019, 5, 8, 9);
	osg::ref_ptr<osgEarth::Util::SkyNode> sky_node = osgEarth::Util::SkyNode::create(mapNode);;
	sky_node->setDateTime(dateTime);
	sky_node->attach(mViewer,2);
	mRoot->addChild(sky_node);

	//�����ر�
	//addLabel();

	//������ʾ�ӵ���Ϣ�Ŀؼ�
	addViewPointLabel();

	//���ӷɻ�
	addPlane();

}

void COSGObject::addViewPointLabel()
{
	mRoot->addChild(osgEarth::Util::Controls::ControlCanvas::get(mViewer));

	osgEarth::Util::Controls::ControlCanvas*canvas = osgEarth::Util::Controls::ControlCanvas::get(mViewer);

	//��ӿؼ���ʾ�ӵ���Ϣ
	osgEarth::Util::Controls::LabelControl*viewCoords = new osgEarth::Util::Controls::LabelControl("lijieLLLLLLLLLLL",osg::Vec4f(1.0,1.0,1.0,1.0));
	viewCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	viewCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_CENTER);

	viewCoords->setBackColor(0,0,0,0.5);
	viewCoords->setSize(800,50);
	viewCoords->setMargin(10);
	canvas->addChild(viewCoords);


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

	//��ӿؼ���ʾ��꽻����Ϣ
	osgEarth::Util::Controls::LabelControl*mouseCoords = new osgEarth::Util::Controls::LabelControl("lijieLLLLLLLLLLL", osg::Vec4f(1.0, 1.0, 1.0, 1.0));
	mouseCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	mouseCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_CENTER);
	mouseCoords->setBackColor(0, 0, 0, 0.5);
	mouseCoords->setSize(400, 50);
	mouseCoords->setMargin(10);
	canvas->addChild(mouseCoords);


}

void COSGObject::addLabel()
{
	osgEarth::Style style;
	osgEarth::Symbology::TextSymbol*textStyle = style.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	//������ɫ
	textStyle->fill()->color() = osg::Vec4f(1.0,1.0,1.0,1.0);
	//���ñ߿�
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
	plane->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
	mtplane = new osg::MatrixTransform;
	osg::Matrix matrix;
	csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(40.0), osg::DegreesToRadians(116.0), 300000.0, matrix);
	matrix.preMult(osg::Matrix::scale(30000, 30000, 30000));
	mtplane->setMatrix(matrix);
	mtplane->addChild(plane);
	mRoot->addChild(mtplane);
}


void COSGObject::FlyTo(double flylog, double flylat, double flyhei)
{
	em->setViewpoint(osgEarth::Viewpoint("",flylog,flylat,0,0,-90,flyhei),4);
}