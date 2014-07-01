/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   MapLoader.cpp
				created:    5/28/2012
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "MapLoader.h"
#include <iostream>
#include "ObjectManager.h"

namespace Tank
{


	MapLoader::MapLoader(const std::string &filename, ObjectManager* objectFactory) 
		: mHasError(false), mFilename(filename), mObjectFactory(objectFactory){}


	
	MapLoader::~MapLoader(){}


	
	bool MapLoader::loadMap()
	{
		TiXmlDocument doc(mFilename.c_str());
		
		if (!doc.LoadFile())
		{
			mHasError = true;
			mErrorMessage += doc.ErrorDesc();
			return false;
		}
		
		TiXmlHandle hDoc(&doc);
		TiXmlElement *terrain;
		TiXmlHandle hRoot(0);
		
		// <Terrain>
		terrain = hDoc.FirstChildElement().Element();
		if (!terrain)
			return true;
		hRoot = TiXmlHandle(terrain);
		// </Terrain>
		
		
		// <Config>
		TiXmlElement *config = hRoot.FirstChildElement("Config").Element();
		if (!loadConfig(config))			
			return false;
		
		// <Tanks>
		TiXmlElement *tank = hRoot.FirstChildElement("Tanks").FirstChild().Element();
		for (tank; tank; tank = tank->NextSiblingElement())
		{
			if (!loadTank(tank))
				return false;
		}

		// <Nodes>
		TiXmlElement *nodes = hRoot.FirstChildElement("Nodes").Element();
		int ID = 0;
		nodes->Attribute("ID", &ID);
		TiXmlElement *node = nodes->FirstChildElement();
		for (node; node; node = node->NextSiblingElement())
		{
			if (!loadNode(node))
				return false;
		}
		
		return true;	
	}



