The_Tank_Game
=============

This is a tank game for internship used by Ogre engine(version 1.8.x) CEGUI(version 1.7.x) 
note: As the different version has different math, using a different version maybe occur some building errors!

Build Environment:

Include direct: ..\dependence\OgreSDK\boost ..\dependence\CEGUI\cegui\include\RendererModules\Ogre ..\dependence\CEGUI\cegui\include ..\dependence\OgreSDK\include ..\dependence\fmod\include ..\dependence\ogreMax\include ..\dependence\tinyXml\include ..\dependence\OgreSDK\include\ogre

library inlcude: ..\dependence\OgreSDK\boost\lib ..\dependence\fmod\lib ..\dependence\CEGUI\cegui\lib ..\dependence\OgreSDK\lib\release

library linker: OgreMain.lib OIS.lib CEGUIBase.lib CEGUIOgreRenderer.lib ..\dependence\fmod\lib\fmodex_vc.lib
