#pragma once

#include<osgViewer/Viewer>
#include<osgDB/ReadFile>
#include<osgViewer/api/win32/GraphicsWindowWin32>
#include<osgGA/TrackballManipulator>


class COSGObject
{
public:
	COSGObject(HWND hWnd);
	~COSGObject(void);


	void InitOSG();
	void InitSceneGraaph();
	void InitCameraConfig();
	void PreFrameUpdate();
	void PosFrameUpdate();
	static void Render(void *ptr);

	osgViewer::Viewer *getViewer();
private:
	HWND m_hWnd;
	osgViewer::Viewer *mViewer;
	osg::ref_ptr<osg::Group>mRoot;

};