	/**
	
		<Node Name="Bridge_1#14">
				<Mesh>Bridge_1</Mesh>
				<Position X="34.9863" Y="3.22929" Z="-15.7592"/>
				<Scale X="1" Y="1" Z="1"/>
				<Rotate X="0" Y="0" Z="0" W="1"/>
				<Metarial NormalMat="Normal_Object" DamageMat="Normal_Object"/>
				<ObjectType>OBJECT_NORMAL</ObjectType>
				<Lock>False</Lock>
				<Data Life="100" Defense="1" FlushTime="10"/>
				<Map X="5" Z="10" Value="0"/>
		</Node>

	*/
	bool MapLoader::loadNode(TiXmlElement* node)
	{
		try
		{
			// Node
			// name
			std::string name = node->Attribute("Name");
			std::string mesh = node->FirstChildElement("Mesh")->GetText();

			// position
			double posX = 0, posY = 0, posZ = 0;
			node->FirstChildElement("Position")->Attribute("X", &posX);
			node->FirstChildElement("Position")->Attribute("Y", &posY);
			node->FirstChildElement("Position")->Attribute("Z", &posZ);
			Ogre::Vector3 pos(posX, posY, posZ);

			// scale
			double scaleX = 0, scaleY = 0, scaleZ = 0;
			node->FirstChildElement("Scale")->Attribute("X", &scaleX);
			node->FirstChildElement("Scale")->Attribute("Y", &scaleY);
			node->FirstChildElement("Scale")->Attribute("Z", &scaleZ);
			Ogre::Vector3 scale(scaleX, scaleY, scaleZ);

			// rotate
			double rotateX = 0, rotateY = 0, rotateZ = 0, rotateW = 0;
			node->FirstChildElement("Rotate")->Attribute("X", &rotateX);
			node->FirstChildElement("Rotate")->Attribute("Y", &rotateY);
			node->FirstChildElement("Rotate")->Attribute("Z", &rotateZ);
			node->FirstChildElement("Rotate")->Attribute("W", &rotateW);
			Ogre::Quaternion rotate(rotateW, rotateX, rotateY, rotateZ);

			// matrtial
			std::string normalMat = node->FirstChildElement("Metarial")->Attribute("NormalMat");
			std::string damageMat = node->FirstChildElement("Metarial")->Attribute("DamageMat");

			// objectType
			std::string objectType = node->FirstChildElement("ObjectType")->GetText();
			bool lock = node->FirstChildElement("ObjectType")->GetText() == "True" ? true : false;

			// data
			int life = 0, defense = 0, collision = 0, destructible = 0;
			node->FirstChildElement("Data")->Attribute("Life", &life);
			node->FirstChildElement("Data")->Attribute("Defense", &defense);
			node->FirstChildElement("Data")->Attribute("Collosion", &collision);
			node->FirstChildElement("Data")->Attribute("Destructible", &destructible);

			// map info
			int mapX = 0, mapZ = 0, mapValue = 0;
			node->FirstChildElement("Map")->Attribute("X", &mapX);
			node->FirstChildElement("Map")->Attribute("Z", &mapZ);
			node->FirstChildElement("Map")->Attribute("Value", &mapValue);

			if (mesh == "Birth_Point")
			{
				mMapInfo.mBirthPoint.push_back(pos);
				return true;
			}
			else if (mesh == "Birth_Point_Enemy")
			{
				mMapInfo.mEnemyBirthPoint.push_back(pos);
				return true;
			}

			Object *object = mObjectFactory->createStaticObject(name, mesh);
			{
				object->setPosition(pos);
				object->setScale(scale);
				object->setOrientation(rotate);
				object->mNormalMat = normalMat;
				object->mDamageMat = damageMat;
				object->mLife = life;
				object->mDefense = defense;
				object->mCollision = collision > 0 ? true : false;
				object->mDestructible = destructible > 0 ? true : false;
				object->mMapValue = mapValue;

				// The value added to the 2D map
				mMapInfo.mValues[mapX][mapZ] += mapValue;
				mMapInfo.mMapIndex.insert(std::make_pair(object->getName(), Ogre::Vector2(mapX, mapZ)));

				// Set collision query mask
				object->getBodyEntity()->setQueryFlags(collision > 0 ? COLLSION_COLLISION : COLLSION_PASS);
			}
		}
		catch(...)
		{
			mHasError = true;
			return false;
		}

		return true;
	}


	
	/**
	
	  <Tank Name="Tank_1">
		<Metarial>Metarial</Metarial>
		<Data Number="10" Money="99" MinPower="50" MaxPower="99" LifeValue="99" 
						TankSpeed="8" BulletSpeed="6" AI="" DefenseValue="5"/>
	  </Tank>		
	  
	*/
	bool MapLoader::loadTank(TiXmlElement* tank)
	{
		try
		{
			// Tank
			// name
			std::string name = tank->Attribute("Name");
			std::string metarial = tank->FirstChildElement("Metarial")->GetText();
			
			// data
			TiXmlElement*data = tank->FirstChildElement("Data");
			int number = 0, money = 0;
			int minPower = 0, maxPower = 0, lifeValue = 0;
			int tankSpeed = 0, bulletSpeed = 0, defense = 0;
			
			std::string ai = data->Attribute("AI");
			
			// infos
			data->Attribute("Number", &number);
			data->Attribute("Money", &money);
			data->Attribute("MinPower", &minPower);
			data->Attribute("MaxPower", &maxPower);
			data->Attribute("LifeValue", &lifeValue);
			data->Attribute("TankSpeed", &tankSpeed);
			data->Attribute("BulletSpeed", &bulletSpeed);
			data->Attribute("DefenseValue", &defense);
		}catch(...)
		{
			mHasError = true;
			return false;
		}
		
		return true;
	}
	


	/**
	
	  <Config Name="map001">
		<Type>Forest_1</Type>
		<Mesh>Terr_Forest_1.mesh</Mesh>
		<Mode>Single Player Mode</Mode>
		<Grid X="15" Z="14" Size="10"/>
	  </Config>
	  
	*/
	bool MapLoader::loadConfig(TiXmlElement* config)
	{
		try
		{
			// Config
			// name
			std::string mapName = config->Attribute("Name");
			
			std::string type = config->FirstChildElement("Type")->GetText();
			std::string mesh = config->FirstChildElement("Mesh")->GetText();
			std::string mode = config->FirstChildElement("Mode")->GetText();
			
			// Grid--X--Z--Size
			int gridX = 0, gridZ = 0, gridSize = 0;
			config->FirstChildElement("Grid")->Attribute("X", &gridX);
			config->FirstChildElement("Grid")->Attribute("Z", &gridZ);
			config->FirstChildElement("Grid")->Attribute("Size", &gridSize);
	
			{
				mMapInfo.mName = mapName;
				mMapInfo.mMeshName = mesh;
				mMapInfo.mGameMode = mode;
				mMapInfo.mGridX = gridX;
				mMapInfo.mGridZ = gridZ;
				mMapInfo.mBlockSize = gridSize;

				for (int i = 0; i < gridZ; i++)
					mMapInfo.mValues.push_back(std::vector<int>(gridX, 0));
			}
		}catch(...)
		{
			mHasError = true;
			return false;
		}
		
		return true;
	}


}