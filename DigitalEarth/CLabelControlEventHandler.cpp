#include "pch.h"
#include "CLabelControlEventHandler.h"

CLabelControlEventHandler::CLabelControlEventHandler(osgEarth::Util::Controls::LabelControl*viewLabel,
	osgEarth::Util::Controls::LabelControl*mouseLabel,
	osgEarth::MapNode*mn)
{
	viewCoords = viewLabel;
	mouseCoords = mouseLabel;
	mapNode = mn;
	//nodePath.push_back(mapNode->getTerrainEngine());
}


CLabelControlEventHandler::~CLabelControlEventHandler(void)
{}

bool CLabelControlEventHandler::handle(const osgGA::GUIEventAdapter&ea, osgGA::GUIEventAdapter&aa)
{
	osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (viewer)
	{
		if (ea.getEventType() == ea.FRAME)
		{
			osgUtil::LineSegmentIntersector::Intersections results;
			if (viewer->computeIntersections(ea.getX(), ea.getY(), nodePath, results))
			{
				osgUtil::LineSegmentIntersector::Intersection first = *(results.begin());
				//取出交点坐标
				osg::Vec3d point = first.getWorldIntersectPoint();
				osg::Vec3d lla;

				osg::EllipsoidModel*em2 = new osg::EllipsoidModel();
				//世界坐标系转为经纬度
				em2->convertXYZToLatLongHeight(point.x(),point.y(),point.z(),lla.x(),lla.y(),lla.z());
				lla.x()= osg::RadiansToDegrees(lla.x());
				lla.y()= osg::RadiansToDegrees(lla.y());
				//mapNode->getMap()->worldPointToMapPoint(point,lla);
				char wsrc[512];
				sprintf(wsrc,"Log:%.2f  Lat:%.2f  Heigth:%.2f",lla.x(),lla.y(),lla.z());
				mouseCoords->setText(wsrc);
			}
			//视点信息
			osgEarth::Util::EarthManipulator *em = dynamic_cast<osgEarth::Util::EarthManipulator *>(viewer->getCameraManipulator());
			if (em)
			{
				osgEarth::Viewpoint vp = em->getViewpoint();
				//osg::Vec3d llh = vp.focalPoint();
				char wscr[512];
				sprintf(wscr, "Log:%.2f  Lat:%.2f  Heigth:%.2f", vp.getHeading(), vp.getPitch(), vp.getRange());
				viewCoords->setText(wscr);
			}
		}

		//键盘控制地球旋转
		if (ea.getEventType() == ea.KEYDOWN)
		{
			//得到当前视点
			osgEarth::Util::EarthManipulator *em = dynamic_cast<osgEarth::Util::EarthManipulator *>(viewer->getCameraManipulator());
			osgEarth::Viewpoint vm = em->getViewpoint();
			//osg::Vec3d logLatHei = vm.focalPoint;
			//double fx = logLatHei.x();
			//double fy = logLatHei.y();
			//double fz = logLatHei.z();
			double fx = 116.0;
			double fy = 40.0;
			double fz = 500000.0;
			//航向角
			double heading = vm.getHeading();
			//俯仰角
			double pitch = vm.getPitch();
			double range = vm.getRange();

			if (ea.getKey() == 'P')
			{
				fx++;
			}
			else if (ea.getKey() == 'p')
			{
				fx--;
			}
			else if (ea.getKey() == 'O')
			{
				fy++;
			}
			else if (ea.getKey() == 'o')
			{
				fy--;
			}

			else if (ea.getKey() == 'I')
			{
				fz++;
			}
			else if (ea.getKey() == 'i')
			{
				fz--;
			}

			else if (ea.getKey() == 'U')
			{
				heading++;
			}
			else if (ea.getKey() == 'u')
			{
				heading--;
			}
			else if (ea.getKey() == 'Y')
			{
				pitch++;
			}
			else if (ea.getKey() == 'y')
			{
				pitch--;
			}

			else if (ea.getKey() == 'T')
			{
				range+=1000;
			}
			else if (ea.getKey() == 't')
			{
				range+=1000;
			}

			em->setViewpoint(osgEarth::Viewpoint("",fx,fy,fz,heading,pitch,range));
		}
	}
	return false;
}